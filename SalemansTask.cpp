#include "SalemansTask.h"


void SalemansTask::search() {
    std::vector<int> path1;
    std::vector<int> path2;

    for (int i = 0; i < _weightGraph.getNumberVertex(); ++i) {
        int numberVertex = i + 1;
        Vertex minSizeEdge = {0, std::numeric_limits<int>::max(), 0, 0};
        Vertex prevMinSizeEdge = {0, std::numeric_limits<int>::max(), 0, 0};
        
        path1 = {numberVertex};
        path2 = {numberVertex};
        
        for (auto& vertex : _weightGraph.getWeightListAdjacency()[numberVertex]) {
            if (vertex.getWeight() < minSizeEdge.getWeight()) {
                prevMinSizeEdge = minSizeEdge;
                minSizeEdge = vertex;
                
            } 
            else if (vertex.getWeight() <= prevMinSizeEdge.getWeight() && 
                       vertex.getNumber() != minSizeEdge.getNumber()) {
                prevMinSizeEdge = vertex;
            }
        }
        
        path1.push_back(minSizeEdge.getNumber());
        path2.push_back(prevMinSizeEdge.getNumber());
        
        
        if (_recursiveSearch(path1, path2) == true) {
            _mergePaths(path1, path2);
            _output();
            
        }
        else {
            cout << endl << "it is impossible to bypass all vertices once with minimum weight from " 
            << numberVertex << endl;
        }

        path1.clear();
        path2.clear();
        _hamiltonianCycle.clear();
    }
}



bool SalemansTask::_recursiveSearch(std::vector<int>& path1, std::vector<int>& path2) {

    int currentVertex1 = path1.back() - 1;
    auto minEdge1 = _searchMinimalEdgeForVertex(_weightGraph.getWeightMatrixAdjacency()[currentVertex1], 
                                                path1, 
                                                path2);
    
    int currentVertex2 = path2.back() - 1;
    auto minEdge2 = _searchMinimalEdgeForVertex(_weightGraph.getWeightMatrixAdjacency()[currentVertex2],
                                                path1, 
                                                path2);

    if (minEdge2.first != 0 || minEdge1.first != 0) {
        if (minEdge1.second <= minEdge2.second) {
            path1.push_back(minEdge1.first);
        } else {
            path2.push_back(minEdge2.first);
        }

        _recursiveSearch(path1, path2);
    }

    return (_checkAdjacency(path1.back() - 1, path2.back() - 1));
}

bool SalemansTask::_checkAdjacency(int vertex1, int vertex2) {
    return (_weightGraph.getWeightMatrixAdjacency()[vertex1][vertex2]);
}

std::pair<int, int> SalemansTask::_searchMinimalEdgeForVertex(const std::vector<int>& adjacencyVertex,
                                                              const std::vector<int>& path1, 
                                                              const std::vector<int>& path2) 
{
    int minSizeEdge = std::numeric_limits<int>::max();
    std::pair<int, int> numberWeight{0, minSizeEdge};    

    for (int i = 0; i < adjacencyVertex.size(); ++i) {
        int vertex = i + 1; 
        
        if (adjacencyVertex[i] != 0) {
            if (find(path1.begin(), path1.end(), vertex) == path1.end() &&
                find(path2.begin(), path2.end(), vertex) == path2.end()) 
            {
                if (adjacencyVertex[i] < numberWeight.second) {
                    numberWeight.second = adjacencyVertex[i];
                    numberWeight.first = vertex;
                }
            }
        }
    }

    return numberWeight;
}

void SalemansTask::_mergePaths(std::vector<int>& path1, std::vector<int>& path2) {
    _hamiltonianCycle.insert(_hamiltonianCycle.begin(), path1.begin(), path1.end());
    _hamiltonianCycle.insert(_hamiltonianCycle.end(), path2.rbegin(), path2.rend());
}

void SalemansTask::_output() {
    cout << endl << "Salemans Task" << endl;
    
    for (auto it = _hamiltonianCycle.begin(); it != _hamiltonianCycle.end();) {
        cout << *it; 
        
        if (++it != _hamiltonianCycle.end())
            cout << " => ";
    }
}