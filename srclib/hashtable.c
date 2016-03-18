/**
 * @file hashtable.c
 * @author Jorge Cifuentes <jorge.cifuentes95@gmail.com>
 * @date 18 mar 2016
 *
 * @brief File containing hashtable library functions implementation.
 *
 */

#include "../includes/hashtable.h"

// staticos

/**
 * @brief      Creates a new node, consisting of a key-value pair.
 *
 * @param      key    The key.
 * @param      value  Values associated with the key.
 *
 * @return     The new node on success, NULL otherwise.
 */
static ht_entry *ht_newnode(char *key, ht_value *value);

/**
 * @brief      Una funcion hash para strings.
 *
 * @param      h     Tabla hash
 * @param      key   Clave que hashear
 *
 * @return     El valor hash o -1 en caso de error
 */
static uint16_t hash(hashtable *h, char *key);


hashtable *ht_create(int8_t size) {

    if (size < 1) return NULL;

    hashtable *t = NULL;
    if (!(t = malloc(sizeof (t[0])))) return NULL;
    if (!(t->content = malloc(sizeof (ht_entry *) * size))) return NULL;

    t->size = size;

    int i;
    for (i = 0; i < t->size; i++) {
        t->content[i] = NULL;
    }

    if (!(t->iterator = malloc(sizeof (t->iterator[0])))) return NULL;
    t->iterator->index = 0;
    t->iterator->current = NULL;

    return t;
}

void clearHashTable(hashtable* h) {

    if (!h) return;

    int i;
    ht_entry *node = NULL;

    for (i = 0; i < h->size; i++) {

        node = h->content[i];
        if (node) {
            while (node) {
                ht_entry *temp = NULL;
                temp = node;
                node = node->next;

                if (temp) {
                    free(temp);
                    // free(temp->value);
                    temp = NULL;
                }
            }
        }
    }

    if (h->size != 0) {
        free(h->content);
        h->content = NULL;

        free(h);
        if (h) h->size = 0;
        h = NULL;
    }
}


static ht_entry *ht_newnode(char *key, ht_value *value) {

    if (!key) return NULL;

    ht_entry *newpair;
    if (!(newpair = malloc(sizeof (ht_entry)))) return NULL;
    if (!(newpair->key = malloc(strlen(key) + 1))) return NULL;

    strcpy(newpair->key, key);
    newpair->value = value;

    newpair->next = NULL;

    return newpair;
}


ht_value *ht_getvalue(hashtable *h, char *key) {

    if (!h || !key) return NULL;

    /* Coge el par en la posicion asociada a la clave key. */
    int pos = hash(h, key);
    ht_entry *entry = NULL;

    if (pos >= 0)
        entry = h->content[pos];
    else
        return NULL;


    /* Recorre la linkedlist */
    while (entry && entry->key && strcmp(key, entry->key) > 0) {
        entry = entry->next;
    }

    return (!entry || !entry->key || strcmp(key, entry->key) != 0) ? NULL : entry->value;
}


int8_t ht_insert(hashtable *h, char *key, ht_value *value) {

    if (!h || !key) return HTERR_BADPARAMS;

    ht_entry *next = NULL, *previous = NULL;


    /* Calcula el hash para nuestra clave. */
    int pos;
    if ((pos = hash(h, key)) >= 0)
        next = h->content[pos];
    else
        return pos;

    /* Recorre la LinkedList de esa posicion de la tabla hash hasta situarse
     * entre dos valores tal que se pueden ordenar de la siguiente manera:
     * key(previous), key(nueva) y key(next).
     * (El primer elemento de la lista sera el valor que devuelve hash()). */
    while (next && next->key && strcmp(key, next->key) > 0) {
        previous = next;
        next = next->next;
    }

    /* En este caso el elemento que iria detras del par nuevo, tiene la misma
     * clave que ya queremos insertar, por lo que la insercion no se puede
     * completar. */
    if (next && next->key && strcmp(key, next->key) == 0) {
        return HTERR_KEYALREADYEXISTS;

    } else {
        /* En cualquier otro caso, creamos un nuevo par y lo colocamos entre
         * previous y next. Si previous es null significa que lo colocamos en
         * el inicio de la LinkedList. */
        ht_entry *node = ht_newnode(key, value);
        if (node) {
            node->next = next;
            (previous != NULL) ? (previous->next = node) : (h->content[pos] = node);
        } else
            return HT_ERR;
    }

    return HT_OK;
}

int8_t ht_fill (hashtable *h, char *key, ht_value *value, ...) {
    return HT_ERR;
}


static uint16_t hash(hashtable *h, char *key) {

    if (!h || !key) return HTERR_BADPARAMS;

    uint16_t hash = 7, i, len = strlen(key);

    for (i = 0; i < len; i++) {
        hash = hash * 31 + key[i];
    }

    return hash % h->size;
}


int8_t ht_keyexists (hashtable *h, char *key) {

    if (!h || !key) return HTERR_BADPARAMS;

    /* Checks the position according to the hash function. */
    int pos = hash(h, key);
    ht_entry *entry = NULL;
    if (pos >= 0)
        entry = h->content[pos];
    else
        return pos;


    /* Iterates through the linked list */
    while (entry && entry->key && strcmp(key, entry->key) > 0) {
        entry = entry->next;
    }

    return (!entry || !entry->key || strcmp(key, entry->key) != 0) ? HTERR_CANTFINDKEY : HT_KEYEXISTS;
}


void ht_print (FILE *f, hashtable *h) {

    if (!f || !h) return;

    int i = 0;
    ht_entry *e = NULL;

    while (i < h->size) {
        e = h->content[i];
        while (e) {
            fprintf(f, "[main index %d] %s\n", i, e->key);
            e = e->next;
        }
        i++;
    }
}


ht_entry *ht_iterate (hashtable *h) {

    if ((1 + h->iterator->index) >= h->size) {
        h->iterator->index = 0;
        h->iterator->current = NULL;
        return NULL;
    }

    /* First, it checks the current linked list. */
    if (h->iterator->current != NULL) {
        if (h->iterator->current->next != NULL) { // If the LL has a next node
            h->iterator->current = h->iterator->current->next; // then we advance the pointer a position and
            // we return it.
            return h->iterator->current;
        }

        h->iterator->index++; // In this case the LL hasn't anymore nodes,
        // so we advance one position on the main array.
        h->iterator->current = h->content[h->iterator->index];
    }

    /* We iterate through the main array searching for the next non-null
        position. */
    while (h->iterator->current == NULL) {
        if ((1 + h->iterator->index) >= h->size) {
            h->iterator->index = 0;
            h->iterator->current = NULL;
            return NULL;
        }

        h->iterator->index++;
        h->iterator->current = h->content[h->iterator->index];
    }

    return h->iterator->current;
}


int8_t ht_deletenode (hashtable *h, char *key) {

    if (!h || !key) return HTERR_BADPARAMS;

    int i;
    ht_entry *node;
    for (i = 0; i < h->size; i++) {
        node = h->content[i];
        if (node) {
            while (node) {
                if (strcmp(node->key, key) == 0) {
                    free(node);
                    node = NULL;
                    return 1;
                } else node = node->next;
            }
        }
    }
}