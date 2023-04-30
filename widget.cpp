
#include "widget.h"
#include "./ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    graphicsScene = new QGraphicsScene(this);
    ui->graphicsView->setScene(graphicsScene);
    viewRoot = nullptr;
}

Widget::~Widget()
{
    ViewTree::nodeDestructor(viewRoot);
    delete ui;
    delete graphicsScene;
}

Observer<Widget::DrawData> *Widget::view()
{
    return &drawDataView_;
}

void Widget::subscribe(Observer<ControllerData> *observer)
{
    controllerData_.subscribe(observer);
}

void Widget::on_pushButton_insert_clicked()
{

}

void Widget::drawData(const DrawData &drawData)
{
    graphicsScene->clear();
    makeViewTree(drawData);
    drawTree(viewRoot);
}

void Widget::makeViewTree(const DrawData &drawData)
{
    ViewTree::nodeDestructor(viewRoot);
    if (drawData.root == nullptr) {
        return;
    }

    viewRoot = new ViewTree(0, 0, drawData.root->key, 0, Qt::black);

    buildViewTree(drawData.changedNode, drawData.root->left, viewRoot->left, 0);
    buildViewTree(drawData.changedNode, drawData.root->right, viewRoot->right, 0);

    fillX(viewRoot->left, -1, 0);
    fillX(viewRoot->right, 1, 0);
}

void Widget::buildViewTree(const std::pair<Node *, DrawData::Status> &changedNode,
                           Node *modelNode,
                           ViewTree *&viewNode,
                           float dy)
{
    if (modelNode == nullptr) {
        return;
    }

    QColor viewNodeColor = getColor(changedNode, modelNode);

    viewNode = new ViewTree(0,
                            dy * ViewTree::vertical_gap + ViewTree::vertical_gap,
                            modelNode->key,
                            0,
                            viewNodeColor);

    buildViewTree(changedNode, modelNode->left, viewNode->left, dy + 1);
    buildViewTree(changedNode, modelNode->right, viewNode->right, dy + 1);

    if (viewNode->left == nullptr && viewNode->right == nullptr) {
        viewNode->subtree_width = 2 * GraphicsNode::radius;
    } else if (viewNode->left && viewNode->right) {
        viewNode->subtree_width = viewNode->left->subtree_width + viewNode->right->subtree_width
                                  + ViewTree::gap;
    } else if (viewNode->left) {
        viewNode->subtree_width = viewNode->left->subtree_width * 2 + ViewTree::gap;
    } else {
        viewNode->subtree_width = viewNode->right->subtree_width * 2 + ViewTree::gap;
    }
}

QColor Widget::getColor(const std::pair<Node *, DrawData::Status> &changedNode, Node *modelNode)
{
    QColor color;
    if (modelNode->color == Color::BLACK) {
        color = Qt::black;
    } else {
        color = Qt::red;
    }
    if (modelNode == changedNode.first) {
        switch (changedNode.second) {
        case DrawData::Status::DELETED:
            color = Qt::darkGray;
            break;
        case DrawData::Status::FOUND:
            color = Qt::green;
            break;
        case DrawData::Status::PASSING:
            color = Qt::cyan;
            break;
        default:
            break;
        }
    }

    return color;
}

void Widget::fillX(ViewTree *viewNode, int sgn, float parent_x)
{
    if (viewNode == nullptr) {
        return;
    }

    viewNode->x = parent_x + sgn * (GraphicsNode::radius + viewNode->subtree_width * 0.5);

    fillX(viewNode->left, -1, viewNode->x);
    fillX(viewNode->right, 1, viewNode->x);
}

void Widget::drawTree(ViewTree *viewNode)
{
    if (viewNode == nullptr) {
        return;
    }

    float l;
    float h;
    int dx;
    int dy;
    if (viewNode->left) {
        dx = (viewNode->x + GraphicsNode::radius) - (viewNode->left->x + GraphicsNode::radius);
        dy = (viewNode->left->y + GraphicsNode::radius) - (viewNode->y + GraphicsNode::radius);
        l = GraphicsNode::radius * dx / sqrt(dx * dx + dy * dy);
        h = GraphicsNode::radius * dy / sqrt(dx * dx + dy * dy);

        graphicsScene->addLine(viewNode->x + GraphicsNode::radius - l,
                               viewNode->y + GraphicsNode::radius + h,
                               viewNode->left->x + GraphicsNode::radius + l,
                               viewNode->left->y + GraphicsNode::radius - h);
        drawTree(viewNode->left);
    }
    if (viewNode->right) {
        dx = ((viewNode->right->x + GraphicsNode::radius) - (viewNode->x + GraphicsNode::radius));
        dy = (viewNode->right->y + GraphicsNode::radius) - (viewNode->y + GraphicsNode::radius);
        l = GraphicsNode::radius * dx / sqrt(dx * dx + dy * dy);
        h = GraphicsNode::radius * dy / sqrt(dx * dx + dy * dy);

        graphicsScene->addLine(viewNode->x + GraphicsNode::radius + l,
                               viewNode->y + GraphicsNode::radius + h,
                               viewNode->right->x + GraphicsNode::radius - l,
                               viewNode->right->y + GraphicsNode::radius - h);
        drawTree(viewNode->right);
    }
    graphicsScene->addItem(
        new GraphicsNode(viewNode->x, viewNode->y, viewNode->color, viewNode->key));
}


Widget::ViewTree::ViewTree(float x, float y, int key, float subtree_width, QColor color)
    : x(x)
    , y(y)
    , key(key)
    , subtree_width(subtree_width)
    , color(color)
    , left(nullptr)
    , right(nullptr)
{}

void Widget::ViewTree::nodeDestructor(ViewTree *node)
{
    if (node == nullptr) {
        return;
    }
    nodeDestructor(node->left);
    nodeDestructor(node->right);
    delete node;
}

