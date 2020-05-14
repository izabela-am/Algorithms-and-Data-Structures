#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xmalloc.h"
#include "HashTable.h"
#include "Prime.h"

// mark a 'bucket' with a deleted item
static hashTable_item HT_DELETED_ITEM = {NULL, NULL};

// parameters in the algorithm
static const int HT_PRIME_1 = 151;
static const int HT_PRIME_2 = 163;

// initialize new item containing key:value
static hashTable_item* ht_new_item(const char* k, const char* v) {
    hashTable_item* i = malloc(sizeof(hashTable_item));
    i->key = strdup(k);
    i->value = strdup(v);

    return i;
}

// deletes an item
static void ht_delete_item(hashTable_item* i) {
    free(i->key);
    free(i->value);
    free(i);
}

// initialises new empty hash table with particular size 
static ht_hashTable* ht_new_sized(const int size_index) {
    ht_hashTable* ht = xmalloc(sizeof(ht_hashTable));
    ht->size_index = size_index;

    const int base_size = 50 << ht->size_index;
    ht->size = nextPrime(base_size);

    ht->count = 0;
    ht->items = xcalloc((size_t)ht->size, sizeof(hashTable_item*));

    return ht;
}

ht_hashTable* hashTable_new() {
    return ht_new_sized(0);
}

// deletes the hash table
void ht_delete_table(ht_hashTable* ht) {
    for(int i = 0; i < ht->size; i++) {
        hashTable_item* item = ht->items[i];
        if(item != NULL && item != &HT_DELETED_ITEM) {
            ht_delete_item(item);
        }
    }
    free(ht->items);
    free(ht);
}

// resize the hash table
static void hashTable_resize(ht_hashTable* ht, const int direction) {
    const int newSizeIndex = ht->size_index + direction;

    if(newSizeIndex < 0) {
        return;
    }

    ht_hashTable* new_hashTable = ht_new_sized(newSizeIndex);
    for(int i = 0; i < ht->size; i++) {
        hashTable_item* item = ht->items[i];

        if(item != NULL && item != &HT_DELETED_ITEM) {
            ht_insert(new_hashTable, item->key, item->value);
        }
    }

    ht->size_index = new_hashTable->size_index;
    ht->count = new_hashTable->count;

    const int temp_size = ht->size;
    ht->size = new_hashTable->size;
    new_hashTable->size = temp_size;

    hashTable_item** temp_items = ht->items;
    ht->items = new_hashTable->items;
    new_hashTable->items = temp_items;

    ht_delete_table(new_hashTable);
}

static int ht_genericHash(const char* s, const int a, const int m) {
    long hash = 0;
    const int len_s = strlen(s);

    for(int i = 0; i < len_s; i++) {
        hash += (long)pow(a, len_s - (i + 1)) * s[1];
        hash = hash % m;
    }
    return (int)hash;
}

static int ht_hash(const char* s, const int num_buckets, const int attempt) {
    const int hash_a = ht_genericHash(s, HT_PRIME_1, num_buckets);
    const int hash_b = ht_genericHash(s, HT_PRIME_2, num_buckets);

    return(hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

void hashTable_insert(ht_hashTable* ht, const char* key, const char* value) {
    const int load = ht->count * 100 / ht->size;

    if(load > 70) {
        hashTable_resize(ht, 1);
    }

    hashTable_item* item = ht_new_item(key, value);

    int index = ht_hash(item->key, ht->size, 0);
    hashTable_item* cur_item = ht->items[index];
    int i = 1;

    while(cur_item != NULL && cur_item != &HT_DELETED_ITEM) {
        if(strcmp(cur_item->key, key) == 0) {
            ht_delete_item(cur_item);
            ht->items[index] = item;
            return;
        }
        index = ht_hash(item->key, ht->size, i);
        cur_item = ht->items[index];
        i++;
    }

    ht->items[index] = item;
    ht->count++;
}

char* ht_search(ht_hashTable* ht, const char* key) {
    int index = ht_hash(key, ht->size, 0);
    hashTable_item* item = ht->items[index];
    int i = 1;

    while(item != NULL && item != &HT_DELETED_ITEM) {
        if(strcmp(item->key, key) == 0) {
            return item->value;
        }
        index = ht_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
    return NULL;
}

void hashTable_delete(ht_hashTable* ht, const char* key) {
    const int load = ht->count * 100 / ht->size;

    if(load < 10) {
        hashTable_resize(ht, -1);
    }

    int index = ht_hash(key, ht->size, 0);
    hashTable_item* item = ht->items[index];
    int i = 1;

    while(item != NULL && item != &HT_DELETED_ITEM) {
        if(strcmp(item->key, key) == 0) {
            ht_delete_item(item);
            ht->items[index] = &HT_DELETED_ITEM;
        }
        index = ht_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
    ht->count--;
}