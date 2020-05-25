#ifndef HASH_TABLE_H
#define HASH_TABLE_H

// store key and value pairs inside a 'struct'
typedef struct hashTable_item {
    char* key;
    char* value;
} hashTable_item;

// the hash table stores details about size and how many items are inside it
typedef struct {
    int size;
    int size_index;
    int count;
    hashTable_item** items;
} ht_hashTable;

ht_hashTable* ht_new();
void ht_delete_table(ht_hashTable* ht);

void ht_insert(ht_hashTable* ht, const char* key, const char* value);
char* ht_search(ht_hashTable* ht, const char* key);
void ht_delete(ht_hashTable* h, const char* key);

#endif // HASH_TABLE_H
