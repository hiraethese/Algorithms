/* ******************************* c203.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c203 - Fronta znaků v poli                                          */
/*  Referenční implementace: Petr Přikryl, 1994                               */
/*  Přepis do jazyka C: Václav Topinka, září 2005                             */
/*  Úpravy: Kamil Jeřábek, září 2020                                          */
/*          Daniel Dolejška, září 2021                                        */
/*          Daniel Dolejška, září 2022                                        */
/* ************************************************************************** */
/*
** Implementujte frontu znaků v poli. Přesnou definici typů naleznete
** v hlavičkovém souboru c203.h (ADT fronta je reprezentována strukturou Queue,
** která obsahuje pole 'array' pro uložení hodnot ve frontě a indexy firstIndex
** a freeIndex. Všechny implementované funkce musí předpokládat velikost pole
** QUEUE_SIZE, i když ve skutečnosti jsou rozměry statického pole definovány
** MAX_QUEUE. Hodnota QUEUE_SIZE slouží k simulaci fronty v různě velkém poli
** a nastavuje se v testovacím skriptu c203-test.c před testováním
** implementovaných funkcí. Hodnota QUEUE_SIZE může nabývat hodnot v rozsahu
** 1 až MAX_QUEUE.
**
** Index firstIndex ukazuje vždy na první prvek ve frontě. Index freeIndex
** ukazuje na první volný prvek ve frontě. Pokud je fronta prázdná, ukazují
** oba indexy na stejné místo. Po inicializaci ukazují na první prvek pole,
** obsahují tedy hodnotu 0. Z uvedených pravidel vyplývá, že v poli je vždy
** minimálně jeden prvek nevyužitý.
**
** Při libovolné operaci se žádný z indexů (firstIndex i freeIndex) nesnižuje
** vyjma případu, kdy index přesáhne hranici QUEUE_SIZE. V tom případě
** se "posunuje" znovu na začátek pole. Za tímto účelem budete deklarovat
** pomocnou funkci NextIndex, která pro kruhový pohyb přes indexy pole
** využívá operaci "modulo".
**
** Implementujte následující funkce:
**
**    Queue_Init ...... inicializace fronty
**    nextIndex ....... pomocná funkce - viz popis výše
**    Queue_IsEmpty ... test na prázdnost fronty
**    Queue_IsFull .... test, zda je fronta zaplněna (vyčerpána kapacita)
**    Queue_Front ..... přečte hodnoty prvního prvku z fronty
**    Queue_Remove .... odstraní první prvek fronty
**    Queue_Dequeue ... přečte a odstraní první prvek fronty
**    Queue_Enqueue ... zařazení prvku na konec fronty
**
** Své řešení účelně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

// Baturov Illia (xbatur00), student 2. ročníku FIT VUT v Brně
// IAL - 1. domácí úloha, c203
// Úkol byl odevzdán 22.10.2022

#include "c203.h"

int QUEUE_SIZE = MAX_QUEUE;
int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 *
 * TUTO FUNKCI, PROSÍME, NEUPRAVUJTE!
 *
 * @param error_code Interní identifikátor chyby
 */
void Queue_Error( int error_code ) {
	static const char *QERR_STRINGS[MAX_QERR + 1] = {
			"Unknown error",
			"Queue error: ENQUEUE",
			"Queue error: FRONT",
			"Queue error: REMOVE",
			"Queue error: DEQUEUE",
			"Queue error: INIT"
	};

	if (error_code <= 0 || error_code > MAX_QERR)
	{
		error_code = 0;
	}
	printf("%s\n", QERR_STRINGS[error_code]);
	error_flag = 1;
}

/**
 * Inicializujte frontu následujícím způsobem:
 * - všechny hodnoty v poli queue->array nastavte na '*',
 * - index na začátek fronty nastavte na 0,
 * - index prvního volného místa nastavte také na 0.
 *
 * V případě, že funkce dostane jako parametr queue == NULL, volejte funkci
 * Queue_Error(QERR_INIT).
 *
 * @param stack Ukazatel na strukturu fronty
 */
void Queue_Init( Queue *queue ) {
	if (queue == NULL) {
		Queue_Error(QERR_INIT);
		return;
	}
	// Nastaví na '*' všechny hodnoty v poli queue->array.
	for(int index = 0; index < QUEUE_SIZE; index++) {
		queue->array[index] = '*';
	}
	queue->firstIndex = 0;
	queue->freeIndex = 0;
	return;
}

/**
 * Pomocná funkce, která vrací index následujícího prvku v poli.
 * Funkci implementujte jako jediný prikaz využívající operace '%'.
 * Funkci nextIndex budete využívat v dalších implementovaných funkcích.
 *
 * @param index Aktuální index
 */
int nextIndex( int index ) {
	return index % QUEUE_SIZE + 1;
}

/**
 * Vrací nenulovou hodnotu, pokud je fronta prázdná, jinak vrací hodnotu 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param queue Ukazatel na inicializovanou strukturu fronty
 */
int Queue_IsEmpty( const Queue *queue ) {
	// Vrací TRUE, pokud je první prvek první volný, jinak vrací FALSE.
	return queue->firstIndex == queue->freeIndex;
}

/**
 * Vrací nenulovou hodnotu, je-li fronta plná, jinak vrací hodnotu 0.
 * Funkci je vhodné implementovat jedním příkazem return
 * s využitím pomocné funkce nextIndex.
 *
 * @param queue Ukazatel na inicializovanou strukturu fronty
 */
int Queue_IsFull( const Queue *queue ) {
	// Vrací TRUE, pokud jsou použity všechny indexy fronty, jinak vrací FALSE.
	return nextIndex(queue->freeIndex) - queue->firstIndex == QUEUE_SIZE;
}

/**
 * Prostřednictvím parametru dataPtr vrátí znak ze začátku fronty queue.
 * Pokud je fronta prázdná, ošetřete to voláním funkce Queue_Error(QERR_FRONT).
 * Volání této funkce při prázdné frontě je vždy nutné považovat za nekorektní.
 * Bývá to totiž důsledek špatného návrhu algoritmu, ve kterém je fronta
 * použita. O takové situaci se proto musí programátor-vývojář dozvědět.
 * V opačném případě je ladění programů obtížnější!
 *
 * Při implementaci využijte dříve definované funkce Queue_IsEmpty.
 *
 * @param queue Ukazatel na inicializovanou strukturu fronty
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void Queue_Front( const Queue *queue, char *dataPtr ) {
	if (Queue_IsEmpty(queue) == TRUE) {
		Queue_Error(QERR_FRONT);
		return;
	}
	*dataPtr = queue->array[queue->firstIndex];
	return;
}

/**
 * Odstraní znak ze začátku fronty queue. Pokud je fronta prázdná, ošetřete
 * vzniklou chybu voláním funkce Queue_Error(QERR_REMOVE).
 * Hodnotu na uvolněné pozici ve frontě nijak neošetřujte (nepřepisujte).
 * Při implementaci využijte dříve definované funkce Queue_IsEmpty a nextIndex.
 *
 * @param queue Ukazatel na inicializovanou strukturu fronty
 */
void Queue_Remove( Queue *queue ) {
	if (Queue_IsEmpty(queue) == TRUE) {
		Queue_Error(QERR_REMOVE);
		return;
	}
	// Nastaví první prvek fronty na následující.
	queue->firstIndex = nextIndex(queue->firstIndex);
	return;
}

/**
 * Odstraní znak ze začátku fronty a vrátí ho prostřednictvím parametru dataPtr.
 * Pokud je fronta prázdná, ošetřete to voláním funkce Queue_Error(QERR_DEQUEUE).
 *
 * Při implementaci využijte dříve definovaných funkcí Queue_IsEmpty,
 * Queue_Front a Queue_Remove.
 *
 * @param queue Ukazatel na inicializovanou strukturu fronty
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void Queue_Dequeue( Queue *queue, char *dataPtr ) {
	if (Queue_IsEmpty(queue) == TRUE) {
		Queue_Error(QERR_DEQUEUE);
		return;
	}
	Queue_Front(queue, dataPtr);
	Queue_Remove(queue);
	return;
}

/**
 * Vloží znak data do fronty. Pokud je fronta plná, ošetřete chybu voláním
 * funkce Queue_Error(QERR_ENQUEUE). Vkládání do plné fronty se považuje za
 * nekorektní operaci. Situace by mohla být řešena i tak, že by operace
 * neprováděla nic, ale v případě použití takto definované abstrakce by se
 * obtížně odhalovaly chyby v algoritmech, které by abstrakci využívaly.
 *
 * Při implementaci využijte dříve definovaných funkcí Queue_IsFull a nextIndex.
 *
 * @param queue Ukazatel na inicializovanou strukturu fronty
 * @param data Znak k vložení
 */
void Queue_Enqueue( Queue *queue, char data ) {
	if(Queue_IsFull(queue) == TRUE) {
		Queue_Error(QERR_ENQUEUE);
		return;
	}
	// Vloží znak data na první volnou pozici ve frontě.
	queue->array[queue->freeIndex] = data;
	// Nastaví první volný prvek fronty na následující.
	queue->freeIndex = nextIndex(queue->freeIndex);
	return;
}

/* Konec příkladu c203.c */
