#include "OrientedGraph.h"


void OrientedGraph::testGraph() {
    if(_matrixAdjacency.size() == 0) {
        int inf = std::numeric_limits<int>::max();
        _matrixAdjacency = {{inf, 5, 3, 2, 4},
                            {4, inf, 2, 7, 3},
                            {1, 3, inf, 2, 2},
                            {4, 1, 5, inf, 3},
                            {5, 1, 1, 1, inf}};

        Vertex vertex1 = {1, 0, 100, 300};
        Vertex vertex2 = {2, 0, 300, 200};
        Vertex vertex3 = {3, 0, 300, 400};
        Vertex vertex4 = {4, 0, 500, 200};
        Vertex vertex5 = {5, 0, 500, 300};

        _numberVertex.push_back(vertex1);
        _numberVertex.push_back(vertex2);
        _numberVertex.push_back(vertex3);
        _numberVertex.push_back(vertex4);
        _numberVertex.push_back(vertex5);

        fillListAdjacency();
    }
    else {
        cout << endl << "The graph is alredy set!" << endl;
    }
}

bool OrientedGraph::isStronglyConnected() {
    for (size_t i = 1; i < _listAdjacency.size(); ++i) {
        cout << _listAdjacency[i].size() << "   " << _numberVertex.size() << endl;

        if (_listAdjacency[i].size() != _numberVertex.size() - 1) {
            return 0;
        }
    }

    return 1;
}


void OrientedGraph::reset() {
    _numberVertex.clear();
    _matrixAdjacency.clear();
    _listAdjacency.clear();
}

int OrientedGraph::checkEdge(int numberVertex1, int numberVertex2) {
    
    numberVertex1--;
    numberVertex2--;

    if(numberVertex1 == numberVertex2)
        return 2;
    
    if(numberVertex1 >= _matrixAdjacency.size() || numberVertex2 >= _matrixAdjacency.size())
        return 0;
    
    if(_matrixAdjacency[numberVertex1][numberVertex2] != std::numeric_limits<int>::max())
        return 1;

    return 0;
}

void OrientedGraph::fillMatrixAdjacency(int initialVertex, int finalVertex, int weight) { 
    _matrixAdjacency.resize(_numberVertex.size());

    for (size_t i = 0; i < _matrixAdjacency.size(); ++i) {
        _matrixAdjacency[i].resize(_numberVertex.size(), std::numeric_limits<int>::max());
    }

    _matrixAdjacency[initialVertex - 1][finalVertex - 1] = weight;
}

void OrientedGraph::fillListAdjacency() {
    for(int i = 0; i < _listAdjacency.size(); i++) {
        _listAdjacency[i + 1].clear();
    }

    for(int i = 0; i < _matrixAdjacency.size(); i++) {
        for(int j = 0; j < _matrixAdjacency.size(); j++) {
            if(_matrixAdjacency[i][j] != std::numeric_limits<int>::max()) {
                _listAdjacency[i + 1].push_back({j + 1, _matrixAdjacency[i][j], _numberVertex[j].getX(), _numberVertex[j].getY()});
            }
        }
    }
}

void OrientedGraph::outputMatrixAdjacency() {
    cout << endl << "Matrix Adjacency Oriented Graph" << endl;

    for (size_t i = 0; i < _matrixAdjacency.size(); i++) {
        for (size_t j = 0; j < _matrixAdjacency.size(); j++) {
                cout << std::setw(5);

                if (_matrixAdjacency[i][j] == std::numeric_limits<int>::max()) {
                    cout << "inf";
                }
                else {
                    cout << _matrixAdjacency[i][j]; 
                }

                cout << "   ";
        }
        
        cout << endl;
    }
}

void OrientedGraph::outputListAdjacency() {
    cout << endl << "List Adjacency Oriented Graph" << endl;

    for(int i = 1; i <= _listAdjacency.size(); i++) {
        printf("Vertex %d:\n", i);
        for(int j = 0; j < _listAdjacency[i].size(); j++) {
            cout << "   vertex: " 
            << _listAdjacency[i][j].getNumber()
            << ", size: " << _listAdjacency[i][j].getWeight()
            << ", coord: " << _listAdjacency[i][j].getX() << ";" << _listAdjacency[i][j].getY() << endl;      
        }

        cout << endl;
    } 
}