
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "rbtree.h"
#include "widget.h"

namespace rbtree {
class Controller
{
    using ControllerData = Widget::ControllerData;

public:
    Controller();
    Controller(RBTree *tree);

    Observer<ControllerData> *view();

private:
    void doAction(const ControllerData &controllerData);

    RBTree *tree_;

    Observer<ControllerData> controllerDataAction_ = [this](const ControllerData &data) {
        doAction(data);
    };
};
} // namespace rbtree

#endif // CONTROLLER_H
