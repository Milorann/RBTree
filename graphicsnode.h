
#ifndef GRAPHICSNODE_H
#define GRAPHICSNODE_H

#include <QPainter>
#include <QGraphicsItem>

class GraphicsNode : public QGraphicsItem
{
    float x;
    float y;
    QColor color;
    int value;

public:
    GraphicsNode(float x, float y, QColor color, int value);

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    static constexpr int radius = 15;
};

#endif // GRAPHICSNODE_H
