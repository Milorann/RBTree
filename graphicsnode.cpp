
#include "graphicsnode.h"

GraphicsNode::GraphicsNode(float x, float y, Color color, int value)
    : x(x)
    , y(y)
    , color(color)
    , value(value)
{
    status = Status::DEFAULT;
}

QRectF GraphicsNode::boundingRect() const
{
    return QRectF(x, y, 30, 30);
}

void GraphicsNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = boundingRect();
    if (color == Color::BLACK) {
        painter->setPen(QPen(Qt::black));
    } else {
        painter->setPen(QPen(Qt::red));
    }
    painter->drawEllipse(rec);
    painter->drawText(rec, Qt::AlignCenter, std::to_string(value).c_str());
}

