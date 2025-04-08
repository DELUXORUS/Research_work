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
#include "SalemansTask.h"


using std::cin;
using std::cout;
using std::endl;

int keyAction(XEvent* event, Graphic& graphic, WeightGraph& weightGraph);
void dispatch(Graphic& graphic, WeightGraph& weightGraph);
int countVertex = 1;
std::vector<Vertex> selectVertex;

int keyAction(XEvent* event, Graphic& graphic, WeightGraph& weightGraph) {
	KeySym symbol;
    WeightAlgorithm *salemansTask;

	XLookupString((XKeyEvent*)event, NULL, 0, &symbol, NULL);
	
    switch(symbol) {
  		case XK_q: {
            return 1;
  			break;
        }

        case XK_a: {
            if(weightGraph.getWeightMatrixAdjacency().size() == 0)
                cout << endl << "The graph is not set!" << endl;

            else {
                weightGraph.outputMatrixWeightGraph();
                weightGraph.outputListWeightGraph();
            }

            break;
        }

        case XK_9: {
            salemansTask = new SalemansTask(weightGraph);
            salemansTask->search();
            break;
        }

        case XK_BackSpace: {
            graphic.windowCleaning();
            weightGraph.reset();
            countVertex = 1;
            selectVertex.clear();
            break;
        }

  		default:
  			break;
	}

	return(0);
}

void dispatch(Graphic& graphic, WeightGraph& weightGraph) {
    XEvent event;
    int flagDone = 0;

    while(flagDone == 0) {
        XNextEvent(graphic.getDisplay(), &event);
        
        switch(event.type) {
            case ButtonPress: {
                Vertex currentVertex(0, 0, event.xbutton.x, event.xbutton.y);
                
                if(event.xbutton.button == Button1) {       
                    if(graphic.checkCollisionVertex(currentVertex, weightGraph.getVectorVertex()) == true) {
                        if(selectVertex.size() == 0)
                            selectVertex.push_back(currentVertex);

                        else {
                            if(weightGraph.checkEdge(selectVertex[0].getNumber(), currentVertex.getNumber()) == 0) {
                                selectVertex.push_back(currentVertex);
                                graphic.drawEdge(selectVertex[0], selectVertex[1]);
                                cout << endl << "Enter weight for edge " << selectVertex[0].getNumber() << " - " << currentVertex.getNumber() << ":" << endl;
                                int weight;
                                cin >> weight;
                                graphic.drawWeight(selectVertex[0], currentVertex, weight);
                                weightGraph.fillMatrixWeight(selectVertex[0].getNumber(), currentVertex.getNumber(), weight);
                                weightGraph.fillListWeight();
                                selectVertex.clear();
                            }
                        }
                    } 
                    
                    else {
                        if (selectVertex.size() == 0) {
                            currentVertex.setNumber(countVertex);
                            weightGraph.getVectorVertex().push_back(currentVertex); 
                            graphic.drawVertex(currentVertex);
                            countVertex++;
                        }
                    }
                }
                break;
            }

            case KeyPress:
                flagDone = keyAction(&event, graphic, weightGraph);
                break;
        }
    }
}

int main(int argc, char **argv) {
    WeightGraph weightGraph;
    Graphic graphic;
    graphic.initializeGraphic();
    dispatch(graphic, weightGraph);
}


