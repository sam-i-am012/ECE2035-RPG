// ============================================
// The HashTable class file
//
// Copyright 2023 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================

/****************************************************************************
 * Include the Public Interface
 *
 * By including the public interface at the top of the file, the compiler can
 * enforce that the function declarations in the the header are not in
 * conflict with the definitions in the file. This is not a guarantee of
 * correctness, but it is better than nothing!
 ***************************************************************************/
#include "hash_table.h"

/****************************************************************************
 * Include other private dependencies
 *
 * These other modules are used in the implementation of the hash table module,
 * but are not required by users of the hash table.
 ***************************************************************************/
#include <stdlib.h> // For malloc and free
#include <stdio.h>  // For printf

/****************************************************************************
 * Hidden Definitions
 *
 * These definitions are not available outside of this file. However, because
 * the are forward declared in hash_table.h, the type names are
 * available everywhere and user code can hold pointers to these structs.
 ***************************************************************************/
/**
 * This structure represents a hash table.
 * Use "HashTable" instead when you are creating a new variable. [See top comments]
 */
struct _HashTable
{
    /** The array of pointers to the head of a singly linked list, whose nodes
        are HashTableEntry objects */
    HashTableEntry **buckets;

    /** The hash function pointer */
    HashFunction hash;

    /** The number of buckets in the hash table */
    unsigned int num_buckets;
};

/**
 * This structure represents a hash table entry.
 * Use "HashTableEntry" instead when you are creating a new variable. [See top comments]
 */
struct _HashTableEntry
{
    /** The key for the hash table entry */
    unsigned int key;

    /** The value associated with this hash table entry */
    void *value;

    /**
     * A pointer pointing to the next hash table entry
     * NULL means there is no next entry (i.e. this is the tail)
     */
    HashTableEntry *next;
};

/****************************************************************************
 * Private Functions
 *
 * These functions are not available outside of this file, since they are not
 * declared in hash_table.h.
 ***************************************************************************/
/**
 * createHashTableEntry
 *
 * Helper function that creates a hash table entry by allocating memory for it on
 * the heap. It initializes the entry with key and value, initialize pointer to
 * the next entry as NULL, and return the pointer to this hash table entry.
 *
 * @param key The key corresponds to the hash table entry (unsigned int)
 * @param value The value stored in the hash table entry (void)
 * @return The pointer to the hash table entry (HashTableEntry)
 */
static HashTableEntry *createHashTableEntry(unsigned int key, void *value)
{
    // TODO: Implement
    // 1. Create and initialize a new hash table entry given an input key and value
    //     Hint: how do we allocate memory in the heap?
    //     Note: Make sure to initialize the next pointer to null
     
    //allocating memory to the heap using malloc and casting it as a HashTableEntry pointer
    HashTableEntry *entry = (HashTableEntry *)malloc(sizeof(HashTableEntry));
    
    entry->key = key;       //assigns the 'key' param value to the 'key' member of the HashTableEntry data struct
    entry->value = value;   //does the same as the above line, except with 'value'
    entry->next = NULL;     //setting next value to NULL (means there is no next entry)

    // 2. Return the new hash table entry
    return entry;     
}

/**
 * findItem
 *
 * Helper function that checks whether there exists the hash table entry that
 * contains a specific key.
 *
 * @param hashTable The pointer to the hash table.
 * @param key The key corresponds to the hash table entry
 * @return The pointer to the hash table entry, or NULL if key does not exist
 */
static HashTableEntry *findItem(HashTable *hashTable, unsigned int key)
{
    // TODO: Implement
    // 1. Get the bucket number 
    if (!hashTable) { //checks if hashTable is NULL
        return NULL;
    }
    unsigned int bucket = hashTable->hash(key);
    
    // 2. Get the head entry
    HashTableEntry *curr = hashTable->buckets[bucket];

    // 3. If there is nothing in the bucket, return NULL
    if (!curr) {
        return NULL;
    }

    // 4. Loop through the hash table to find if the key matches
    //      4a. While you are not at end node of the hash table 
    //      4b. If the key is found, return the hash table entry
    //      4c. Otherwise, move to the next node
    while (curr) { //while the current entry is not null
        if (curr->key == key) { //if the key is found
            return curr;
        }
        curr = curr->next; //move to the next node
    }
    // 5. Return NULL if the key is not present
    return NULL;
}

/****************************************************************************
 * Public Interface Functions
 *
 * These functions implement the public interface as specified in the header
 * file, and make use of the private functions and hidden definitions in the
 * above sections.
 ****************************************************************************/
// The createHashTable is provided for you as a starting point.
HashTable *createHashTable(HashFunction hashFunction, unsigned int numBuckets)
{
    // The hash table has to contain at least one bucket. Exit gracefully if
    // this condition is not met.
    if (numBuckets == 0)
    {
        printf("Hash table has to contain at least 1 bucket...\n");
        exit(1);
    }

    // Allocate memory for the new HashTable struct on heap.
    HashTable *newTable = (HashTable *)malloc(sizeof(HashTable));

    // Initialize the components of the new HashTable struct.
    newTable->hash = hashFunction;
    newTable->num_buckets = numBuckets;
    newTable->buckets = (HashTableEntry **)malloc(numBuckets * sizeof(HashTableEntry *));

    // As the new buckets are empty, init each bucket as NULL.
    unsigned int i;
    for (i = 0; i < numBuckets; ++i)
    {
        newTable->buckets[i] = NULL;
    }

    // Return the new HashTable struct.
    return newTable;
}

void destroyHashTable(HashTable *hashTable)
{
    // TODO: Implement
    // 1. Loop through each bucket of the hash table to remove all items.
    //      1a. set temp to be the first entry of the ith bucket
    //      1b. delete all entries
    if (!hashTable) {
        return;
    }

    for (unsigned int i = 0; i < hashTable->num_buckets; i++ ) {    //loop through each bucket 
        HashTableEntry *curr = hashTable->buckets[i];   //current entry that will be checked
        HashTableEntry *nextEntry;
        while (curr) {
            nextEntry = curr->next; //next entry
            if (curr->value) {
                free(curr->value); //need to free the memory associated with the values
            }
            free(curr); //then the struct itself can be free
            curr = nextEntry; //updates curr
        }
    }
    // 2. Free buckets (since they are no longer being used)
    free(hashTable->buckets);
    
    // 3. Free hash table
    free(hashTable);
}

void *insertItem(HashTable *hashTable, unsigned int key, void *value)
{
    // TODO: Implement
    //1. First, we want to check if the key is present in the hash table.
    HashTableEntry *curr = findItem(hashTable, key); //uses previous function to check if the key is present

    //2. If the key is present in the hash table, store new value and return old value
    if (curr) {
        void *previous = curr->value; //stores current value
        curr->value = value; //updates with new value
        return previous; //returns old value 
    }
    //3. If not, create entry for new value and return NULL
    else {
        HashTableEntry *newEntry = createHashTableEntry(key, value); //pointer to new entry
        if (newEntry) {
             if (!hashTable) {
                return NULL;
            }
            unsigned int bucket = hashTable->hash(key); //finds the bucket
            newEntry->next = hashTable->buckets[bucket]; //links the new entry to the entries that are already in the bucket
            hashTable->buckets[bucket] = newEntry; //updates bucket to point to the newly made entry
        }
        return NULL;
    }
}

void *getItem(HashTable *hashTable, unsigned int key)
{
    // TODO: Implement
    
    // **NOTE: DIfferences between Find and Get**
    // This function simply calls another function to FIND the item
    // based on the key, and check if the key exist and then return the item's value
    // to GET the value that corresponds to the key in the hash table.
 
 
    //1. First, we want to check if the key is present in the hash table.
    HashTableEntry *curr = findItem(hashTable, key); //checks if the key is present

    //2. If the key exist, return the value
    if (curr) {
        return curr->value; //returns the value at current address
    }
    //3. If not. just return NULL
    return NULL;
}

void *removeItem(HashTable *hashTable, unsigned int key)
{
    // TODO: Implement
    //Remove the item in hash table based on the key and return the old value stored in it.
    //In other words, free the hash table entry from heap and return its old value
 
    //1. Get the bucket number and the head entry
    unsigned int bucket = hashTable->hash(key); //finds the bucket
    HashTableEntry *head = hashTable->buckets[bucket]; //head entry

    void *old = NULL; //creates pointer var that temp holds the old value
    HashTableEntry *removed = NULL; //pointer to store removed entry
    
    //2. If the head holds the key, change the head to the next value, and return the old value
    if ((head) && (head->key == key)) {
        hashTable->buckets[bucket] = head->next; //changes head to next value
        old = head->value; //stores old value so that I can free head
        free(head); //no longer need mem allocation for head 
        return old; //returns old value
    }

    //3. If not the head, search for the key to be removed 
    HashTableEntry *curr = head; //pointer of current entry (which starts at the head)
    while (curr && curr->next != NULL) { //checking if next entry isn't mull because the mem can't be accessed if it doesn't exit
        if (curr->next->key == key) { //key has been found
            removed = curr->next; //pointer to entry that holds key
            curr->next = removed->next; //unlinks the removed entry
            old = removed->value; //stores old val so removed can be free
            free(removed);
            return old;
        }
        curr = curr->next; //moves to next entry in linked list
    }
    
    //4. If the key is not present in the list, return NULL
    return NULL;
    //5. Unlink node from the list, free, and return old value (done during the while loop)
    
}

void deleteItem(HashTable *hashTable, unsigned int key)
{
    // TODO: Implement
    //Delete the item in the hash table based on the key. 
    
    // **NOTE: DIfferences between Remove and Delete**
    // This function simply calls another function that REMOVES the item
    // based on the key, and then free its return value to DELETE it from the hash table
    // You're basically clearing the memory
 
    //1. Remove the entry and free the returned data
    void *old = removeItem(hashTable, key); //removes entry and stores old value
    if (old) {
        free(old); //has to be non-null or else it would potentially cause an error
    }
}
