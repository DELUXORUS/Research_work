#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>

using std::cout;
using std::endl;


class Vertex {
    public:
        Vertex() : _number(0), _weight(0), _x(0), _y(0)  {}
        Vertex(int number, int weight, int x, int y) : _number(number), _weight(weight), _x(x), _y(y) {}
        int getX() { return _x; }
        int getY() { return _y; }
        int getNumber() { return _number; }
        int getWeight() { return _weight; }
        void setNumber(int number) { _number = number; }
        void setWeight(int weight) { _weight = weight; }
    private:
        int _number;
        int _weight;
        int _x;
        int _y;
};

#endif