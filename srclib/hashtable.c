/**
 * @file hashtable.c
 * @author     Jorge Cifuentes <jorge.cifuentes95@gmail.com>
 * @date       20 mar 2016
 *
 * @brief      File containing hashtable library functions implementation.
 */

#include "../includes/hashtable.h"


/* Private functions */

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
 * @brief      A hash functions for strings.
 *
 * @param      h     Hash table.
 * @param      key   Key.
 *
 * @return     El valor hash o -1 en caso de error
 */
static uint16_t hash(hashtable *h, char *key);


static void ht_freenode(ht_entry *node);


static void ht_copynode(ht_entry *dst, ht_entry *src);


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

void ht_clear (hashtable *h) {

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
                    free(temp->key);
                    temp->key = NULL;

                    free(temp);
                    temp = NULL;
                }
            }
        }
    }

    if (h->size != 0) {
        free(h->content);
        h->content = NULL;

        free(h->iterator);
        h->iterator = NULL;

        free(h);
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


static void ht_freenode(ht_entry *node) {
    free(node->key);
    free(node);
}

static void ht_copynode(ht_entry *dst, ht_entry *src) {

    if (!dst || !src) return;

    memcpy(dst, src, sizeof(*src));
    dst->key = strdup (src->key); // memcpy does not duplicate the string, only the value of the ptr
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
            if (previous != NULL) {
                previous->next = node;
            } else {
                h->content[pos] = malloc(sizeof(node));
                ht_copynode(h->content[pos], node);
                ht_freenode(node);
            }
        } else return HT_ERR;
    }

    return HT_OK;
}


int8_t ht_fill (hashtable *h, ...) {

    if (!h) return HTERR_BADPARAMS;

    char *k;
    ht_value *v;

    va_list ap;
    va_start(ap, h);

    while ((k = va_arg(ap, char *)) && (v = va_arg(ap, ht_value *))) {
        ht_insert(h, k, v);
    }

    va_end(ap);
    return HT_OK;
}


static uint16_t hash (hashtable *h, char *key) {

    if (!h || !key) return HTERR_BADPARAMS;

    uint16_t hash = 7, i;

    for (i = 0; i < strlen(key); i++) {
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


    /* Iterates through the linked list. */
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

    int pos;
    pos = hash(h, key);

    if (key < 0) {
        return HTERR_CANTFINDKEY;
    }

    ht_entry *node = h->content[pos];
    void* base_ptr = node;

    while (node) {
        if (strcmp(node->key, key) == 0) {
          
            free(node->key);
            node->key = NULL;
          
            /* If the key is in the main array, we free that as well. */
            if (node == base_ptr) {
                free(base_ptr);
                h->content[pos] = NULL;
            }

            return 1;
        } else node = node->next;
    }

    return HTERR_CANTFINDKEY;
}