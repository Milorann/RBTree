
#ifndef WIDGET_H
#define WIDGET_H

#include "Observer.h"
#include "graphicsnode.h"
#include "rbtree.h"

#include <QWidget>
#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

    using RBTree = rbtree::RBTree;
    using Node = rbtree::Node;
    using Color = rbtree::Color;
    using DrawData = RBTree::DrawData;
    using GraphicsNode = rbtree::GraphicsNode;

public:
    struct ControllerData
    {
        int x;
        enum Action { DoNothing, Insert, Erase, Find, LowerBound, UpperBound, Clear };
        Action action;
    };

    struct ViewNode
    {
        float x;
        float y;
        int key;
        float subtree_width;
        QColor color;
        ViewNode *left;
        ViewNode *right;
    };

    class ViewTree
    {
    public:
        ViewTree() = default;
        ViewTree(const DrawData &drawData);
        ViewTree(const ViewTree &) = delete;
        ViewTree &operator=(const ViewTree &) = delete;
        ViewTree(ViewTree &&) noexcept = delete;
        ViewTree &operator=(ViewTree &&) noexcept = delete;
        ~ViewTree();

        void rebuildViewTree_(const DrawData &drawData);

        bool needAnimation = false;
        ViewNode *viewRoot = nullptr;

    private:
        void buildViewTree(const std::pair<const Node *, DrawData::Status> &changedNode,
                           Node *modelNode,
                           ViewNode *&viewNode,
                           float dy);
        void fillX(ViewNode *viewNode, int sgn, float parent_x);
        void destroySubtree(ViewNode *node);

        static constexpr int gap = 30;
        static constexpr int vertical_gap = 70;
    };

    Widget(QWidget *parent = nullptr);
    Widget(const Widget &) = delete;
    Widget &operator=(const Widget &) = delete;
    Widget(Widget &&) noexcept = delete;
    Widget &operator=(Widget &&) noexcept = delete;
    ~Widget();

    rbtree::Observer<DrawData> *view();

    void subscribe(rbtree::Observer<ControllerData> *observer);

private slots:
    void on_pushButton_insert_clicked();
    void on_pushButton_delete_clicked();
    void on_pushButton_find_clicked();
    void on_pushButton_lowerBound_clicked();
    void on_pushButton_upperBound_clicked();
    void on_pushButton_clear_clicked();
    void on_pushButton_print_clicked();

    void on_checkBox_stateChanged(int state);

private:
    void drawData(const DrawData &drawData);

    void drawTree(const ViewNode *viewNode);

    std::string traverseInOrder(ViewNode *viewNode);

    std::unique_ptr<Ui::Widget> ui_;
    QGraphicsScene *graphicsScene_;

    ViewTree viewTree_;

    rbtree::Observable<ControllerData> controllerPort_
        = ControllerData{-1, ControllerData::Action::DoNothing};
    rbtree::Observer<DrawData> dataInput_ = [this](const DrawData &data) { drawData(data); };
};

class Palette
{
    using RBTree = rbtree::RBTree;
    using Node = rbtree::Node;
    using Color = rbtree::Color;
    using DrawData = RBTree::DrawData;

public:
    static QColor getColor(const std::pair<const Node *, DrawData::Status> &changedNode,
                           const Node *modelNode);
};

#endif // WIDGET_H
