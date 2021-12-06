#include "./graph.h"
#include "./heap.h"
#include "./hash.h"
#include <string>
#include <tuple>
#include <list>
#include <iostream>
#include "limits.h"
#include <chrono>
#include <iostream>
#include <fstream>

using namespace std;

graph::vertex::vertex(const string& name){
    this->name = name; 
}

// Function to insert edge between 2 vertexes
int graph::insert(const string& startV, const string& endV, int weight){
    bool startV_exist, endV_exist;
    vertex* startVertex = (vertex*)this->map.getPointer(startV,&startV_exist); 
    vertex* endVertex = (vertex*)this->map.getPointer(endV,&endV_exist);

    // If start and/or end vertex don't exist then add them
    if(!startV_exist){
        this->vertices.push_back(vertex(startV));
        this->map.insert(startV, &this->vertices.back());
        startVertex = &(this->vertices.back());
    }

    if(!endV_exist){
        this->vertices.push_back(vertex(endV));
        this->map.insert(endV, &this->vertices.back());
        endVertex = &(this->vertices.back());
    }

    tuple<vertex*,int> edge = {endVertex, weight};
    startVertex->edges.push_back(edge);
    return 0;
}

// Function to check if given vertex exists in graph
int graph::vertexExists(const std::string& vertex){
    bool vertExists;
    this->map.getPointer(vertex,&vertExists);
    return vertExists;
}

// Function to perform Dijkstra's Algorithm on the graph starting at a given node
int graph::Dijkstra(const std::string& startVertex){

    auto start = chrono::_V2::steady_clock::now();
    heap h(this->vertices.size());

    for(auto& vert : this->vertices){ //adds all vertexes to binary heap with their dmin as key (1000000000 as default)
        if(vert.name == startVertex) vert.dmin = 0; //sets source node dmin to zero 
        h.insert(vert.name, vert.dmin,&vert);
    }

    int dv;
    string v_name;
    vertex* v;
    string w_name;
    int dw;
    int Cvw;
    bool found;

    while(h.deleteMin(&v_name, &dv, &v) == 0){ //pops vertex with smallest dmin from heap and stores its name, dmin, and data to pointers above

        for(auto& edge : v->edges){ // goes through the popped vertex's adjacent nodes
            w_name = get<0>(edge)->name;
            
            dw = h.getKey(w_name,&found); // stores adjacent node's dmin
            if(!found) continue; //if adjacent node has been popped already, continue to next adjacent node
            Cvw = get<1>(edge); // stores cost to adjacent node

            if((dv + Cvw) < dw){
                h.setKey(w_name, dv + Cvw);
                get<0>(edge)->dmin = dv + Cvw;
                get<0>(edge)->preV = v;
            }
        }
    }

    auto end = chrono::_V2::steady_clock::now();
    chrono::duration<double> duration = end-start; 
    
    cout << "Total CPU time while applying Dijkstra's algorithm: " << duration.count() << " seconds\n";
    return 0;
}

// Function to print paths from source node to each node after performing Dijkstra's Algorithm
void graph::printDijkstra(const std::string &outFilePath){

    ofstream outFile(outFilePath);

    vector<string> path;
    vertex* currentVert;

    for(auto &vert : this->vertices){
        currentVert = &vert;
        path.clear();

        while(true){ //follows path of previous nodes
            path.insert(path.begin(), currentVert->name); 
            currentVert = currentVert->preV;
            if(currentVert == nullptr) break; //at source node
        }

        outFile << vert.name << ": ";
        if(vert.dmin == 1000000000){ //impossible to get to this node from source node
            outFile << "NO PATH\n";
        }
        else{
            outFile << vert.dmin << " [";
            for(auto v : path){
                outFile << v;
                if(v != path.back()) outFile << ", ";
            }
            outFile << "]\n";
        }
    }
}

// Function to print each vertex and its adjacent nodes and costs in the graph
void graph::printGraph(){

    for(auto& vert : this->vertices){

        cout << vert.name << ": ";
        for(auto& ed : vert.edges){
            cout << "[" << get<0>(ed)->name << "," << get<1>(ed) << "] ";
        }
        cout << "\n";
    }
}