
#include "widget.h"
#include "./ui_widget.h"

Widget::ViewTree::ViewTree(const DrawData &drawData)
{
    ViewTree::rebuildViewTree_(drawData);
}

Widget::ViewTree::~ViewTree()
{
    destroySubtree(viewRoot);
}

void Widget::ViewTree::rebuildViewTree_(const DrawData &drawData)
{
    ViewTree::destroySubtree(viewRoot);
    viewRoot = nullptr;
    if (drawData.root == nullptr) {
        return;
    }

    viewRoot = new ViewNode{0,
                            0,
                            drawData.root->key,
                            0,
                            Palette::getColor(drawData.changedNode, drawData.root)};

    buildViewTree(drawData.changedNode, drawData.root->left, viewRoot->left, 0);
    buildViewTree(drawData.changedNode, drawData.root->right, viewRoot->right, 0);

    fillX(viewRoot->left, -1, 0);
    fillX(viewRoot->right, 1, 0);
}

void Widget::ViewTree::buildViewTree(const std::pair<const Node *, DrawData::Status> &changedNode,
                                     Node *modelNode,
                                     ViewNode *&viewNode,
                                     float dy)
{
    if (modelNode == nullptr) {
        return;
    }

    QColor viewNodeColor = Palette::getColor(changedNode, modelNode);

    viewNode = new ViewNode{0, dy * vertical_gap + vertical_gap, modelNode->key, 0, viewNodeColor};

    buildViewTree(changedNode, modelNode->left, viewNode->left, dy + 1);
    buildViewTree(changedNode, modelNode->right, viewNode->right, dy + 1);

    if (viewNode->left == nullptr && viewNode->right == nullptr) {
        viewNode->subtree_width = 2 * GraphicsNode::radius;
    } else if (viewNode->left && viewNode->right) {
        viewNode->subtree_width = viewNode->left->subtree_width + viewNode->right->subtree_width
                                  + gap;
    } else if (viewNode->left) {
        viewNode->subtree_width = viewNode->left->subtree_width + gap;
    } else {
        viewNode->subtree_width = viewNode->right->subtree_width + gap;
    }
}

void Widget::ViewTree::fillX(ViewNode *viewNode, int sgn, float parent_x)
{
    if (viewNode == nullptr) {
        return;
    }

    if (sgn == -1) {
        if (viewNode->right) {
            viewNode->x = parent_x - (gap + viewNode->right->subtree_width);
        } else {
            viewNode->x = parent_x - gap;
        }
    } else {
        if (viewNode->left) {
            viewNode->x = parent_x + (gap + viewNode->left->subtree_width);
        } else {
            viewNode->x = parent_x + gap;
        }
    }

    fillX(viewNode->left, -1, viewNode->x);
    fillX(viewNode->right, 1, viewNode->x);
}

void Widget::ViewTree::destroySubtree(ViewNode *node)
{
    if (node == nullptr) {
        return;
    }
    destroySubtree(node->left);
    destroySubtree(node->right);
    delete node;
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui_(std::make_unique<Ui::Widget>())
{
    ui_->setupUi(this);
    graphicsScene_ = new QGraphicsScene(this);
    ui_->graphicsView->setScene(graphicsScene_);
    show();
}

Widget::~Widget() = default;

rbtree::Observer<Widget::DrawData> *Widget::view()
{
    return &dataInput_;
}

void Widget::subscribe(rbtree::Observer<ControllerData> *observer)
{
    controllerPort_.subscribe(observer);
}

void Widget::on_pushButton_insert_clicked()
{
    controllerPort_.set({ui_->spinBox_insert->value(), ControllerData::Action::Insert});
}

void Widget::on_pushButton_delete_clicked()
{
    controllerPort_.set({ui_->spinBox_delete->value(), ControllerData::Action::Erase});
}

void Widget::on_pushButton_find_clicked()
{
    controllerPort_.set({ui_->spinBox_find->value(), ControllerData::Action::Find});
}

void Widget::on_pushButton_lowerBound_clicked()
{
    controllerPort_.set({ui_->spinBox_lowerBound->value(), ControllerData::Action::LowerBound});
}

void Widget::on_pushButton_upperBound_clicked()
{
    controllerPort_.set({ui_->spinBox_upperBound->value(), ControllerData::Action::UpperBound});
}
void Widget::on_pushButton_clear_clicked()
{
    controllerPort_.set({-1, ControllerData::Action::Clear});
}

void Widget::on_pushButton_print_clicked()
{
    std::string print_text = traverseInOrder(viewTree_.viewRoot);
    ui_->textBrowser->setPlainText(print_text.c_str());
}

void Widget::on_checkBox_stateChanged(int state)
{
    viewTree_.needAnimation = state;
}

void Widget::drawData(const DrawData &drawData)
{
    graphicsScene_->clear();
    viewTree_.rebuildViewTree_(drawData);
    drawTree(viewTree_.viewRoot);
    repaint();
    QThread::sleep(viewTree_.needAnimation);
}

void Widget::drawTree(const ViewNode *viewNode)
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

        graphicsScene_->addLine(viewNode->x + GraphicsNode::radius - l,
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

        graphicsScene_->addLine(viewNode->x + GraphicsNode::radius + l,
                                viewNode->y + GraphicsNode::radius + h,
                                viewNode->right->x + GraphicsNode::radius - l,
                                viewNode->right->y + GraphicsNode::radius - h);
        drawTree(viewNode->right);
    }
    graphicsScene_->addItem(
        new GraphicsNode(viewNode->x, viewNode->y, viewNode->color, viewNode->key));
}

std::string Widget::traverseInOrder(ViewNode *viewNode)
{
    if (viewNode == nullptr) {
        return "";
    }

    std::string ans;

    ans = traverseInOrder(viewNode->left) + " " + std::to_string(viewNode->key) + " ";

    QColor prev_color = viewNode->color;
    viewNode->color = Qt::cyan;
    graphicsScene_->clear();
    drawTree(viewTree_.viewRoot);
    repaint();
    QThread::sleep(viewTree_.needAnimation);
    viewNode->color = prev_color;

    ans += traverseInOrder(viewNode->right);

    if (viewNode == viewTree_.viewRoot) {
        drawTree(viewTree_.viewRoot);
    }

    return ans;
}

QColor Palette::getColor(const std::pair<const Node *, DrawData::Status> &changedNode,
                         const Node *modelNode)
{
    QColor color;
    if (modelNode->color == Color::Black) {
        color = Qt::black;
    } else {
        color = Qt::red;
    }
    if (changedNode.first && modelNode->key == changedNode.first->key) {
        switch (changedNode.second) {
        case DrawData::Status::Deleted:
            color = Qt::darkGray;
            break;
        case DrawData::Status::Found:
            color = Qt::green;
            break;
        case DrawData::Status::Passing:
            color = Qt::cyan;
            break;
        default:
            break;
        }
    }

    return color;
}
