#include <iostream>
#include <vector>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>

#include "Graphic.h"
#include "WeightGraph.h"
#include "OrientedGraph.h"
#include "Control.h"


using std::cin;
using std::cout;
using std::endl;

int main() {
    WeightGraph weightGraph;
    OrientedGraph orientedGraph;
    Graphic graphic;
    graphic.initialize();
    Control control;
    control.dispatch(graphic, weightGraph, orientedGraph);
}
