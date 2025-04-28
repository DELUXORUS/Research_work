#ifndef BRANCH_AND_BOUND_H_
#define BRANCH_AND_BOUND_H_

#include <vector>
#include <limits>
#include <numeric>
#include <algorithm>
#include <iomanip>

#include "BaseWeightAlgorithm.h"


class BranchAndBound : public WeightAlgorithm {
    public:
        BranchAndBound() {}
        // BranchAndBound(WeightGraph weightGraph) : WeightAlgorithm(weightGraph) {}
        void initialize(std::map<int, std::vector<Vertex>> listAdjacency,
                        std::vector<std::vector<int>> matrixAdjacency,
                        std::vector<Vertex> numberVertex);
        void search();
        std::map<int, std::vector<Vertex>>& getWeightListAdjacency() { return _weightListAdjacency; }
    private:
        struct ZeroValuation {
            int row;
            int col;
            int grade;
        };

        enum FlagRowCol {ROW = 0, COL = 1};

        int _accumulateGrades(std::vector<int>& minElements);
        void _outputVector(std::vector<int>& vector);
        void _addInListAdjacency(std::map<int, std::vector<Vertex>>& weightListAdjacency, int numberVertex1, int numberVertex2);
        void _output();
        void _outputMatrix(std::vector<std::vector<int>>& weightMatrixAdjacency);
        // void _sortEdges();
        void _recursivelyBrandAndBound(std::vector<std::vector<int>>& weightMatrixAdjacency, 
                                       int currentLowBound,
                                       std::map<int, std::vector<Vertex>>& weightListAdjacency,
                                       std::vector<int>& rowIndexDecreasingMatrix, 
                                       std::vector<int>& colIndexDecreasingMatrix);
        void _reductionMatrix(std::vector<std::vector<int>>& weightMatrixAdjacency, 
                              int row, int col);
        std::vector<int> _searchMinInRowOrCol(FlagRowCol rowOrCol, 
                                              std::vector<std::vector<int>>& weightMatrixAdjacency);            // row = 0, col = 1; 
        void _matrixReductionRowOrCol(FlagRowCol rowOrCol, std::vector<int>& minElements, 
                                      std::vector<std::vector<int>>& weightMatrixAdjacency);                    // row = 0, col = 1; 
        ZeroValuation _searchMaxGradeZero(std::vector<std::vector<int>>& weightMatrixAdjacency,
                                          std::vector<int>& rowIndexDecreasingMatrix, 
                                          std::vector<int>& colIndexDecreasingMatrix);
        unsigned _lowerBound;
        std::map<int, std::vector<Vertex>> _weightListAdjacency;
        
};

#endif
