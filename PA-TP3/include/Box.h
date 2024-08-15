#ifndef BOX_H
#define BOX_H

#include "Point.h"

class Box {
public:
    Point topLeft;
    Point bottomRight;
    
    Box();
    Box(Point tl, Point br) : topLeft(tl), bottomRight(br) {}

    bool contains(Point p) const {
        return (p.x >= topLeft.x && p.x <= bottomRight.x &&
                p.y >= topLeft.y && p.y <= bottomRight.y);
    }

    // Box getQuadrant(int index) const;
};
#endif  // BOX_H