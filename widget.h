
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

    Widget(QWidget *parent = nullptr);
    ~Widget();

    Observer<DrawData> *view();

    void subscribe(Observer<ControllerData> *observer)
    {
        controllerData_.subscribe(observer);
    }

private slots:
    void on_pushButton_insert_clicked();

private:
    void drawData(const DrawData &drawData);

    Ui::Widget *ui;
    QGraphicsScene *graphicsScene;

    Observable<ControllerData> controllerData_ = ControllerData{};
    Observer<DrawData> drawDataView_ = [this](const DrawData& data) {
        drawData(data);
    };
};

#endif // WIDGET_H
