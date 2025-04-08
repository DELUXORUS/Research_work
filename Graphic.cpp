#include "Graphic.h"


void Graphic::initializeGraphic() {
    _display = XOpenDisplay(nullptr);
    
    if(_loadFont() == false)
        throw std::runtime_error("Failed to load the font!");

    _loadFont();
    _setColor();
    _createWindow();
    _createGC();
}

bool Graphic::_loadFont() {
    _fontInfo = XLoadQueryFont(_display, "6x13");

    if (_fontInfo == nullptr) {
        return false;
    }

    return true;
}

void Graphic::_setColor() {
    Colormap colormap = DefaultColormap(_display, DefaultScreen(_display));
    XColor exact;

    XAllocNamedColor(_display, colormap, "#FFFFFF", &_color[0], &exact); // Цвет шрифта
    XAllocNamedColor(_display, colormap, "#4B0082", &_color[1], &exact); // Цвет вершины и ребра
    XAllocNamedColor(_display, colormap, "#008080", &_color[2], &exact); // Цвет фона
}

void Graphic::_createGC() {
    _gc[0] = XCreateGC(_display, _window, 0, NULL);
    _gc[1] = XCreateGC(_display, _window, 0, NULL);
    _gc[2] = XCreateGC(_display, _window, 0, NULL);


    XSetFont(_display, _gc[0], _fontInfo->fid);
    XSetForeground(_display, _gc[0], _color[0].pixel);
    XSetForeground(_display, _gc[1], _color[1].pixel);

    int lineWidth = 5;
    XSetForeground(_display, _gc[2], _color[1].pixel);
    XSetLineAttributes(_display, _gc[2], lineWidth, LineSolid, CapButt, JoinMiter);

}

void Graphic::_createWindow() {
    unsigned long mask = CWOverrideRedirect | CWEventMask | CWBackPixel;
    int depth = DefaultDepth(_display, DefaultScreen(_display));
    
    XSetWindowAttributes attributes;
    attributes.override_redirect = False;
    attributes.event_mask = ButtonPressMask | ExposureMask | KeyPressMask;
    attributes.background_pixel = _color[2].pixel; 
    
    _rootWindow = DefaultRootWindow(_display);
    _window = XCreateWindow(_display, _rootWindow, 50, 50, 640, 480, 1, 
                            depth, InputOutput, CopyFromParent, mask, &attributes);
    XMapWindow(_display, _window);
}

void Graphic::drawWeight(Vertex vertex1, Vertex vertex2, int weight) {
    std::string weightStr = std::to_string(weight);
    int distX = (vertex1.getX() + vertex2.getX()) / 2;
    int distY = (vertex1.getY() + vertex2.getY()) / 2;
    XDrawString(_display, _window, _gc[0], distX, distY, weightStr.c_str(), weightStr.size());
}

void Graphic::drawVertex(Vertex currentVertex) {
    int xCoordCenterVertex = currentVertex.getX();
    int yCoordCenterVertex = currentVertex.getY();
    std::string numberVertex = std::to_string(currentVertex.getNumber());

    XFillArc(_display, _window,  _gc[1], xCoordCenterVertex - RADIUS_VERTEX  / 2, 
             yCoordCenterVertex - RADIUS_VERTEX  / 2, 
             RADIUS_VERTEX, RADIUS_VERTEX, 0, 360 * 64);
    XDrawString(_display, _window,  _gc[0], xCoordCenterVertex, yCoordCenterVertex, 
                numberVertex.c_str(), numberVertex.size());
}

void Graphic::drawEdge(Vertex vertex1, Vertex vertex2) {
    XDrawLine(_display, _window, _gc[2], vertex1.getX(),
              vertex1.getY(), vertex2.getX(), vertex2.getY());
              
    drawVertex(vertex1);
    drawVertex(vertex2);
}

bool Graphic::checkCollisionVertex(Vertex& currentVertex, Graph& graph) {    
    for (int i = 0; i < graph.getVectorVertex().size(); i++) {
        int x = graph.getVectorVertex()[i].getX() - currentVertex.getX();
        int y = graph.getVectorVertex()[i].getY() - currentVertex.getY();
        
        if (x * x + y * y <= RADIUS_VERTEX * RADIUS_VERTEX) {
            currentVertex = graph.getVectorVertex()[i];
            return true; 
        }
    }
    
    return false;
}

bool Graphic::checkCollisionVertex(Vertex& currentVertex, std::vector<Vertex>& numberVertex) {    
    for(int i = 0; i < numberVertex.size(); i++) {
        int x = numberVertex[i].getX() - currentVertex.getX();
        int y = numberVertex[i].getY() - currentVertex.getY();
        
        if(x * x + y * y <= RADIUS_VERTEX * RADIUS_VERTEX) {
            currentVertex.setNumber(numberVertex[i].getNumber());
            return true; 
        }
    }
    
    return false;
}

void Graphic::rendering(std::map<int, std::vector<Vertex>>& listAdjacency, std::vector<Vertex>& numberVertex) {
    XClearWindow(_display, _window);
    std::set<int> renderedVertex;
    
    for(auto& vertex : listAdjacency) {
        Vertex currentVertex = numberVertex[vertex.first - 1];
        drawVertex(currentVertex);
        renderedVertex.insert(currentVertex.getNumber());
        
        for(auto& adjacencyVertex : vertex.second) {
            if(renderedVertex.find(adjacencyVertex.getNumber()) == renderedVertex.end()) {
                drawVertex(adjacencyVertex);
                renderedVertex.insert(adjacencyVertex.getNumber());

            }
            
            drawEdge(currentVertex, adjacencyVertex);

            if(adjacencyVertex.getWeight() != 0) 
                drawWeight(currentVertex, adjacencyVertex, adjacencyVertex.getWeight());
        }
    }
}

void Graphic::_destroyGraphic() {
    XDestroyWindow(_display, _window);
	XCloseDisplay(_display);
}