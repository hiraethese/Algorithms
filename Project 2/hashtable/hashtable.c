/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

// Baturov Illia (xbatur00), student 2. ročníku FIT VUT v Brně
// IAL - 2. domácí úloha, hashtable.c
// Úkol byl odevzdán 13.11.2022

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
  if (table == NULL) {
    return;
  }
  // Nastaví všechny indexy na NULL.
  for (int index = 0; index < HT_SIZE; index++) {
    (*table)[index] = NULL;
  }
  return;
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  if (table == NULL) {
    return NULL;
  }
  int index = get_hash(key);
  ht_item_t *tmp = (*table)[index];
  while (tmp != NULL && strcmp(tmp->key, key) != 0) {
    tmp = tmp->next;
  }
  // Vrací NULL nebo ukazatel na nalezený prvek.
  return tmp;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  if (table == NULL) {
    return;
  }
  // Pokud existuje prvek se stejným klíčem.
  if (ht_search(table, key) != NULL) {
    ht_search(table, key)->value = value;
    return;
  }
  ht_item_t *new_element = (ht_item_t*)malloc(sizeof(ht_item_t));
  if (new_element == NULL) {
    return;
  }
  new_element->key = key;
  new_element->next = NULL;
  new_element->value = value;
  int index = get_hash(key);
  // Pokud index je prazdný, udělá nový element
  if ((*table)[index] == NULL) {
    (*table)[index] = new_element;
    return;
  }
  ht_item_t *tmp = (*table)[index];
  // Uděla poslední prvek tohoto indexu jako nový prvek.
  while (tmp->next != NULL) {
    tmp = tmp->next;
  }
  tmp->next = new_element;
  return;
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  if (table == NULL) {
    return NULL;
  }
  // Hledá prvek a vrací ukazatel na hodnotu value.
  if (ht_search(table, key) != NULL) {
    return &(ht_search(table, key)->value);
  }
  return NULL;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  if (table == NULL) {
    return;
  }
  int index = get_hash(key);
  // Pokud první prvek indexu je NULL, nic neudělá.
  if ((*table)[index] == NULL) {
    return;
  }
  ht_item_t *tmp = (*table)[index];
  // Pokud první prvek indexu má stejný klíč, uvolní ho a udělá následující prvek prvním.
  if (strcmp((*table)[index]->key, key) == 0) {
    (*table)[index] = tmp->next;
    free(tmp);
    return;
  }
  // Hledáme prvek se stejným klíčem.
  while (tmp->next != NULL) {
    if (strcmp(tmp->next->key, key) == 0) {
      // Dočasný ukazatel na prvek, který budeme uvolňovat.
      ht_item_t *remove = tmp->next;
      // Propojí prvky mezi sebou a uvolní prvek remove.
      tmp->next = remove->next;
      free(remove);
      return;
    }
    tmp = tmp->next;
  }
  return;
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
  if (table == NULL) {
    return;
  }
  // Uvolní každý index a nastaví ho na NULL, jak bylo po inicializace.
  for (int index = 0; index < HT_SIZE; index++) {
    while ((*table)[index] != NULL) {
      ht_item_t *remove = (*table)[index];
      (*table)[index] = (*table)[index]->next;
      free(remove);
    }
  }
  return;
}

// The end of hashtable.c
