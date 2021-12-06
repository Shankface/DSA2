#include <iostream>
#include <string> 
#include <fstream>
#include <regex> 
#include "hash.h"
#include <chrono>

using namespace std;

// Makes words lowercase
string toLower(string word){
    for(int i = 0; i < word.length(); i++){
        word[i] = tolower(word[i]);
    }
    return word;
}

// Fills dictionary
hashTable fillDict(){
   
    hashTable table(0);
    
    cout << "Enter name of dictionary: ";
    string dictPath;
    cin >> dictPath;

    auto start = chrono::steady_clock::now(); //start clock

    ifstream dictFile(dictPath);

    if(dictFile.is_open()){
        string word;
        while(getline(dictFile, word)){
            table.insert(toLower(word));
        }
    }

    auto end = chrono::steady_clock::now(); //end clock
    chrono::duration<double> sec = end-start; 
    cout << "Total time (in seconds) to load dictionary: " << sec.count() << "s\n"; 
    return table;
}

// Check if word is valid or not
void wordCheck(string w, int lineNum, hashTable& dict, ofstream& outputDocument){
    string word = toLower(w);

    if(word.size() > 20){
        outputDocument << "Long word at line " << lineNum << ", starts: " << word.substr(0,20) << "\n";
        return;
    }

    else if(any_of(word.begin(), word.end(), ::isdigit) || dict.contains(word)) return; //ignore if word includes number or exists in dictionary

    else{
        outputDocument << "Unknown word at line " << lineNum << ": " << word << "\n";
        return;
    }
}


void seperate_and_check(hashTable& dict){ 
    string inpFilePath, outFilePath;
    cout << "Enter name of input file: "; 
    cin >> inpFilePath;
    cout << "Enter name of output file: "; 
    cin >> outFilePath;

    auto start = chrono::steady_clock::now(); //start clock

    ifstream inpFile(inpFilePath);
    ofstream outFile(outFilePath);

    if(inpFile.is_open()){
        int lineNum = 1;
        string line; 
        regex r("([a-zA-Z0-9'-]+)"); //regex that counts every symbol other than the desired symbols as word seperators

        while(getline(inpFile,line)){ //while not at EOF
            smatch match; 
            while(regex_search(line,match,r)){ //while there's still vaild words on the line 
                wordCheck(match[0], lineNum, dict, outFile);
                line = match.suffix(); //remove word from the line for next loop
            }
            lineNum++;
        }
    }

    auto end = chrono::steady_clock::now(); //end clock
    chrono::duration<double> sec = end-start; 
    cout << "Total time (in seconds) to check document: " << sec.count() << "s\n"; 
}


int main(){
    hashTable dict = fillDict();
    seperate_and_check(dict);
    return 0;
}

