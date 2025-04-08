#ifndef BASE_WEIGHT_ALGHORITMS_H
#define BASE_WEIGHT_ALGHORITMS_H

#include <iostream>

#include "WeightGraph.h"


class WeightAlgorithm {
    public:
        WeightAlgorithm(WeightGraph weightGraph): _weightGraph(weightGraph) {};
        WeightAlgorithm() {};
        virtual void search() = 0;
    protected:
        virtual void _output() = 0; 
        WeightGraph _weightGraph;
};
#endif
