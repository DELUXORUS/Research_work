#include "Control.h"

void Control::dispatch(Graphic& graphic, WeightGraph& weightGraph, 
                       OrientedGraph& orientedGraph) 
{
    cout << "Press \"i\" for show instruction and \"o\" for hide" << endl;
    XEvent event;
    int flagDone = 0;

    while(flagDone == 0) {
        XNextEvent(graphic.getDisplay(), &event);

        switch(event.type) {
            case Expose: {
                graphic.windowCleaning();

                graphic.rendering(_currentGraph, _vertex);
                break;
            }

            case ButtonPress: {
                if (_currentGraph.size() == 0) {
                    Vertex currentVertex(0, 0, event.xbutton.x, event.xbutton.y);

                    if(graphic.checkCollisionVertex(currentVertex, _vertex) == true) {
                        if(event.xbutton.button == Button1) {    
                            _constructionWeightGraph(graphic, weightGraph, currentVertex);
                        }
                        else if (event.xbutton.button == Button3) {
                            _constructionOrientedGraph(graphic, orientedGraph, currentVertex);
                        }   

                    } 
                    else {
                        _addVertex(graphic, currentVertex);
                        weightGraph.getVectorVertex() = _vertex;
                        orientedGraph.getVectorVertex() = _vertex;
                    }
                }
                else {
                    cout << "It's a test graph, you can't change it!" << endl;
                }

                break;
            }

            case KeyPress:
                flagDone = _keyAction(&event, graphic, weightGraph, orientedGraph);
                break;
        }
    }
}

void Control::_addVertex(Graphic& graphic, Vertex currentVertex) {
    if (_selectVertexForWeight.size() == 0) {
        currentVertex.setNumber(_countVertex);
        _vertex.push_back(currentVertex);
        graphic.drawVertex(currentVertex);
        _countVertex++;
    }
}

void Control::_constructionWeightGraph(Graphic& graphic, WeightGraph& weightGraph, Vertex currentVertex) {
    if (_lockButton1 == 0) {
        _lockButton3 = 1;

        if(_selectVertexForWeight.size() == 0)
            _selectVertexForWeight.push_back(currentVertex);

        else {
            _selectVertexForWeight.push_back(currentVertex);

            if (weightGraph.checkEdge(_selectVertexForWeight[0].getNumber(), 
                                        _selectVertexForWeight[1].getNumber()) == 0)
            {
                graphic.drawEdge(_selectVertexForWeight[0], _selectVertexForWeight[1]);
                cout << endl << "Enter weight for edge " << _selectVertexForWeight[0].getNumber() << " - " << currentVertex.getNumber() << ":" << endl;
                int weight;
                cin >> weight;
                graphic.drawWeight(_selectVertexForWeight[0], _selectVertexForWeight[1], weight);
                weightGraph.fillMatrixWeight(_selectVertexForWeight[0].getNumber(), currentVertex.getNumber(), weight);
                weightGraph.fillListWeight();
                _selectVertexForWeight.clear();
            }
            else {
                _selectVertexForWeight.clear();
            }
        }
    }
}

void Control::_constructionOrientedGraph(Graphic& graphic, OrientedGraph& orientedGraph, Vertex currentVertex) {
    if (_lockButton3 == 0) {
        _lockButton1 = 1;

        if(_selectVertexForWeight.size() == 0)
            _selectVertexForWeight.push_back(currentVertex);

        else {
            _selectVertexForWeight.push_back(currentVertex);

            if (orientedGraph.checkEdge(_selectVertexForWeight[0].getNumber(), 
                                        _selectVertexForWeight[1].getNumber()) == 0) 
            {
                Vertex initialVertex = _selectVertexForWeight[0];
                Vertex finalVertex = _selectVertexForWeight[1];
                int numberVertex1 = initialVertex.getNumber();
                int numberVertex2 = finalVertex.getNumber();
                int weight;

                cout << endl << "Enter weight for edge " << _selectVertexForWeight[0].getNumber() << " - " << currentVertex.getNumber() << ":" << endl;
                cin >> weight;
                graphic.drawEdge(_selectVertexForWeight[0], _selectVertexForWeight[1]);
                graphic.drawWeightForOriented(initialVertex, finalVertex, weight);
                graphic.drawArrow(initialVertex, finalVertex);
                
                orientedGraph.fillMatrixAdjacency(initialVertex.getNumber(), finalVertex.getNumber(), weight);
                orientedGraph.fillListAdjacency();
                _selectVertexForWeight.clear();
            }
            else {
                _selectVertexForWeight.clear();
            }
        }
    } 
}

int Control::_keyAction(XEvent* event, Graphic& graphic, 
                     WeightGraph& weightGraph, OrientedGraph& orientedGraph)
{
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
                _currentGraph.clear();
                copy(weightGraph.getWeightListAdjacency().begin(), 
                     weightGraph.getWeightListAdjacency().end(), 
                     inserter(_currentGraph, _currentGraph.end()));
                _vertex = weightGraph.getVectorVertex();
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
                _currentGraph.clear();
                copy(orientedGraph.getListAdjacency().begin(), 
                     orientedGraph.getListAdjacency().end(), 
                     inserter(_currentGraph, _currentGraph.end()));
                _vertex = orientedGraph.getVectorVertex();
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
                        _currentGraph.clear();
                        copy(derivedBranchAndBound->getWeightListAdjacency().begin(), 
                            derivedBranchAndBound->getWeightListAdjacency().end(), 
                            inserter(_currentGraph, _currentGraph.end()));
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
                            _currentGraph.clear();
                            copy(derivedBranchAndBound->getWeightListAdjacency().begin(), 
                                derivedBranchAndBound->getWeightListAdjacency().end(), 
                                inserter(_currentGraph, _currentGraph.end()));
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

                _currentGraph.clear();
                copy(weightGraph.getWeightListAdjacency().begin(), 
                    weightGraph.getWeightListAdjacency().end(), 
                    inserter(_currentGraph, _currentGraph.end()));
                graphic.rendering(weightGraph.getWeightListAdjacency(), 
                                  _vertex);
            }

            else {
                _currentGraph.clear();
                copy(orientedGraph.getListAdjacency().begin(), 
                    orientedGraph.getListAdjacency().end(), 
                    inserter(_currentGraph, _currentGraph.end()));
                graphic.rendering(orientedGraph.getListAdjacency(), 
                                  _vertex);
            }
            
            break;
        }

        case XK_BackSpace: {
            graphic.windowCleaning();
            graphic.outputLegend();
            weightGraph.reset();
            orientedGraph.reset();
            _currentGraph.clear();
            _vertex.clear();
            _selectVertexForWeight.clear();
            _countVertex = 1;
            _lockButton1 = 0;
            _lockButton3 = 0;
            break;
        }

  		default:
  			break;
	}

	return(0);
}