#include <iostream>
#include <vector>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>

#include "Graphic.h"
#include "Vertex.h"
#include "WeightGraph.h"
#include "BaseWeightAlgorithm.h"
#include "BranchAndBound.h"
#include "OrientedGraph.h"
#include "Control.h"


using std::cin;
using std::cout;
using std::endl;


int main(int argc, char **argv) {
    WeightGraph weightGraph;
    OrientedGraph orientedGraph;
    Graphic graphic;
    graphic.initialize();
    Control control;
    control.dispatch(graphic, weightGraph, orientedGraph);
}


