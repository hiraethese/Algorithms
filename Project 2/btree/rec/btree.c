/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

// Baturov Illia (xbatur00), student 2. ročníku FIT VUT v Brně
// IAL - 2. domácí úloha, rec/btree.c
// Úkol byl odevzdán 13.11.2022

#include "../btree.h"
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  if (tree == NULL) {
    return false;
  }
  // Pokud klíč je stejný, zapíše ho do value a vrací true.
  else if (key == tree->key) {
    *value = tree->value;
    return true;
  }
  else if (key < tree->key) {
    bst_search(tree->left, key, value);
  }
  else if (key > tree->key) {
    bst_search(tree->right, key, value);
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  if (tree == NULL) {
    return;
  }
  // Pokud uzel je NULL pointer, alokuje pamět' a vloží prvek do stromu.
  if (*tree == NULL) {
    bst_node_t *new_element = (bst_node_t*)malloc(sizeof(bst_node_t));
    if (new_element == NULL) return;
    new_element->key = key;
    new_element->left = NULL;
    new_element->right = NULL;
    new_element->value = value;
    *tree = new_element;
    return;
  }
  // Pokud uzel má stejný klíč, nahradí ho hodnotou value.
  else if (key == (*tree)->key) {
    (*tree)->value = value;
    return;
  }
  else if (key < (*tree)->key) {
    bst_insert( &((*tree)->left), key, value );
  }
  else if (key > (*tree)->key) {
    bst_insert( &((*tree)->right), key, value );
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if (tree == NULL) {
    return;
  }
  if ((*tree)->right == NULL) {
    target->key = (*tree)->key;
    target->value = (*tree)->value;
    // Pokud prvek zcela vpravo má prvek vlevo, udělá ho novým prvkem vpravo.
    bst_node_t *new_rightmost = (*tree)->left;
    free(*tree);
    *tree = new_rightmost;
    return;
  }
  else {
    bst_replace_by_rightmost( target, &((*tree)->right) );
  }
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
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  if (tree == NULL) {
    return;
  }
  if (*tree == NULL) {
    return;
  }
  // Hledá prvek se stejným klíčem.
  else if (key == (*tree)->key) {
    // Pokud uzel nemá podstromy, uvolní ho.
    if ((*tree)->left == NULL && (*tree)->right == NULL) {
      free(*tree);
      *tree = NULL;
      return;
    }
    // Pokud uzel má levý podstrom.
    else if ((*tree)->left != NULL && (*tree)->right == NULL) {
      bst_node_t *new_parent = (*tree)->left;
      free(*tree);
      *tree = new_parent;
      return;
    }
    // Pokud uzel má pravý podstrom.
    else if ((*tree)->left == NULL && (*tree)->right != NULL) {
      bst_node_t *new_parent = (*tree)->right;
      free(*tree);
      *tree = new_parent;
      return;
    }
    // Pokud uzel má dva podstromy.
    else if ((*tree)->left != NULL && (*tree)->right != NULL) {
      bst_replace_by_rightmost( *tree, &((*tree)->left) );
      return;
    }
  }
  else if (key < (*tree)->key) {
    bst_delete( &((*tree)->left), key );
  }
  else if (key > (*tree)->key) {
    bst_delete( &((*tree)->right), key );
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  if (tree == NULL) {
    return;
  }
  // Pokud levý podstrom není NULL.
  if (*tree != NULL) {
    bst_dispose( &((*tree)->left) );
    bst_dispose( &((*tree)->right) );
    free(*tree);
    *tree = NULL;
    return;
  } 
  else {
    *tree = NULL;
  }
  return;
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  if (tree == NULL) {
    return;
  }
  bst_print_node(tree);
  bst_preorder(tree->left);
  bst_preorder(tree->right);
  return;
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  if (tree == NULL) {
    return;
  }
  bst_inorder(tree->left);
  bst_print_node(tree);
  bst_inorder(tree->right);
  return;
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  if (tree == NULL) {
    return;
  }
  bst_postorder(tree->left);
  bst_postorder(tree->right);
  bst_print_node(tree);
  return;
}

// The end of rec/btree.c
