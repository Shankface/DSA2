#include <iostream>
#include <string>
#include "./heap.h"
#include "./graph.h"
#include <string>
#include <chrono>
#include <cstring>  
#include <sstream>
#include <vector>
#include <fstream>

using namespace std;

int main(){

    string graphFilePath, outFilePath, startVertex;
    graph g;

    cout << "Enter name of graph file: "; 
    cin >> graphFilePath;

    ifstream graphFile(graphFilePath);
    
    if(graphFile.is_open()){
        string line; 
        
        while(getline(graphFile,line)){
            stringstream ss(line); 
            string start,end;
            int weight; 
            ss >> start >> end >> weight; 
            g.insert(start, end, weight);
        }
    }
    else {
        cout << "ERROR opening file: " << graphFilePath << endl;
        exit(-1);
    }

    cout << "Enter name of starting vertex: ";
    cin >> startVertex;
    while(!g.vertexExists(startVertex)){
        cout << "Invalid starting vertex\nEnter name of starting vertex: ";
        cin >> startVertex;
    }

    g.Dijkstra(startVertex);

    cout << "Enter name of output file: ";
    cin >> outFilePath;
    
    g.printDijkstra(outFilePath);

    return 0;
}