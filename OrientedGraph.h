#ifndef ORIENTED_GRAPH_H_
#define ORIENTED_GRAPH_H_

#include <map>
#include <vector>
#include <algorithm>
#include <limits>
#include <iomanip>

#include "WeightGraph.h"
#include "Vertex.h"

class OrientedGraph {
    public:
        OrientedGraph() {}
        void fillMatrixAdjacency(int initialVertex, int finalVertex, int weight);
        void fillListAdjacency();
        std::map<int, std::vector<Vertex>>& getListAdjacency() { return _listAdjacency; }
        std::vector<std::vector<int>>& getMatrixAdjacency() { return _matrixAdjacency; }
        std::vector<Vertex>& getVectorVertex() { return _numberVertex; }
        int getNumberVertex() { return _numberVertex.size(); }
        void outputMatrixAdjacency();
        void outputListAdjacency();
        int checkEdge(int numberVertex1, int numberVertex2);
        bool isStronglyConnected();
        void testGraph();
        void reset();
    private:
        std::vector<Vertex> _numberVertex;
        std::vector<std::vector<int>> _matrixAdjacency;
        std::map<int, std::vector<Vertex>> _listAdjacency;
};

#endif