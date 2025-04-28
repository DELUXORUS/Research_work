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


using std::cin;
using std::cout;
using std::endl;

static int countVertex = 1;
static std::vector<Vertex> selectVertex;

int keyAction(XEvent* event, Graphic& graphic, WeightGraph& weightGraph, OrientedGraph& orientedGraph) {
	KeySym symbol;
    // WeightAlgorithm *greedyAlgorithm;
    WeightAlgorithm *branchAndBound;

	XLookupString((XKeyEvent*)event, NULL, 0, &symbol, NULL);
	
    switch(symbol) {
  		case XK_q: {
            return 1;
  			break;
        }

        case XK_i: {
            graphic.showInstruction();
            break;
        }

        case XK_o: {
            graphic.hideInstruction();
            break;
        }

        case XK_a: {
            if(orientedGraph.getMatrixAdjacency().size() == 0) {
                if (weightGraph.getWeightMatrixAdjacency().size() == 0) {
                    cout << endl << "Neither a weighted nor an oriented graph is defined!" << endl;
                    break;
                }

                weightGraph.outputMatrixWeightGraph();
                weightGraph.outputListWeightGraph();
            }

            else {
                orientedGraph.outputMatrixAdjacency();
                orientedGraph.outputListAdjacency();
            }

            break;
        }

        case XK_e: {
            if(weightGraph.getWeightMatrixAdjacency().size() == 0) {
                weightGraph.testGraph();
                graphic.rendering(weightGraph.getWeightListAdjacency(), 
                                  weightGraph.getVectorVertex());
            }

            else {
                cout << endl << "The graph is alredy set!" << endl;
            }

            break;
        }

        case XK_r: {
            if(orientedGraph.getMatrixAdjacency().size() == 0) {
                orientedGraph.testGraph();
                graphic.rendering(orientedGraph.getListAdjacency(), 
                                  orientedGraph.getVectorVertex());
            }
            else {
                cout << endl << "The oriented graph is alredy set!" << endl;
            }
                                
            break;
        }

        case XK_9: {
            branchAndBound = new BranchAndBound();

            if(orientedGraph.getMatrixAdjacency().size() == 0) {
                if (weightGraph.getWeightMatrixAdjacency().size() == 0) {
                    cout << endl << "Neither a weighted nor an oriented graph is defined!" << endl;
                    break;
                }
                
                branchAndBound->initialize(weightGraph.getWeightListAdjacency(), weightGraph.getWeightMatrixAdjacency(), weightGraph.getVectorVertex());
                branchAndBound->search();

                BranchAndBound* derivedBranchAndBound = static_cast<BranchAndBound*>(branchAndBound);

                if(derivedBranchAndBound != nullptr) {
                    if (derivedBranchAndBound->getWeightListAdjacency().size() == weightGraph.getNumberVertex()) {
                        graphic.rendering(derivedBranchAndBound->getWeightListAdjacency(), 
                                          weightGraph.getVectorVertex());
                    }
                }
            }

            else {
                    branchAndBound->initialize(orientedGraph.getListAdjacency(), orientedGraph.getMatrixAdjacency(), orientedGraph.getVectorVertex());
                    branchAndBound->search();
        
                    BranchAndBound* derivedBranchAndBound = static_cast<BranchAndBound*>(branchAndBound);
        
                    if(derivedBranchAndBound != nullptr) {
                        if (derivedBranchAndBound->getWeightListAdjacency().size() == orientedGraph.getNumberVertex()) {
                            graphic.rendering(derivedBranchAndBound->getWeightListAdjacency(), 
                                              orientedGraph.getVectorVertex());
                        }
                    }
            }

            break;
        }

        case XK_p: {
            if (weightGraph.getWeightListAdjacency().size() != 0) {
                graphic.rendering(weightGraph.getWeightListAdjacency(), 
                                  weightGraph.getVectorVertex());
            }
            
            break;
        }

        case XK_BackSpace: {
            graphic.windowCleaning();
            weightGraph.reset();
            orientedGraph.reset();
            countVertex = 1;
            selectVertex.clear();
            break;
        }

  		default:
  			break;
	}

	return(0);
}

void dispatch(Graphic& graphic, WeightGraph& weightGraph, OrientedGraph& orientedGraph) {
    XEvent event;
    int flagDone = 0;

    while(flagDone == 0) {
        XNextEvent(graphic.getDisplay(), &event);

        
        switch(event.type) {
            case Expose: {
                graphic.windowCleaning();
                graphic.rendering(weightGraph.getWeightListAdjacency(), weightGraph.getVectorVertex());
                break;
            }

            case ButtonPress: {
                Vertex currentVertex(0, 0, event.xbutton.x, event.xbutton.y);
                
                if(event.xbutton.button == Button1) {       
                    if(graphic.checkCollisionVertex(currentVertex, weightGraph.getVectorVertex()) == true) {
                        if(selectVertex.size() == 0)
                            selectVertex.push_back(currentVertex);

                        else {
                            switch (weightGraph.checkEdge(selectVertex[0].getNumber(), currentVertex.getNumber())) {
                                case 1: {
                                    Vertex initialVertex = selectVertex[0];
                                    Vertex finalVertex = currentVertex;
                                    int weight = weightGraph.getWeightMatrixAdjacency()[initialVertex.getNumber() - 1][finalVertex.getNumber() - 1];
                                    graphic.drawArrow(initialVertex, finalVertex);
                                    graphic.drawWeight(initialVertex, finalVertex, weight);
                                    orientedGraph.getVectorVertex() = weightGraph.getVectorVertex();
                                    orientedGraph.fillMatrixAdjacency(initialVertex.getNumber(), finalVertex.getNumber(), weight);
                                    orientedGraph.fillListAdjacency();
                                    selectVertex.clear();
                                    break;
                                }

                                case 0: {
                                    selectVertex.push_back(currentVertex);
                                    graphic.drawEdge(selectVertex[0], selectVertex[1]);
                                    cout << endl << "Enter weight for edge " << selectVertex[0].getNumber() << " - " << currentVertex.getNumber() << ":" << endl;
                                    int weight;
                                    cin >> weight;
                                    graphic.drawWeight(selectVertex[0], currentVertex, weight);
                                    weightGraph.fillMatrixWeight(selectVertex[0].getNumber(), currentVertex.getNumber(), weight);
                                    weightGraph.fillListWeight();
                                    selectVertex.clear();
                                    break;
                                }

                                default: {
                                    selectVertex.clear();
                                    break;
                                }

                            }
                        }
                    } 
                    
                    else {
                        if (selectVertex.size() == 0) {
                            currentVertex.setNumber(countVertex);
                            weightGraph.getVectorVertex().push_back(currentVertex);
                            // orientedGraph.getVectorVertex().push_back(currentVertex);
                            graphic.drawVertex(currentVertex);
                            countVertex++;
                        }
                    }
                }
                break;
            }

            case KeyPress:
                flagDone = keyAction(&event, graphic, weightGraph, orientedGraph);
                break;
        }
    }
}

int main(int argc, char **argv) {
    WeightGraph weightGraph;
    OrientedGraph orientedGraph;
    Graphic graphic;
    graphic.initialize();
    dispatch(graphic, weightGraph, orientedGraph);
}


