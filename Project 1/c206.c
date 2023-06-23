/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

// Baturov Illia (xbatur00), student 2. ročníku FIT VUT v Brně
// IAL - 1. domácí úloha, c206
// Úkol byl odevzdán 22.10.2022

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
	list->activeElement = NULL;
	list->firstElement = NULL;
	list->lastElement = NULL;
	return;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
    list->activeElement = NULL;
	list->lastElement = NULL;
	DLLElementPtr tmp = NULL;
	// Dokud není prvním prvkem NULL,
	// prochází každý prvek seznamu list a uvolňuje paměť přidělenou těmto prvkům.
    while (list->firstElement != NULL)
    {
       tmp = list->firstElement;
       list->firstElement = tmp->nextElement;
       free(tmp);
    }
    return;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
    DLLElementPtr head = (DLLElementPtr)malloc(sizeof(struct DLLElement));
    if (head == NULL) {
        DLL_Error();
		return;
    }
	head->data = data;
	// Pokud seznam list je prázdný, vytvoří první a poslední prvek současně.
	if (list->firstElement == NULL || list->lastElement == NULL) {
		head->nextElement = NULL;
		head->previousElement = NULL;
		list->firstElement = head;
		list->lastElement = head;
		return;
	}
	// Tím se prvky vzájemně propojí.
	list->firstElement->previousElement = head;
	head->nextElement = list->firstElement;
	head->previousElement = NULL;
	list->firstElement = head;
    return;
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
    DLLElementPtr tail = (DLLElementPtr)malloc(sizeof(struct DLLElement));
    if (tail == NULL) {
        DLL_Error();
		return;
    }
	tail->data = data;
	// Pokud seznam list je prázdný, vytvoří první a poslední prvek současně.
	if (list->firstElement == NULL || list->lastElement == NULL) {
		tail->nextElement = NULL;
		tail->previousElement = NULL;
		list->firstElement = tail;
		list->lastElement = tail;
		return;
	}
	// Tím se prvky vzájemně propojí.
	list->lastElement->nextElement = tail;
	tail->previousElement = list->lastElement;
	tail->nextElement = NULL;
	list->lastElement = tail;
	return;
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
	list->activeElement = list->firstElement;
	return;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
	list->activeElement = list->lastElement;
	return;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
    if (list->firstElement == NULL || list->lastElement == NULL) {
        DLL_Error();
        return;
    }
	*dataPtr = list->firstElement->data;
    return;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
    if (list->firstElement == NULL || list->lastElement == NULL) {
        DLL_Error();
        return;
    }
	*dataPtr = list->lastElement->data;
	return;
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
	if (list->firstElement == NULL || list->lastElement == NULL) {
        return;
    }
    if (list->activeElement == list->firstElement) {
        list->activeElement = NULL;
    }
    DLLElementPtr remove = list->firstElement;
	// Pokud seznam list má jediný prvek, vyprázdní seznam.
	if (remove->previousElement == NULL && remove->nextElement == NULL) {
		list->firstElement = NULL;
		list->lastElement = NULL;
		free(remove);
		return;
	}
	// Jinak nastaví jako první další prvek seznamu.
    list->firstElement = remove->nextElement;
	list->firstElement->previousElement = NULL;
    free(remove);
    return;
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
	if (list->firstElement == NULL || list->lastElement == NULL) {
        return;
    }
    if (list->activeElement == list->lastElement) {
        list->activeElement = NULL;
    }
    DLLElementPtr remove = list->lastElement;
	// Pokud seznam list má jediný prvek, vyprázdní seznam.
	if (remove->previousElement == NULL && remove->nextElement == NULL) {
		list->firstElement = NULL;
		list->lastElement = NULL;
		free(remove);
		return;
	}
	// Jinak nastaví jako poslední předchozí prvek seznamu.
	list->lastElement = remove->previousElement;
	list->lastElement->nextElement = NULL;
	free(remove);
	return;
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
	if (list->activeElement == NULL || list->activeElement == list->lastElement) {
		return;
	}
	DLLElementPtr remove = list->activeElement->nextElement;
	// Pokud je prvek, který má být odstraněn, posledním prvkem v seznamu,
	// udělá aktivní prvek posledním v seznamu.
	if (remove == list->lastElement) {
		list->activeElement->nextElement = NULL;
		list->lastElement = list->activeElement;
		free(remove);
		return;
	}
	// Spojí prvky, které jsou po stranách odebraného prvku.
	list->activeElement->nextElement = remove->nextElement;
	remove->nextElement->previousElement = list->activeElement;
	free(remove);
	return;
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
	if (list->activeElement == NULL || list->activeElement == list->firstElement) {
		return;
	}
	DLLElementPtr remove = list->activeElement->previousElement;
	// Pokud je prvek, který má být odstraněn, prvním prvkem v seznamu,
	// udělá aktivní prvek prvním v seznamu.
	if (remove == list->firstElement) {
		list->activeElement->previousElement = NULL;
		list->firstElement = list->activeElement;
		free(remove);
		return;
	}
	// Spojí prvky, které jsou po stranách odebraného prvku.
	list->activeElement->previousElement = remove->previousElement;
	remove->previousElement->nextElement = list->activeElement;
	free(remove);
	return;
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {
	if (list->activeElement == NULL) {
        return;
    }
    DLLElementPtr new_element = (DLLElementPtr)malloc(sizeof(struct DLLElement));
    if (new_element == NULL) {
        DLL_Error();
        return;
    }
	new_element->data = data;
	// Nastaví ukazatel dalšího prvku po novém na prvek po aktivním a
	// ukazatel předchozího prvku před novým na aktivní prvek,
	// aby se zachovalo spojení mezi prvky.
    new_element->nextElement = list->activeElement->nextElement;
	new_element->previousElement = list->activeElement;
	list->activeElement->nextElement = new_element;
	// Kontrola, zda je nový prvek posledním,
	// aby seznam neztratil poslední prvek lastELement.
	if (new_element->nextElement == NULL) {
		list->lastElement = new_element;
	}
    return;
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {
	if (list->activeElement == NULL) {
        return;
    }
    DLLElementPtr new_element = (DLLElementPtr)malloc(sizeof(struct DLLElement));
    if (new_element == NULL) {
        DLL_Error();
        return;
    }
	new_element->data = data;
	// Nastaví ukazatel předchozího prvku před novým na prvek před aktivním a
	// ukazatel dalšího prvku po novém na aktivní prvek,
	// aby se zachovalo spojení mezi prvky.
    new_element->previousElement = list->activeElement->previousElement;
	new_element->nextElement = list->activeElement;
	list->activeElement->previousElement = new_element;
	// Kontrola, zda je nový prvek prvním,
	// aby seznam neztratil první prvek firstELement.
	if (new_element->previousElement == NULL) {
		list->firstElement = new_element;
	}
    return;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
	if (list->activeElement == NULL) {
        DLL_Error();
        return;
    }
	*dataPtr = list->activeElement->data;
    return;
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
	if (list->activeElement == NULL) {
        return;
    }
    list->activeElement->data = data;
    return;
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
	if (list->activeElement == NULL) {
        return;
    }
    list->activeElement = list->activeElement->nextElement;
    return;
}

/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
	if (list->activeElement == NULL) {
        return;
    }
    list->activeElement = list->activeElement->previousElement;
    return;
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
	// Vrací TRUE, pokud aktivní prvek není NULL, jinak vrací FALSE.
    return list->activeElement != NULL;
}

/* Konec c206.c */
