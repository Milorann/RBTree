
#ifndef GRAPHICSNODE_H
#define GRAPHICSNODE_H

#include <QPainter>
#include <QGraphicsItem>

#include "rbtree.h"

enum Status {
    PASSING, FOUND, DELETED, DEFAULT
};

class GraphicsNode : public QGraphicsItem
{
    float x;
    float y;
    Color color;
    int value;
    Status status;

public:
    GraphicsNode(float x, float y, Color color, int value);

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // GRAPHICSNODE_H
