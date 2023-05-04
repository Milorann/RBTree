
#include "graphicsnode.h"

namespace rbtree {
GraphicsNode::GraphicsNode(float x, float y, QColor color, int value)
    : x(x)
    , y(y)
    , color(color)
    , value(value)
{}

QRectF GraphicsNode::boundingRect() const
{
    return QRectF(x, y, radius * 2, radius * 2);
}

void GraphicsNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QRectF rec = boundingRect();

    painter->setPen(QPen(color, 2));
    painter->drawEllipse(rec);
    painter->drawText(rec, Qt::AlignCenter, std::to_string(value).c_str());
}
} // namespace rbtree
