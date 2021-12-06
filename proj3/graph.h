#ifndef _GRAPH_H
#define _GRAPH_H
#include <list>
#include "./hash.h"
#include <string>
#include <tuple>
#include "limits.h"

class graph{
    public:
        graph() = default;
        int insert(const std::string& startVertex, const std::string& endVertex, int weight);
        
        int vertexExists(const std::string& vertex);

        void printGraph();
        
        int Dijkstra(const std::string& startVertex);
        
        void printDijkstra(const std::string& outFilePath);
        

    private:
        class vertex{
            public:

                vertex() = default; 
                vertex(const std::string& name);
                std::string name;
                std::list<std::tuple<vertex*,int>> edges;
                vertex* preV {nullptr};
                int dmin {1000000000}; 
        };

        std::list<vertex> vertices; 
        hashTable map; 
};

#endif