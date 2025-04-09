#ifndef GREEDY_ALGORITHM_H_
#define GREEDY_ALGORITHM_H_

#include <vector>
#include <algorithm>
#include <limits>
#include <iterator>

#include "BaseWeightAlgorithm.h"
#include "VertexWeight.h"


class GreedyAlgorithm : public WeightAlgorithm {
public:
    GreedyAlgorithm() {}
    GreedyAlgorithm(WeightGraph weightGraph) : WeightAlgorithm(weightGraph) {}
    void search();

private:
    bool _recursiveSearch(std::vector<int>& path1, std::vector<int>& path2);
    std::pair<int, int> _searchMinimalEdgeForVertex(const std::vector<int>& adjacencyVertex,
                                                    const std::vector<int>& path1, 
                                                    const std::vector<int>& path2);
    void _mergePaths(std::vector<int>& path1, std::vector<int>& path2);
    bool _checkAdjacency(int vertex1, int vertex2);
    // void _outputPaths(const std::vector<int>& path1, const std::vector<int>& path2);
    void _output();
    std::vector<int> _hamiltonianCycle;
};

#endif