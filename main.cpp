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
static std::map<int, std::vector<Vertex>> currentGraph;
static std::vector<Vertex> vertex;

int keyAction(XEvent* event, Graphic& graphic, WeightGraph& weightGraph, OrientedGraph& orientedGraph) {
	KeySym symbol;
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
                currentGraph.clear();
                copy(weightGraph.getWeightListAdjacency().begin(), 
                     weightGraph.getWeightListAdjacency().end(), 
                     inserter(currentGraph, currentGraph.end()));
                vertex = weightGraph.getVectorVertex();
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
                currentGraph.clear();
                copy(orientedGraph.getListAdjacency().begin(), 
                     orientedGraph.getListAdjacency().end(), 
                     inserter(currentGraph, currentGraph.end()));
                vertex = orientedGraph.getVectorVertex();
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
                else if (weightGraph.isFullGraph() == 0) {
                    cout << endl << "The given graph is not full!" << endl;
                    break;
                }
                
                branchAndBound->initialize(weightGraph.getWeightListAdjacency(), 
                                           weightGraph.getWeightMatrixAdjacency(), 
                                           weightGraph.getVectorVertex());
                branchAndBound->search();

                BranchAndBound* derivedBranchAndBound = static_cast<BranchAndBound*>(branchAndBound);

                if(derivedBranchAndBound != nullptr) {
                    if (derivedBranchAndBound->getWeightListAdjacency().size() == weightGraph.getNumberVertex()) {
                        currentGraph.clear();
                        copy(derivedBranchAndBound->getWeightListAdjacency().begin(), 
                            derivedBranchAndBound->getWeightListAdjacency().end(), 
                            inserter(currentGraph, currentGraph.end()));
                        graphic.rendering(derivedBranchAndBound->getWeightListAdjacency(), 
                                          weightGraph.getVectorVertex());
                    }
                }
            }

            else {
                    if (orientedGraph.isStronglyConnected() == 0) {
                        cout << endl << "The given graph is not strongly connected!" << endl;
                        break;
                    }
                    
                    branchAndBound->initialize(orientedGraph.getListAdjacency(), 
                                               orientedGraph.getMatrixAdjacency(), 
                                               orientedGraph.getVectorVertex());
                    branchAndBound->search();
        
                    BranchAndBound* derivedBranchAndBound = static_cast<BranchAndBound*>(branchAndBound);
        
                    if(derivedBranchAndBound != nullptr) {
                        if (derivedBranchAndBound->getWeightListAdjacency().size() == orientedGraph.getNumberVertex()) {
                            currentGraph.clear();
                            copy(derivedBranchAndBound->getWeightListAdjacency().begin(), 
                                derivedBranchAndBound->getWeightListAdjacency().end(), 
                                inserter(currentGraph, currentGraph.end()));
                            graphic.rendering(derivedBranchAndBound->getWeightListAdjacency(), 
                                              orientedGraph.getVectorVertex());
                        }
                    }
            }

            break;
        }

        case XK_p: {
            if(orientedGraph.getMatrixAdjacency().size() == 0) {
                if (weightGraph.getWeightMatrixAdjacency().size() == 0) {
                    cout << endl << "Neither a weighted nor an oriented graph is defined!" << endl;
                    break;
                }

                currentGraph.clear();
                copy(weightGraph.getWeightListAdjacency().begin(), 
                    weightGraph.getWeightListAdjacency().end(), 
                    inserter(currentGraph, currentGraph.end()));
                graphic.rendering(weightGraph.getWeightListAdjacency(), 
                                  vertex);
            }

            else {
                currentGraph.clear();
                copy(orientedGraph.getListAdjacency().begin(), 
                    orientedGraph.getListAdjacency().end(), 
                    inserter(currentGraph, currentGraph.end()));
                graphic.rendering(orientedGraph.getListAdjacency(), 
                                  vertex);
            }
            
            break;
        }

        case XK_BackSpace: {
            graphic.windowCleaning();
            graphic.outputLegend();
            weightGraph.reset();
            orientedGraph.reset();
            currentGraph.clear();
            vertex.clear();
            selectVertex.clear();
            countVertex = 1;
            break;
        }

  		default:
  			break;
	}

	return(0);
}

void dispatch(Graphic& graphic, WeightGraph& weightGraph, OrientedGraph& orientedGraph) {
    cout << "Press \"i\" for show instruction and \"o\" for hide" << endl;
    XEvent event;
    int flagDone = 0;

    while(flagDone == 0) {
        XNextEvent(graphic.getDisplay(), &event);

        switch(event.type) {
            case Expose: {
                graphic.windowCleaning();

                graphic.rendering(currentGraph, vertex);
                break;
            }

            case ButtonPress: {
                if (currentGraph.size() == 0) {
                    Vertex currentVertex(0, 0, event.xbutton.x, event.xbutton.y);
                    
                    if(event.xbutton.button == Button1) {       
                        if(graphic.checkCollisionVertex(currentVertex, weightGraph.getVectorVertex()) == true) {
                            if(selectVertex.size() == 0)
                                selectVertex.push_back(currentVertex);
    
                            else {
                                selectVertex.push_back(currentVertex);

                                switch (weightGraph.checkEdge(selectVertex[0].getNumber(), selectVertex[1].getNumber())) {
                                    case 1: {
                                        Vertex initialVertex = selectVertex[0];
                                        Vertex finalVertex = selectVertex[1];
                                        int numberVertex1 = initialVertex.getNumber();
                                        int numberVertex2 = finalVertex.getNumber();
                                        int weight;


                                        if (orientedGraph.checkEdge(selectVertex[1].getNumber(), selectVertex[0].getNumber()) == true) {
                                            if (orientedGraph.checkEdge(selectVertex[0].getNumber(), selectVertex[1].getNumber()) == false) {
                                                cout << endl << "Enter weight for edge " << selectVertex[0].getNumber() << " - " << currentVertex.getNumber() << ":" << endl;
                                                cin >> weight;
                                                graphic.drawWeight(initialVertex, finalVertex, weight);
                                                graphic.drawArrow(initialVertex, finalVertex);
                                            }
                                        }
                                        else {
                                            weight = weightGraph.getWeightMatrixAdjacency()[--numberVertex1][--numberVertex2];
                                            graphic.drawWeight(initialVertex, finalVertex, weight);
                                            graphic.drawArrow(initialVertex, finalVertex);
                                        }
                                        
                                        orientedGraph.getVectorVertex() = weightGraph.getVectorVertex();
                                        orientedGraph.fillMatrixAdjacency(initialVertex.getNumber(), finalVertex.getNumber(), weight);
                                        orientedGraph.fillListAdjacency();
                                        selectVertex.clear();
                                        break;
                                    }
    
                                    case 0: {
                                        graphic.drawEdge(selectVertex[0], selectVertex[1]);
                                        cout << endl << "Enter weight for edge " << selectVertex[0].getNumber() << " - " << currentVertex.getNumber() << ":" << endl;
                                        int weight;
                                        cin >> weight;
                                        graphic.drawWeight(selectVertex[0], selectVertex[1], weight);
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
                                vertex.push_back(currentVertex);
                                // orientedGraph.getVectorVertex().push_back(currentVertex);
                                graphic.drawVertex(currentVertex);
                                countVertex++;
                            }
                        }
                    }
                }
                else {
                    cout << "It's a test graph, you can't change it!" << endl;
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


