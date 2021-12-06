#include <vector>
#include <iostream> 
#include "heap.h"
#include "hash.h"
#include "limits.h"

using namespace std;

heap::heap(int capacity){
    this->capacity = capacity;
    this->data = vector<heap::heapItem>(capacity + 1);
    this->hashmap = hashTable(capacity);
}

int heap::insert(const std::string &id, int key, void *pv){

    if(this->endIndex == this->capacity) return 1;
    if(this->hashmap.contains(id)) return 2;

    for(int i = this->endIndex + 1; i > 0; i/=2){
        if(check(i, key) == 0){
            this->data[i].id = id;
            this->data[i].key = key;
            this->data[i].pv = pv; 
            this->hashmap.insert(id , &data[i]); 
            // this->data[i].index = i; 
            this->endIndex++;
            return 0;
        }
        this->data[i] = this->data[i/2];
        // this->data[i].index /= 2;
        this->hashmap.setPointer(this->data[i].id, &(this->data[i]));
    }
    return 0;
}

int heap::setKey(const std::string &id, int key){
    bool exists;
    heapItem * target = (heapItem*)this->hashmap.getPointer(id, &exists);
    void * pv = target->pv; 
    if(!exists) return 1;

    // int i = target->index;
    int i = this->getIndex(target); 
    while(true){
        int checkFlag = check(i,key); 
        if(i == 0) cerr << "index 0 err\n"; 
        if(checkFlag == 0){
            this->data[i].id = id; 
            this->data[i].key = key;
            this->data[i].pv = pv;  
            // this->data[i].index = i;
            this->hashmap.setPointer(this->data[i].id, &(this->data[i]));
            return 0;
        }
        else if(checkFlag == 1){
            this->data[i] = this->data[i/2];
            // this->data[i].index = i;
            this->hashmap.setPointer(this->data[i].id, &(this->data[i]));
            i/=2;
        }
        else if(checkFlag == 2){
            this->data[i] = this->data[i*2];
            // this->data[i].index = i;
            this->hashmap.setPointer(this->data[i].id, &(this->data[i]));
            i *= 2;
        }
        else if(checkFlag == 3){
            this->data[i] = this->data[i*2 + 1];
            // this->data[i].index = i;
            this->hashmap.setPointer(this->data[i].id, &(this->data[i]));
            i = i*2 + 1;
        }
    }
    return 0;
}


int heap::getKey(const std::string &id, bool * found){
    bool exists;
    heapItem * target = (heapItem*)this->hashmap.getPointer(id, &exists);
    *found = exists;
    if(!exists) return 1;
    return target->key;
}

int heap::deleteMin(std::string *pId, int *pKey, void *ppData){
    if(this->endIndex == 0) return 1; 

    if(pId != nullptr) *pId = this->data[1].id; 
    if(pKey != nullptr) *pKey = this->data[1].key;
    if(ppData != nullptr) *(static_cast<void **> (ppData)) = data[1].pv;
    this->hashmap.remove(this->data[1].id); 

    bool right = false;
    heapItem last = this->data[endIndex];
    int key = last.key;
    this->endIndex--;

    for(int i = 1; i <= this->endIndex; i*=2){
        if(right){ //if replacing right child, the iterator should go up by one since its default even
            i++;
            right = false;
        }
        int checkFlag = check(i,key); 
        if(i == 0) cerr << "index 0 err\n"; 
        if(checkFlag == 0){
            this->data[i] = last; 
            // this->data[i].index = i;
            this->hashmap.setPointer(this->data[i].id, &(this->data[i]));
            return 0;
        }
        else if(checkFlag == 2){
            this->data[i] = this->data[i*2];
            // this->data[i].index = i;
            this->hashmap.setPointer(this->data[i].id, &(this->data[i]));
        }
        else if(checkFlag == 3){
            this->data[i] = this->data[i*2 + 1];
            // this->data[i].index = i;
            this->hashmap.setPointer(this->data[i].id, &(this->data[i]));
            right = true;
        }
    }
    return 0;
}

int heap::remove(const std::string &id, int *pKey, void *ppData){

    bool exists; 
    heapItem * item = (heapItem*)this->hashmap.getPointer(id,&exists);
    if(!exists) return 1; 

    if(pKey != nullptr) *pKey = item->key;
    if(ppData != nullptr) *(static_cast<void **> (ppData)) = item->pv;  

    setKey(id, this->data[1].key - 1); //set desired node to top
    return deleteMin(); //pop it off top
}

int heap::check(int index, int key){
    if(index != 1){
        int parentKey = this->data[index/2].key;
        if(parentKey > key) return 1; //parent is bigger
    }

    int leftChildKey = index*2 <= this->endIndex ? this->data[index*2].key : INT_MIN;
    int rightChildKey = index*2 + 1 <= this->endIndex ? this->data[index*2 + 1].key : INT_MIN;

    if(leftChildKey != INT_MIN && leftChildKey < key && leftChildKey < (rightChildKey != INT_MIN ? rightChildKey : leftChildKey + 1)) return 2; //left child is bigger than parent and right child
    if(rightChildKey != INT_MIN && rightChildKey < key) return 3; //right child is bigger than parent and left child

    return 0;  //valid spot in tree                                                                                                     
}


int heap::printHeap(){
    //cout <<  this->endIndex;;
    int r = 6;
    for(int i = 1; i <= this->endIndex; i++){
        int n = i;
        while(n%2 == 0) n/=2;
        if(n == 1){
            cout << "\n";
            for(int j = r; j > 0; j--){
                cout << "       ";
            }
            r--;
        }

        cout << this->data[i].key << "(" <<this->data[i].id << ") ";
    }
    
    cout << "\nlast index: " << this->endIndex << "\n"; 
    return 0;
}
