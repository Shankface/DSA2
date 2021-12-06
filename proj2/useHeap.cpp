#include <iostream>
#include <string>
#include <cstdlib>
#include <random> 
#include <set>
#include <functional>
#include "heap.h"
#include "vector"
#include <bits/stdc++.h>

#define FORCE test.force
#define FI test.force<int>
#define FS test.force<string>

using namespace std;

class utester {
  public:
    utester() = default;
    template<class T> 
    void force(T input , T expectation, string message = "Invalid response"  ){
      if (input != expectation){
        cerr << "Failure: " << message << "\ncounter: " << counter<< " expected: " << expectation << " revieved: " << input << "\n"; 
        exit(-1); 
      }
      //else cout << "Pass " << counter << "\n";
      counter++; 
    }
  private:
    unsigned int counter = 1; 
};



void baseTest(){
  heap h(500); 
  utester test; 
  int counter = 0; 
  FI(h.insert("string one",35),0, "Insert return failure 1");
  FI(h.insert("string two",55),0, "Insert return failure 2");
  FI(h.insert("string three",45),0, "Insert return failure 3");
  FI(h.insert("string four",5),0, "Insert return failure 4");
  FI(h.insert("string two",10),2, "Insert return unsuccessful");
  FI(h.insert("string five",80),0, "Insert return failure 5");
  FI(h.insert("string six",20),0, "Insert return failure 6");
  FI(h.insert("string seven",70),0, "Insert return failure 7");
  int key;
  string id; 
  FI(h.deleteMin(&id,&key),0);
  FI(key,5, "Incorrect Key"); 
  FORCE<string>(id,"string four","Incorrect id"); 
  FI(h.remove("string four",&key),1);
  FI(h.deleteMin(&id,&key),0);
  FI(key,20,"Incorrect Key"); 
  FORCE<string>(id,"string six", "Incorrect id");
  FI(h.setKey("string one", 50),0);
  FI(h.remove("string two",&key),0);
  FI(key,55); // fail
  FI(h.deleteMin(&id,&key),0);
  FI(key,45,"Incorrect Key"); 
  FORCE<string>(id,"string three","Incorrect id");
  FI(h.deleteMin(&id,&key),0);
  FI(key,50,"Incorrect Key"); 
  FORCE<string>(id,"string one","Incorrect ID"); 
  FI(h.setKey("string five",60),0);
  FI(h.deleteMin(&id,&key),0);
  FI(key,60);
  FORCE<string>(id,"string five"); 
  FI(h.deleteMin(&id,&key),0);
  FI(key,70);
  FORCE<string>(id,"string seven"); 
  FI(h.deleteMin(&id,&key),1);

  cout << "passed first test\n"; 
}

void insertionSort(vector<int> &arr, int n)
{
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        j = i - 1;
 
        /* Move elements of arr[0..i-1], that are
        greater than key, to one position ahead
        of their current position */
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void testOnlyInsertAndDelteMin(int iterations){
  std::random_device rd;     
  std::mt19937 rng(rd());    
  std::uniform_int_distribution<int> uni(INT_MIN,INT_MAX); 
  utester test; 
  heap h(iterations); 
  vector<int> sorted(iterations); 
  for(int i = 0; i<iterations; i++){
    int random_integer = uni(rng);
    FI(h.insert("string " + to_string(i), random_integer),0);
  }
  for(int i = 0; i<iterations;i++){
    int key; 
    FI(h.deleteMin(nullptr,&key),0);
    sorted[i] = key; 
  }
  vector<int> toBeSorted = sorted; 
  insertionSort(toBeSorted,iterations);
  for(int i = 0; i<iterations;i++){
    FI(sorted[i],toBeSorted[i]); 
  }

}



int main(){
  /*std::random_device rd;     
  std::mt19937 rng(rd());    
  std::uniform_int_distribution<int> uni(0,10); 
*/
  heap h(10); 
  int key;
  string id;
  vector<int> sorted(20);

  
  // h.insert("string one", 3);
  // h.printHeap();
  // h.insert("string two", 0);
  //   h.printHeap();

  // h.insert("string three", -1);
  //   h.printHeap();

  // h.insert("string four", 2);
  //   h.printHeap();

  // h.deleteMin(nullptr,&key);
  //   h.printHeap();

  // h.deleteMin(nullptr,&key);
  //   h.printHeap();

  // h.deleteMin(nullptr,&key);
  //   h.printHeap();

  // h.deleteMin(nullptr,&key);
  //   h.printHeap();




  
  baseTest(); 
  testOnlyInsertAndDelteMin(250000);
  cout << "Passed all tests\n"; 
}
