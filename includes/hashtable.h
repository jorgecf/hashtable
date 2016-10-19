/**
 * @file hashtable.h
 * @author     Jorge Cifuentes <jorge.cifuentes95@gmail.com>
 * @date       20 mar 2016
 *
 * @brief      File containing hashtable library functions definition.
 *
 *             This hashtable consists of a fixed-size main array (of size ht->
 *             size), and a linked list for every one of this main array
 *             positions. The hashtable itself it's a matrix (a pointer to an
 *             array) of ht_entry, which is a combination of a key and a value,
 *             encapsulated on its own struct so it can be changed easily.
 *
 *             The hashtable has as well a internal iterator, so it can be used
 *             to iterate trough its elements.
 */

#ifndef HASHTABLE_H
#define	HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>


#define HT_OK 1
#define HT_KEYEXISTS 2

#define HT_ERR -1
#define HTERR_BADPARAMS -2
#define HTERR_KEYALREADYEXISTS -3
#define HTERR_CANTFINDKEY -4


struct table_val {
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
 * @brief      Frees all the memory of the hash table.
 *
 * @param      h     Hash table.
 */
void ht_clear (hashtable *h);


/**
 * @brief      Gets a mapped value to a key.
 *
 * @param      h     Hash table
 * @param      key   Key
 *
 * @return     Key or null otherwise
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
 * @brief      Fills the table with a variadic number of key-value params.
 *
 *             This functions uses variadic arguments, so it must be provided a
 *             series of "char *key, ht_value *value" values.
 *
 * @param      h          Hash table.
 *
 * @return     HT_OK on success, a number lesser than 0 otherwise.
 */
int8_t ht_fill (hashtable *h, ...);


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
 * @brief      Deletes an element from the hash table given its key.
 *
 * @param      h     Hash table
 * @param      key   Key
 *
 * @return     HT_OK on success, HTERR_BADPARAMS or HTERR_CANTFINDKEY otherwise
 */
int8_t ht_deletenode (hashtable *h, char *key);


/**
 * @brief      Given a hash table, gets its next valid value (that is, not NULL
 * 						 and not processed yet), useing its internal iterator.
 *
 * @param      h     Hash table
 *
 * @return     The entry value or NULL if the iteration finished
 */
ht_entry *ht_iterate (hashtable *h);


void ht_print (FILE *f, hashtable *h);

#endif	/* HASHTABLE_H */
