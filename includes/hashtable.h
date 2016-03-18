/**
 * @file hashtable.ch
 * @author Jorge Cifuentes <jorge.cifuentes95@gmail.com>
 * @date 18 mar 2016
 *
 * @brief File containing hashtable library functions definition.
 * 
 * This hashtable consists of a fixed-size main array (of size ht->size),
 * and a linked list for every one of this main array positions.
 * The hashtable itself it's a matrix (a pointer to an array) of ht_entry, 
 * which is a combination of a key and a value, encapsulated on its own
 * struct so it can be changed easily.
 * 
 * The hashtable has as well a internal iterator, so it can be used to iterate trough
 * its elements.
 *
 */

#ifndef HASHTABLE_H
#define	HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


#define HT_OK 1
#define HT_KEYEXISTS 2

#define HT_ERR -1
#define HTERR_BADPARAMS -2
#define HTERR_KEYALREADYEXISTS -3
#define HTERR_CANTFINDKEY -4


struct table_val { // valor de los elementos en la tabla
	int v;
};  
typedef struct table_val ht_value;


struct table_entry {
	char *key;
	ht_value *value;
	struct table_entry *next;
};  
typedef struct table_entry ht_entry;


struct table_iterator {
	int index;
	ht_entry *current;
};  
typedef struct table_iterator ht_iterator;


struct table_struct {
	int size;
	ht_entry **content;
	ht_iterator *iterator;
};
typedef struct table_struct hashtable;


/* Hashtable public functions */

 /**
  * @brief      Creates a new hash table given a size.
  *
  * @param      size  Size of the hash table to be created. Must be greater than
  *                   1.
  *
  * @return     The hash table on success, NULL otherwise.
  */
hashtable* ht_create (int8_t size);


/**
 * @brief      Libera la memoria asociada a una tabla hash.
 *
 * @param      h     Tabla a liberar.
 */
void clearHashTable (hashtable* h);


/**
 * @brief      Devuelve el valor mapeado.
 *
 * @param      h     Tabla hash
 * @param      key   Clave
 *
 * @return     La direccion del value de key o NULL si no existe
 */
ht_value *ht_getvalue (hashtable *h, char *key);


/**
 * @brief      Inserts a key-value pair in the table.
 *
 * @param      h      Hash table.
 * @param      key    Key.
 * @param      value  Value to be mapped to key.
 *
 * @return     HT_OK on success, a number lesser than 0 otherwise.
 */
int8_t ht_insert (hashtable *h, char *key, ht_value *value);


/**
 * @brief      Checks whether or not a given key exists in a hashtable.
 *
 * @param      h     Hash table.
 * @param      key   Key to check.
 *
 * @return     HT_KEYEXISTS on success, a number lesser than 0 otherwise.
 */
int8_t ht_keyexists (hashtable *h, char *key);


/**
 * @brief      Borra un elemento de la tabla hash.
 *
 * @param      h     Tabla hash
 * @param      key   Clave
 *
 * @return     { description_of_the_return_value }
 */
int8_t ht_deletenode (hashtable *h, char *key);


/**
 * @brief      Dada una tabla hash, devuelve su elemento valido siguiente (es
 *             decir, no nulo y no procesado aun). Hace uso del iterador interno
 *             de la tabla.
 *
 * @param      h     Tabla hash
 *
 * @return     El contenido de la tabla o NULL si no hay mas
 */
ht_entry *ht_iterate (hashtable *h);


void ht_print (FILE *f, hashtable *h);

#endif	/* HASHTABLE_H */