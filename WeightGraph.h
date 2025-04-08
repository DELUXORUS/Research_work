#ifndef WEIGHTED_GRAPH_H
#define WEIGHTED_GRAPH_H

#include <random>
#include <iomanip>

#include "Graph.h"
#include "Vertex.h"
#include "VertexWeight.h"


class WeightGraph {
    public: 
        void initializeWeightGraph(std::vector<Vertex> numberVertex);
        WeightGraph() {}
        void fillMatrixWeight(int vertex1, int vertex2, int weight);
        void fillListWeight();
        std::map<int, std::vector<Vertex>>& getWeightListAdjacency() { return _weightListAdjacency; }
        std::vector<std::vector<int>>& getWeightMatrixAdjacency() { return _weightMatrixAdjacency; }
        std::vector<Vertex>& getVectorVertex() { return _numberVertex; }
        int getNumberVertex() { return _numberVertex.size(); }
        void outputMatrixWeightGraph();
        void outputListWeightGraph();
        bool checkEdge(int numberVertex1, int numberVertex2);
        void reset();
    private:
        std::vector<Vertex> _numberVertex;
        std::map<int, std::vector<Vertex>> _weightListAdjacency;
        std::vector<std::vector<int>> _weightMatrixAdjacency;
};

#endif