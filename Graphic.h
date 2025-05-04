#ifndef GRAPHIC_H_
#define GRAPHIC_H_

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <cmath>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "Vertex.h"

#define RADIUS_VERTEX 50


class Graphic {
    public:
        void initialize();
        Graphic() {}
        Graphic(Window rootWindow) : _rootWindow(rootWindow) {}
        ~Graphic() { _destroyGraphic(); }
        void drawArrow(Vertex initialVertex, Vertex finalVertex);
        void drawVertex(Vertex currentVertex);
        void drawEdge(Vertex vertex1, Vertex vertex2);
        // void drawWeightForOriented(Vertex vertex1, Vertex vertex2, int weight);
        void drawWeight(Vertex vertex1, Vertex vertex2, int weight);
        void rendering(std::map<int, std::vector<Vertex>>& listAdjacency, 
                       std::vector<Vertex>& numberVertex);
        void windowCleaning() { XClearWindow(_display, _window); }
        bool checkCollisionVertex(Vertex& currentVertex, std::vector<Vertex>& numberVertex);
        void showInstruction();
        void hideInstruction();
        void outputLegend();
        Display* getDisplay() { return _display; }
        private:
        bool _checkDrawArrow(std::map<int, std::vector<Vertex>>& listAdjacency, int vertex, Vertex vertexAdjacency);
        void _outputInstruction();
        void _createWindow();
        void _createWindowInstruction();
        void _createGC();
        void _setColor();
        bool _loadFont();
        void _destroyGraphic();
        Window _rootWindow;
        Window _window;
        Window _windowInstruction;
        Display* _display;
        std::vector<GC> _gc;
        std::vector<XColor> _color;
        XFontStruct* _fontInfo;
};

#endif