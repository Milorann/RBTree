
#ifndef WIDGET_H
#define WIDGET_H

#include "graphicsnode.h"
#include "Observer.h"
#include "rbtree.h"

#include <QWidget>
#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget

{
    Q_OBJECT

    using DrawData = RBTree::DrawData;

public:
    struct ControllerData
    {
        int x;
        enum Action { DEFAULT, INSERT, ERASE, FIND, LOWERBOUND, UPPERBOUND };
        Action action;
    };

    struct ViewTree
    {
        float x;
        float y;
        int key;
        float subtree_width;
        QColor color;
        ViewTree *left;
        ViewTree *right;

        ViewTree(float x, float y, int key, float subtree_width, QColor color);
        static void nodeDestructor(ViewTree *node);

        static constexpr int gap = 30;
        static constexpr int vertical_gap = 70;
    };

    Widget(QWidget *parent = nullptr);
    ~Widget();

    Observer<DrawData> *view();

    void subscribe(Observer<ControllerData> *observer);

private slots:
    void on_pushButton_insert_clicked();

private:
    void drawData(const DrawData &drawData);

    void makeViewTree(const DrawData &drawData);
    void buildViewTree(const std::pair<Node *, DrawData::Status> &changedNode,
                       Node *modelNode,
                       ViewTree *&viewNode,
                       float dy);
    QColor getColor(const std::pair<Node *, DrawData::Status> &changedNode, Node *modelNode);
    void fillX(ViewTree *viewNode, int sgn, float parent_x);
    void drawTree(ViewTree *viewNode);

    Ui::Widget *ui;
    QGraphicsScene *graphicsScene;

    ViewTree *viewRoot;

    Observable<ControllerData> controllerData_ = ControllerData{};
    Observer<DrawData> drawDataView_ = [this](const DrawData& data) {
        drawData(data);
    };
};

#endif // WIDGET_H
