#include "BranchAndBound.h"


void BranchAndBound::initialize(std::map<int, std::vector<Vertex>> listAdjacency,
    std::vector<std::vector<int>> matrixAdjacency,
    std::vector<Vertex> numberVertex)
{
    copy(listAdjacency.begin(), listAdjacency.end(), inserter(_listAdjacencyForCurrentGraph, _listAdjacencyForCurrentGraph.end()));
    // copy(matrixAdjacency.begin(), matrixAdjacency.end(), inserter(_matrixAdjacencyForCurrentGraph, matrixAdjacency.end()));
    // copy(numberVertex.begin(), numberVertex.end(), inserter(_numberVertexForCurrentGraph, _numberVertexForCurrentGraph.end()));
    // _listAdjacencyForCurrentGraph = listAdjacency;
    _matrixAdjacencyForCurrentGraph = matrixAdjacency;
    _numberVertexForCurrentGraph = numberVertex;
}

void BranchAndBound::search() {
    _lowerBound = std::numeric_limits<unsigned>::max();
    // std::vector<std::vector<int>> weightMatrixAdjacency = _weightGraph.getWeightMatrixAdjacency();
    std::vector<std::vector<int>> weightMatrixAdjacency = _matrixAdjacencyForCurrentGraph;


    cout << endl << "Initial matrix" << endl;
    _outputMatrix(weightMatrixAdjacency);

    std::vector<int> rowIndexDecreasingMatrix, colIndexDecreasingMatrix;

    for (size_t i = 0; i < weightMatrixAdjacency.size(); ++i) {
        rowIndexDecreasingMatrix.push_back(i);
        colIndexDecreasingMatrix.push_back(i);
    }

    std::vector<int> minElements = _searchMinInRowOrCol(ROW, weightMatrixAdjacency);
    int currentLowBound = accumulate(minElements.begin(), minElements.end(), 0);
    _matrixReductionRowOrCol(ROW, minElements, weightMatrixAdjacency);
    cout << endl << "Matrix after reduction row" << endl;
    _outputMatrix(weightMatrixAdjacency);

    minElements = _searchMinInRowOrCol(COL, weightMatrixAdjacency);
    currentLowBound += accumulate(minElements.begin(), minElements.end(), 0);
    _matrixReductionRowOrCol(COL, minElements, weightMatrixAdjacency);
    cout << endl << "Matrix after reduction col" << endl;
    _outputMatrix(weightMatrixAdjacency);

    // ZeroValuation zeroOnDelete;
    // zeroOnDelete = _searchMaxGradeZero(weightMatrixAdjacency, rowIndexDecreasingMatrix, colIndexDecreasingMatrix);
    // cout << endl << "Row: " << zeroOnDelete.row << ", Col: " << zeroOnDelete.col << endl;
    // _reductionMatrix(weightMatrixAdjacency, zeroOnDelete.row, zeroOnDelete.col);
    // _outputMatrix(weightMatrixAdjacency);

    std::map<int, std::vector<Vertex>> weightListAdjacency;
    _recursivelyBrandAndBound(weightMatrixAdjacency, currentLowBound, weightListAdjacency, rowIndexDecreasingMatrix, colIndexDecreasingMatrix);
    copy(weightListAdjacency.begin(), weightListAdjacency.end(), inserter(_weightListAdjacency, _weightListAdjacency.end()));

    if (_lowerBound != std::numeric_limits<unsigned>::max()) {
        cout << endl << endl << "Hamiltonian cycle of smallest length is found!" << endl;
        cout << "Total weight: " << _lowerBound << endl;
        _output();
    }
}

void BranchAndBound::_recursivelyBrandAndBound(std::vector<std::vector<int>>& weightMatrixAdjacency, 
                                               int currentLowBound, 
                                               std::map<int, std::vector<Vertex>>& weightListAdjacency,
                                               std::vector<int>& rowIndexDecreasingMatrix, 
                                               std::vector<int>& colIndexDecreasingMatrix) 
{
    if (_lowerBound <= currentLowBound) {
        return;
    }
    
    if (weightMatrixAdjacency.size() == 1) {
        _lowerBound = currentLowBound;
        _addInListAdjacency(weightListAdjacency, 
            rowIndexDecreasingMatrix[0] + 1, 
            colIndexDecreasingMatrix[0] + 1);
            return;
    }

    cout << endl << "-----------------------------------------------------------------------------------" << endl;
    static int step = 1;
    cout << endl << "Step " << step << endl;
    ++step;

    ZeroValuation zeroOnDelete;
    int conversionConst, negativeNode, positiveNode;
    int numberVertex1, numberVertex2;
    std::vector<int> minElements;
    std::vector<std::vector<int>> matrixWithoutDelete = weightMatrixAdjacency;
    std::vector<int> rowIndexWithoutDeleteMatrix = rowIndexDecreasingMatrix;
    std::vector<int> colIndexWithoutDeleteMatrix = colIndexDecreasingMatrix;
    
    zeroOnDelete = _searchMaxGradeZero(weightMatrixAdjacency, rowIndexDecreasingMatrix, colIndexDecreasingMatrix);

    // cout << endl << "Max grade zero: " << endl;
    // cout << "Row: " << zeroOnDelete.row << endl << "Col: " << zeroOnDelete.col << endl;

    if (zeroOnDelete.grade == std::numeric_limits<int>::max()) {
        negativeNode = std::numeric_limits<int>::max();
    }
    else {
        negativeNode = currentLowBound + zeroOnDelete.grade;
    }
    

    int trueRowIndex = rowIndexDecreasingMatrix[zeroOnDelete.row];
    int trueColIndex = colIndexDecreasingMatrix[zeroOnDelete.col];
    numberVertex1 = trueRowIndex + 1;
    numberVertex2 = trueColIndex + 1;
    _reductionMatrix(weightMatrixAdjacency, zeroOnDelete.row, zeroOnDelete.col);

    cout << endl << "Indexes before delete" << endl;
    cout << "Row indexes: ";
    _outputVector(rowIndexDecreasingMatrix);
    cout << endl;
    cout << "Col indexes: ";
    _outputVector(colIndexDecreasingMatrix);

    rowIndexDecreasingMatrix.erase(rowIndexDecreasingMatrix.begin() + zeroOnDelete.row);
    colIndexDecreasingMatrix.erase(colIndexDecreasingMatrix.begin() + zeroOnDelete.col);

    cout << endl << "Indexes after delete" << endl;
    cout << "Row indexes: ";
    _outputVector(rowIndexDecreasingMatrix);
    cout << endl;
    cout << "Col indexes: ";
    _outputVector(colIndexDecreasingMatrix);
    cout << endl;
    
    auto pos1 = std::find(rowIndexDecreasingMatrix.begin(), 
                            rowIndexDecreasingMatrix.end(), trueColIndex);
    auto pos2 = std::find(colIndexDecreasingMatrix.begin(), 
                            colIndexDecreasingMatrix.end(), trueRowIndex);

    if (pos1 != rowIndexDecreasingMatrix.end() && 
        pos2 != colIndexDecreasingMatrix.end()) 
    {
        int indexRowForInfinity = std::distance(rowIndexDecreasingMatrix.begin(), pos1);
        int indexColForInfinity = std::distance(colIndexDecreasingMatrix.begin(), pos2);

        // cout << endl << "Row for infinity: " << indexRowForInfinity << endl;
        // cout << "Col for infinity: " << indexColForInfinity << endl;
        weightMatrixAdjacency[indexRowForInfinity][indexColForInfinity] = std::numeric_limits<int>::max();
    }
    
    matrixWithoutDelete[zeroOnDelete.row][zeroOnDelete.col] = std::numeric_limits<int>::max();

    cout << endl << "Matrix without adding an edge to the path";
    _outputMatrix(matrixWithoutDelete);

    cout << endl << "Matrix after remove row and col";
    _outputMatrix(weightMatrixAdjacency);

    cout << endl << "Matrix row reduction" << endl;
    minElements = _searchMinInRowOrCol(ROW, weightMatrixAdjacency);
    _outputVector(minElements);
    cout << endl;
    int conversionConst1 = _accumulateGrades(minElements);
    _matrixReductionRowOrCol(ROW, minElements, weightMatrixAdjacency);

    
    cout << endl << "Matrix col reduction" << endl;
    minElements = _searchMinInRowOrCol(COL, weightMatrixAdjacency);
    _outputVector(minElements);
    cout << endl;
    int conversionConst2 = _accumulateGrades(minElements);
    _matrixReductionRowOrCol(COL, minElements, weightMatrixAdjacency);
    // cout << endl << "Matrix after reduction at row and col";
    // _outputMatrix(weightMatrixAdjacency);

    if (conversionConst1 == std::numeric_limits<int>::max() ||
        conversionConst2 == std::numeric_limits<int>::max()) 
    {
        cout << endl << "The sum of the smallest elements in the row and in the column is infinity. A Hamiltonian cycle cannot be found" << endl;
        return;
    }

    positiveNode = currentLowBound + conversionConst1 + conversionConst2;

    cout << endl << "Low bound with add edge: " << positiveNode;
    cout << endl << "Low bound without add edge: " << negativeNode << endl; 


    if (positiveNode <= negativeNode) {
        currentLowBound = positiveNode;
        _addInListAdjacency(weightListAdjacency, numberVertex1, numberVertex2);
        _recursivelyBrandAndBound(weightMatrixAdjacency, currentLowBound, 
                                  weightListAdjacency, rowIndexDecreasingMatrix, colIndexDecreasingMatrix);

        if (negativeNode < _lowerBound) {
            currentLowBound = negativeNode;
            _recursivelyBrandAndBound(matrixWithoutDelete, currentLowBound, 
                                      weightListAdjacency, rowIndexWithoutDeleteMatrix, colIndexWithoutDeleteMatrix);
        }
    }
    else {
        currentLowBound = negativeNode;
        _recursivelyBrandAndBound(matrixWithoutDelete, currentLowBound, 
                                  weightListAdjacency, rowIndexWithoutDeleteMatrix, colIndexWithoutDeleteMatrix);

        if (positiveNode < _lowerBound) {
            currentLowBound = positiveNode;
            _addInListAdjacency(weightListAdjacency, numberVertex1, numberVertex2);
            _recursivelyBrandAndBound(weightMatrixAdjacency, currentLowBound, 
                                      weightListAdjacency, rowIndexDecreasingMatrix, colIndexDecreasingMatrix);
        }
    }
}

int BranchAndBound::_accumulateGrades(std::vector<int>& minElements) {
    int amount = 0;

    for (size_t i = 0; i < minElements.size(); ++i) {
        if (minElements[i] == std::numeric_limits<int>::max()) {
            amount = std::numeric_limits<int>::max();
            return amount;
        }

        amount += minElements[i];
    }

    return amount;
}

void BranchAndBound::_addInListAdjacency(std::map<int, std::vector<Vertex>>& weightListAdjacency, int numberVertex1, int numberVertex2){
    // Vertex vertex1 = _weightGraph.getVectorVertex()[numberVertex1 - 1];
    // Vertex vertex2 = _weightGraph.getVectorVertex()[numberVertex2 - 1];
    Vertex vertex1 = _numberVertexForCurrentGraph[numberVertex1 - 1];
    Vertex vertex2 = _numberVertexForCurrentGraph[numberVertex2 - 1];

    // vertex1.setWeight(_weightGraph.getWeightMatrixAdjacency()[numberVertex1 - 1][numberVertex2 - 1]);
    vertex1.setWeight(_matrixAdjacencyForCurrentGraph[numberVertex1 - 1][numberVertex2 - 1]);

    vertex2.setWeight(vertex1.getWeight());

    weightListAdjacency[numberVertex1].push_back(vertex2);
    weightListAdjacency[numberVertex2].push_back(vertex1);
}

// void BranchAndBound::_sortEdges() {
//     std::vector<std::pair<int, int>> sortedHamiltonianCycle;
//     std::pair<int, int> firstEdge = _hamiltonianCycle[0];
//     int currentEndPath = firstEdge.second;
//     sortedHamiltonianCycle.push_back(firstEdge);

//     int i = 1;

//     while(i < _hamiltonianCycle.size()) {
//         std::pair<int, int> currentEdge = _hamiltonianCycle[i];

//         if (currentEdge.first == currentEndPath) {
//             sortedHamiltonianCycle.push_back(currentEdge);
//             currentEndPath = currentEdge.second;
//             i = 1;
//             continue;
//         }

//         ++i;
//     }

//     _hamiltonianCycle = sortedHamiltonianCycle;
// }

void BranchAndBound::_reductionMatrix(std::vector<std::vector<int>>& weightMatrixAdjacency,
                                      int row, int col)
{
    weightMatrixAdjacency.erase(weightMatrixAdjacency.begin() + row);

    for (size_t i = 0; i < weightMatrixAdjacency.size(); ++i) {
        weightMatrixAdjacency[i].erase(weightMatrixAdjacency[i].begin() + col);
    }
}

std::vector<int> BranchAndBound::_searchMinInRowOrCol(FlagRowCol rowOrCol, 
                                                      std::vector<std::vector<int>>& weightMatrixAdjacency) 
{
    std::vector<int> minElements;

    for (size_t i = 0; i < weightMatrixAdjacency.size(); ++i) {
        int min = std::numeric_limits<int>::max();

        for (size_t j = 0; j < weightMatrixAdjacency.size(); ++j) {
            if (rowOrCol == ROW) {
                if (weightMatrixAdjacency[i][j] < min && 
                    weightMatrixAdjacency[i][j] >= 0) 
                {
                    min = weightMatrixAdjacency[i][j]; 
                }
            }
            else if (rowOrCol == COL) {
                if (weightMatrixAdjacency[j][i] < min && 
                    weightMatrixAdjacency[j][i] >= 0) 
                {
                    min = weightMatrixAdjacency[j][i]; 
                }
            }
        }

        minElements.push_back(min);
    }
    
    return minElements;
}

void BranchAndBound::_matrixReductionRowOrCol(FlagRowCol rowOrCol, 
                                              std::vector<int>& minElements, 
                                              std::vector<std::vector<int>>& weightMatrixAdjacency)
{
    for (size_t i = 0; i < weightMatrixAdjacency.size(); ++i) {
        for (size_t j = 0; j < weightMatrixAdjacency.size(); ++j) {
            if (rowOrCol == ROW) {
                if (weightMatrixAdjacency[i][j] > 0 && 
                    weightMatrixAdjacency[i][j] != std::numeric_limits<int>::max()) 
                {
                    weightMatrixAdjacency[i][j] -= minElements[i];
                }
            }
            else if (rowOrCol == COL) {
                if (weightMatrixAdjacency[j][i] > 0 && 
                    weightMatrixAdjacency[j][i] != std::numeric_limits<int>::max()) 
                {
                    weightMatrixAdjacency[j][i] -= minElements[i];
                }
            }
        }
    }
}

BranchAndBound::ZeroValuation BranchAndBound::_searchMaxGradeZero(std::vector<std::vector<int>>& weightMatrixAdjacency,
                                                                  std::vector<int>& rowIndexDecreasingMatrix, 
                                                                  std::vector<int>& colIndexDecreasingMatrix) 
{
    ZeroValuation maxGradeZero;
    maxGradeZero.grade = 0;

    for (size_t i = 0; i < weightMatrixAdjacency.size(); ++i) {
        for (size_t j = 0; j < weightMatrixAdjacency.size(); ++j) {
            if (weightMatrixAdjacency[i][j] == 0) {
                std::vector<std::vector<int>> copyMatrixWithoutSearchMin = weightMatrixAdjacency;
                copyMatrixWithoutSearchMin[i][j] = -1;
                std::vector<int> minRow = _searchMinInRowOrCol(ROW, copyMatrixWithoutSearchMin);
                std::vector<int> minCol = _searchMinInRowOrCol(COL, copyMatrixWithoutSearchMin);

                // cout << "row = " << i << ",  " << "col = " << j << endl; 
                // cout << endl << "Min in each row: ";
                // _outputVector(minRow);
                // cout << endl << "Min for current zero: " << minRow[i] << endl;
                // cout << endl << "Min in each col: ";
                // _outputVector(minCol);
                // cout << endl << "Min for current zero: " << minCol[j] << endl;
                // cout << endl;

                if (minRow[i] == std::numeric_limits<int>::max() ||
                    minCol[j] == std::numeric_limits<int>::max()) 
                {
                    maxGradeZero.row = i;
                    maxGradeZero.col = j;
                    maxGradeZero.grade = std::numeric_limits<int>::max();
                }
                else {
                    if (maxGradeZero.grade <= minRow[i] + minCol[j]) {
                        maxGradeZero.row = i;
                        maxGradeZero.col = j;
                        maxGradeZero.grade = minRow[i] + minCol[j];
                    }
                }
            }
        }
    }

    cout << endl << "Max grade zero: " << endl;
    cout << maxGradeZero.grade << endl;
    cout << "Row: " << maxGradeZero.row << endl << "Col: " << maxGradeZero.col << endl;
    return maxGradeZero;
}

void BranchAndBound::_output() {
    cout << endl << "List adjacency for Hamiltonian Cycle" << endl;

    for(int i = 1; i <= _weightListAdjacency.size(); i++) {
        printf("Vertex %d:\n", i);
        for(int j = 0; j < _weightListAdjacency[i].size(); j++) {
            cout << "   vertex: " 
            << _weightListAdjacency[i][j].getNumber()
            << ", size: " << _weightListAdjacency[i][j].getWeight()
            << ", coord: " << _weightListAdjacency[i][j].getX() << ";" << _weightListAdjacency[i][j].getY() << endl;      
        }

        cout << endl;
    }
    // cout << endl << "Method Branch And Bound for Salesmans task" << endl;
    // cout << "Total weight: " << _lowerBound << endl;
    
    // for (auto edge : _hamiltonianCycle) {
    //     cout << edge.first << " - " << edge.second << endl;
    // }
}

void BranchAndBound::_outputMatrix(std::vector<std::vector<int>>& weightMatrixAdjacency) {
    cout << std::setw(5 * weightMatrixAdjacency.size()) << endl << "Output Matrix" << endl;

    for (size_t i = 0; i < weightMatrixAdjacency.size(); ++i) {
        for (size_t j = 0; j < weightMatrixAdjacency.size(); ++j) {
            cout << std::setw(5);

            if (weightMatrixAdjacency[i][j] == std::numeric_limits<int>::max()) {
                cout << "inf";
            }
            else {
                cout << weightMatrixAdjacency[i][j];
            }

            cout << "    ";
        }

        cout << endl;
    }
}

void BranchAndBound::_outputVector(std::vector<int>& vector) {
    for (size_t i = 0; i < vector.size(); ++i) {
        cout << std::setw(5);

        if (vector[i] == std::numeric_limits<int>::max()) {
            cout << "inf";
        }
        else {
            cout << vector[i];
        }

        cout << "     ";
    }
}
