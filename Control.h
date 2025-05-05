#ifndef GRAPH_CONSTRUCTION
#define CONTROL

#include <iostream>
#include "WeightGraph.h"
#include "OrientedGraph.h"
#include "BranchAndBound.h"
#include "Graphic.h"
#include "Vertex.h"


using std::cout;
using std::endl;
using std::cin;


class Control {
    public:
        Control() : _lockButton1(0), _lockButton3(0), _countVertex(1) {}
        void dispatch(Graphic& graphic, WeightGraph& weightGraph, 
                      OrientedGraph& orientedGraph);
    private:
        int _keyAction(XEvent* event, Graphic& graphic, 
                    WeightGraph& weightGraph, OrientedGraph& orientedGraph);
        void _constructionWeightGraph(Graphic& graphic, WeightGraph& weightGraph, Vertex currentVertex);
        void _constructionOrientedGraph(Graphic& graphic, OrientedGraph& orientedGraph, Vertex currentVertex);
        void _addVertex(Graphic& graphic, Vertex currentVertex);
        std::vector<Vertex> _selectVertexForWeight;
        std::vector<Vertex> _selectVertexForOriented;
        std::map<int, std::vector<Vertex>> _currentGraph;
        std::vector<Vertex> _vertex;
        int _countVertex = 1;
        bool _lockButton1 = 0;
        bool _lockButton3 = 0;
};

#endif