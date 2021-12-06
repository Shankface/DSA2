#include "./hash.h"
#include <vector>
#include <iostream>

using namespace std;

hashTable::hashTable(int size){
    this->capacity = hashTable::getPrime(size);
    this->data = vector<hashTable::hashItem>(this->capacity); 
}

int hashTable::insert(const string &key, void *pv){
    if((2 * this->filled) >= this->capacity){ //rehash if capacity reaches half full and returns 2 if rehash fails
        if(this->rehash() == false) return 2;  
    }

    int index = this->hash(key);
    bool success = false;

    while(true){
        hashTable::hashItem item = this->data[index];

        if(!item.isDeleted){
            
            if(item.isOccupied){
                if(item.key == key) return 1; //returns 1 if key already exists
                else {
                    index = (index + 1) % this->capacity;
                }                
            }

            else {
                this->data[index].key = key;
                this->data[index].isOccupied = true;
                this->data[index].pv = pv;
                this->filled++;
                return 0; //returns 0 if key is successfully inserted
            }
        }

        else {index = (index + 1)%capacity; }
    }
}


bool hashTable::contains(const string &key){
    return this->findPos(key) != -1; 
}

void* hashTable::getPointer(const string &key, bool *b) {
    int index = this->findPos(key);
    
    if(b != nullptr){
        *b = (index != -1);
    }
    return index != -1 ? this->data[index].pv : nullptr;
}


int hashTable::setPointer(const string &key, void *pv){
    int index = this->findPos(key);
    if(index != -1){
        this->data[index].pv = pv;
        return 0;
    }
    else return 1;
}

bool hashTable::remove(const string &key){
    int index = this->findPos(key);
    if(index != -1){
        this->data[index].isDeleted = true;
        return true;
    }
    else return false;
}


int hashTable::hash(const std::string &key){
    unsigned int hash = 0;
    for(char c: key){
        hash = hash*67 + (int)c; //67 is a prime number that allows for less collisions
    }
    return hash % this->capacity;
}


int hashTable::findPos(const std::string &key){
    const int initial_index = this->hash(key);
    int index = initial_index;
    bool repeat = false;

    while(!repeat || index < initial_index){ //as long as the search hasn't wrapped to the beggining keep searching 
        hashTable::hashItem item = this->data[index];

        if(!item.isDeleted){
            if(!item.isOccupied) return -1; //if there's an empty slot then it can't of been added in the first place
            else if(item.isOccupied){
                if(item.key == key) return index;
                else index = (index + 1) % this->capacity;
            }
        }

        else if(item.isDeleted) index = (index + 1) % this->capacity;
        if(index < initial_index) repeat = true;
    }
    return -1;
}


bool hashTable::rehash(){
    vector<hashTable::hashItem> old = {this->data.begin(), this->data.begin() + this->capacity}; //copy old hash into buffer vector

    const int new_size = hashTable::getPrime(this->capacity); 
    if(new_size == -1) return false;
    this->capacity = new_size;
    this->filled = 0;
    this->data = vector<hashTable::hashItem>(this->capacity);
    for(auto item: old){ //make new vector with new capacity and copy old data into it
        if(item.isOccupied && !item.isDeleted){
            this->insert(item.key,item.pv);
        }
    }
    return true; 
}

unsigned int hashTable::getPrime(int size){ //each next prime number is about double of last
    const int primes[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593,49157, 98317, 196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};
    for(int prime : primes){
        if(prime > size)
            return prime; 
    }
    return -1; //size is too large
}