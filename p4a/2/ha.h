#ifndef __HA_H
#define __HA_H

//SOURCE: http://www.sparknotes.com/cs/searching/hashtables/section3.rhtml


typedef struct _list_t_ {
    char *str;
    struct _list_t_ *next;
} list_t;

typedef struct _hash_table_t_ {
    int size;      
    list_t **table; 
} hash_table_t;

hash_table_t *create_hash_table(int size);

unsigned int hash(hash_table_t *hashtable, char *str);

list_t *lookup_string(hash_table_t *hashtable, char *str);

int add_string(hash_table_t *hashtable, char *str);

void free_table(hash_table_t *hashtable);

#endif
