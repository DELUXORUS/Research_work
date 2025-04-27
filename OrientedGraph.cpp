#include "OrientedGraph.h"

void OrientedGraph::testGraph() {
    if(_matrixAdjacency.size() == 0) {
        int inf = std::numeric_limits<int>::max();
        _matrixAdjacency = {{inf, 1, 9, inf, 4, inf, inf, inf, inf, inf},
                            {inf, inf, 4, inf, inf, inf, inf, inf, inf, inf},
                            {inf, inf, inf, inf, 2, 3, inf, inf, inf, inf},
                            {inf, 1, 2, inf, 1, inf, inf, inf, inf, inf},
                            {inf, inf, inf, inf, inf, 2, 7, inf, 3, inf},
                            {inf, inf, inf, inf, inf, inf, inf, inf, 1, inf},
                            {inf, inf, inf, 5, inf, inf, inf, 4, inf, 4},
                            {inf, inf, inf, inf, 3, inf, inf, inf, 1, 4},
                            {inf, inf, inf, inf, inf, inf, inf, inf, inf, 2},
                            {inf, inf, inf, inf, inf, inf, inf, inf, inf, inf}};

        Vertex vertex1 = {1, 0, 100, 300};
        Vertex vertex2 = {2, 0, 200, 200};
        Vertex vertex3 = {3, 0, 200, 400};
        Vertex vertex4 = {4, 0, 300, 200};
        Vertex vertex5 = {5, 0, 300, 300};
        Vertex vertex6 = {6, 0, 300, 400};
        Vertex vertex7 = {7, 0, 400, 200};
        Vertex vertex8 = {8, 0, 400, 300};
        Vertex vertex9 = {9, 0, 400, 400};
        Vertex vertex10 = {10, 0, 500, 300};

        _numberVertex.push_back(vertex1);
        _numberVertex.push_back(vertex2);
        _numberVertex.push_back(vertex3);
        _numberVertex.push_back(vertex4);
        _numberVertex.push_back(vertex5);
        _numberVertex.push_back(vertex6);
        _numberVertex.push_back(vertex7);
        _numberVertex.push_back(vertex8);
        _numberVertex.push_back(vertex9);
        _numberVertex.push_back(vertex10);

        fillListAdjacency();
    }
    else {
        cout << endl << "The graph is alredy set!" << endl;
    }
}

void OrientedGraph::fillMatrixAdjacency(int initialVertex, int finalVertex, int weight) { 
    // _matrixAdjacency.resize(_numberVertex.size(), 
    //                         std::vector<int>(_numberVertex.size(), std::numeric_limits<int>::max()));

    _matrixAdjacency.resize(_numberVertex.size());

    for (size_t i = 0; i < _matrixAdjacency.size(); ++i) {
        _matrixAdjacency[i].resize(_numberVertex.size(), std::numeric_limits<int>::max());
    }

    _matrixAdjacency[initialVertex - 1][finalVertex - 1] = weight;
}

void OrientedGraph::fillListAdjacency() {
    for(int i = 0; i < _listAdjacency.size(); i++)
        _listAdjacency[i + 1].clear();

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