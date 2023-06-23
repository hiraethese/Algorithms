/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

// Baturov Illia (xbatur00), student 2. ročníku FIT VUT v Brně
// IAL - 2. domácí úloha, iter/btree.c
// Úkol byl odevzdán 13.11.2022

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  // Nastaví ukazatel na strom na NULL.
  if (tree != NULL) {
    *tree = NULL;
  }
  return;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  if (tree == NULL) {
    return false;
  }
  while (tree != NULL) {
    if (key == tree->key) {
      *value = tree->value;
      return true;
    }
    else if (key < tree->key) {
      tree = tree->left;
    }
    else if (key > tree->key) {
      tree = tree->right;
    }
  }
  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  if (tree == NULL) {
    return;
  }
  bst_node_t *empty_node = *tree;
  bst_node_t *last_node = NULL;
  // Pokud uzel není prázdný.
  while (empty_node != NULL) {
    last_node = empty_node;
    if (key == last_node->key) {
      last_node->value = value;
      return;
    }
    else if (key < last_node->key) {
      empty_node = last_node->left;
    }
    else if (key > last_node->key) {
      empty_node = last_node->right;
    }
  }
  // Pokud uzel je prázdný, alokuje pamět' pro nový prvek.
  bst_node_t *new_node = (bst_node_t*)malloc(sizeof(bst_node_t));
  if (new_node == NULL) {
    return;
  }
  new_node->key = key;
  new_node->left = NULL;
  new_node->right = NULL;
  new_node->value = value;
  // Udělá z nového prvku uzel stromu.
  if (last_node == NULL) {
    *tree = new_node;
  }
  else if (key < last_node->key) {
    last_node->left = new_node;
  }
  else if (key > last_node->key) {
    last_node->right = new_node;
  }
  return;
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if (tree == NULL || *tree == NULL) {
    return;
  }
  bst_node_t *rightmost_node = *tree;
  bst_node_t *previous_node = NULL;
  // Pokud nenajde prvek zcela vpravo od uzlu tree.
  while (rightmost_node->right != NULL) {
    previous_node = rightmost_node; 
    rightmost_node = rightmost_node->right;
  }
  target->key = rightmost_node->key;
  target->value = rightmost_node->value;
  // Pokud prvek zcela vpravo má prvek vlevo...
  if (rightmost_node->left != NULL) {
    previous_node->right = rightmost_node->left;
  }
  else {
    previous_node->right = NULL;
  }
  free(rightmost_node);
  return;
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  if (tree == NULL) {
    return;
  }
  bst_node_t *remove_node = *tree;
  bst_node_t *parent_node = NULL;
  while (remove_node != NULL) {
    if (remove_node->key == key) {
      break;
    }
    parent_node = remove_node;
    if (key < parent_node->key) {
      remove_node = remove_node->left;
    }
    else if (key > parent_node->key) {
      remove_node = remove_node->right;
    }
  }
  // Pokud takový klíč neexistuje.
  if (remove_node == NULL) {
    return;
  }
  // Pokud uzel nemá podstromy, uvolní ho.
  else if (remove_node->left == NULL && remove_node->right == NULL) {
    if (parent_node == NULL) {
      free(*tree);
      *tree = NULL;
      return;
    }
    if (parent_node->left == remove_node) {
      parent_node->left = NULL;
    }
    else if (parent_node->right == remove_node) {
      parent_node->right = NULL;
    }
    free(remove_node);
    return;
  }
  // Pokud uzel má levý podstrom.
  else if (remove_node->left != NULL && remove_node->right == NULL) {
    if (parent_node == NULL) {
      *tree = remove_node->left;
    }
    if (parent_node->left == remove_node) {
      parent_node->left = remove_node->left;
    }
    else if (parent_node->right == remove_node) {
      parent_node->right = remove_node->left;
    }
    free(remove_node);
    return;
  }
  // Pokud uzel má pravý podstrom.
  else if (remove_node->left == NULL && remove_node->right != NULL) {
    if (parent_node == NULL) {
      *tree = remove_node->right;
    }
    if (parent_node->left == remove_node) {
      parent_node->left = remove_node->right;
    }
    else if (parent_node->right == remove_node) {
      parent_node->right = remove_node->right;
    }
    free(remove_node);
    return;
  }
  // Pokud uzel má dva podstromy.
  else if (remove_node->left != NULL && remove_node->right != NULL) {
    bst_replace_by_rightmost( remove_node, &(remove_node->left) );
    return;
  }
  return;
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  if (tree == NULL) {
    return;
  }
  // Inicializace stacku.
  stack_bst_t stack_bst;
  stack_bst_init(&stack_bst);
  do {
    if (*tree == NULL) {
      if ( !stack_bst_empty(&stack_bst) ) {
        *tree = stack_bst_pop(&stack_bst);
      }
    }
    else {
      if ((*tree)->right != NULL) {
        stack_bst_push(&stack_bst, (*tree)->right);
      }
      bst_node_t *remove_node = *tree;
      *tree = (*tree)->left;
      free(remove_node);
    }
  } while ( *tree != NULL || !stack_bst_empty(&stack_bst) );
  return;
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
  if (tree == NULL) {
    return;
  }
  while (tree != NULL) {
    bst_print_node(tree);
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
  return;
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  if (tree == NULL) {
    return;
  }
  // Inicializace stacku.
  stack_bst_t stack_bst;
  stack_bst_init(&stack_bst);
  bst_leftmost_preorder(tree, &stack_bst);
  while ( !stack_bst_empty(&stack_bst) ) {
    tree = stack_bst_pop(&stack_bst);
  }
  return;
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  if (tree == NULL) {
    return;
  }
  while (tree != NULL) {
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
  return;
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  if (tree == NULL) {
    return;
  }
  // Inicializace stacku.
  stack_bst_t stack_bst;
  stack_bst_init(&stack_bst);
  bst_leftmost_inorder(tree, &stack_bst);
  while ( !stack_bst_empty(&stack_bst) ) {
    tree = stack_bst_pop(&stack_bst);
    bst_print_node(tree);
    bst_leftmost_inorder(tree->right, &stack_bst);
  }
  return;
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) {
  if (tree == NULL) {
    return;
  }
  while (tree != NULL) {
    stack_bst_push(to_visit, tree);
    stack_bool_push(first_visit, true);
    tree = tree->left;
  }
  return;
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  if (tree == NULL) {
    return;
  }
  // Inicializace stacku.
  stack_bst_t stack_bst;
  stack_bool_t stack_bool;
  stack_bst_init(&stack_bst);
  stack_bool_init(&stack_bool);
  bool left;
  bst_leftmost_postorder(tree, &stack_bst, &stack_bool);
  while ( !stack_bst_empty(&stack_bst) ) {
    tree = stack_bst_pop(&stack_bst);
    stack_bst_push(&stack_bst, tree);
    left = stack_bool_pop(&stack_bool);
    if (left) {
      stack_bool_push(&stack_bool, false);
      bst_leftmost_postorder(tree->right, &stack_bst, &stack_bool);
    }
    else {
      stack_bst_pop(&stack_bst);
      bst_print_node(tree);
    }
  }
  return;
}

// The end of iter/btree.c
