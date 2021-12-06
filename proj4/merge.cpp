#include <iostream>
#include <string>
#include <chrono>
#include <cstring>  
#include <sstream>
#include <vector>
#include <fstream>
#define NOTMERGE "*** NOT A MERGE ***"

using namespace std;

bool indexes[1001][1001];                                                                                                           // matrix that stores whether a path beginning at a pair of indexes of the two strings is a dead end or not 


string checkMerge(const string& s1, const string& s2, const string& merged, int index1 = 0, int index2 = 0, int mergedIndex = 0){

    if(merged.length() != (s1.length() + s2.length())) return NOTMERGE;                                                             // if the two string lengths combined don't equal the merged word length, it's not a valid merge

    string result;                                                                                                                  // final string that has letters from first string capatilized or specifies if it's not a merge

    while(mergedIndex < merged.length()){

        // current pair of indexes is marked as dead end
        if(!indexes[index1][index2]) 
            return NOTMERGE;

                                                                                                                                     
        else if((merged[mergedIndex] == s1[index1]) && (merged[mergedIndex] == s2[index2])){                                        // current letter of merged word matches both current indexes of the 2 strings

            mergedIndex++;
            string temp;

            if((temp = checkMerge(s1, s2, merged, index1+1, index2, mergedIndex)) != NOTMERGE)                                      // checks if path taken if the first word is chosen ends in a dead end
                return result + (char)toupper(s1[index1]) + temp;                                                                   // add the letter and the letter before it to the final resulting string
            
            indexes[index1+1][index2] = false;                                                                                      // if you checkMerge and it is a dead end then update indexes matrix      

            temp = checkMerge(s1,s2,merged, index1,index2+1, mergedIndex);                                                          // if it isn't marked as dead end, check what happens when you choose second string
            indexes[index1][index2+1] = (temp != NOTMERGE);                                                                         // update indexes matrix  
            return indexes[index1][index2+1] ? result + s2[index2] + temp : NOTMERGE;                                               // add the letter and the letter before it to the final resulting string or NOTMERGE if it's a dead end
            
        }

        else if((merged[mergedIndex] == s1[index1])){                                                                               // letter at current index first string matches letter at current index of merged word 
            result += (char)toupper(s1[index1]);
            index1++;
            mergedIndex++;
        }

        else if(merged[mergedIndex] == s2[index2]){                                                                                 // letter at current index second string matches letter at current index of merged word 
            result += s2[index2];
            index2++;
            mergedIndex++;
        }
        
        else {          // if neither of the letters at current indexes of either string match the current merged word letter than it is a dead end
            indexes[index1][index2] = false;
            return NOTMERGE;
        }
    }
    return result; 
}


int main(){

    string line, string1, string2, merged, inpFilePath, outFilePath;

    cout << "Enter name of input file: "; 
    cin >> inpFilePath;
    cout << "Enter name of output file: "; 
    cin >> outFilePath;

    ifstream inpFile(inpFilePath);
    ofstream outFile(outFilePath); 

    if(inpFile.is_open()){
        while(inpFile >> string1 && inpFile >> string2 && inpFile >> merged){
            memset(indexes,true,sizeof(indexes)); 
            outFile << checkMerge(string1, string2, merged) << "\n";
        }
        inpFile.close();
        outFile.close();
    }

    else {
        cout << "ERROR opening file: " << inpFilePath << endl;
        exit(-1);
    }

    return 0;
}