
#ifndef GRAPHICSNODE_H
#define GRAPHICSNODE_H

#include <QGraphicsItem>
#include <QPainter>

namespace rbtree {
class GraphicsNode : public QGraphicsItem
{
public:
    GraphicsNode(float x, float y, QColor color, int value);

    static constexpr int radius = 15;

private:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

    float x;
    float y;
    QColor color;
    int value;
};
} // namespace rbtree
#endif // GRAPHICSNODE_H
