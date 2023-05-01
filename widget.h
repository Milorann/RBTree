
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

    using DrawData = RBTree::DrawData;

public:
    struct ControllerData
    {
        int x;
        enum Action { DEFAULT, INSERT, ERASE, FIND, LOWERBOUND, UPPERBOUND, CLEAR, PRINT };
        Action action;
    };

    struct ViewTree
    {
        ViewTree(float x, float y, int key, float subtree_width, QColor color);
        static void nodeDestructor(ViewTree *node);

        static constexpr int gap = 30;
        static constexpr int vertical_gap = 70;

        float x;
        float y;
        int key;
        float subtree_width;
        QColor color;
        ViewTree *left;
        ViewTree *right;
    };

    Widget(QWidget *parent = nullptr);
    ~Widget();

    Observer<DrawData> *view();

    void subscribe(Observer<ControllerData> *observer);

private slots:
    void on_pushButton_insert_clicked();

    void on_pushButton_delete_clicked();

    void on_pushButton_find_clicked();

    void on_pushButton_lowerBound_clicked();

    void on_pushButton_upperBound_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_print_clicked();

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

    std::string printTree(ViewTree *node);

    bool isInitial_;

    Ui::Widget *ui;
    QGraphicsScene *graphicsScene;
    ViewTree *viewRoot;

    Observable<ControllerData> controllerData_ = ControllerData{};
    Observer<DrawData> drawDataView_ = [this](const DrawData &data) { drawData(data); };
};

#endif // WIDGET_H
