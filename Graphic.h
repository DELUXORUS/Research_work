#ifndef GRAPHIC_H_
#define GRAPHIC_H_

#include <vector>
#include <map>
#include <set>
#include <string>
#include <cmath>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "WeightGraph.h"

#define RADIUS_VERTEX 50


class Graphic {
    public:
        void initializeGraphic();
        Graphic() {}
        ~Graphic() { _destroyGraphic(); }
        void drawArrow(Vertex initialVertex, Vertex finalVertex);
        void drawVertex(Vertex currentVertex);
        void drawEdge(Vertex vertex1, Vertex vertex2);
        void drawWeight(Vertex vertex1, Vertex vertex2, int weight);
        void rendering(std::map<int, std::vector<Vertex>>& listAdjacency, std::vector<Vertex>& numberVertex);
        void windowCleaning() { XClearWindow(_display, _window); }
        bool checkCollisionVertex(Vertex& currentVertex, std::vector<Vertex>& numberVertex);
        void outputInstruction();
        Display* getDisplay() { return _display; }
    private:
        void _createWindow();
        void _createGC();
        void _setColor();
        bool _loadFont();
        void _destroyGraphic();
        Window _rootWindow;
        Window _window;
        Display* _display;
        GC _gc[3];
        XColor _color[3];
        XFontStruct* _fontInfo;
};

#endif