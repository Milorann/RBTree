
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "rbtree.h"
#include "widget.h"

class Controller
{
    using ControllerData = Widget::ControllerData;

public:
    Controller();
    Controller(RBTree *tree);

    Observer<ControllerData> *view() {
        return &controllerDataAction_;
    }

private:
    void doAction(const ControllerData &controllerData){
        // Сделать что-нибудь с деревом.
    }

    RBTree *tree;

    Observer<ControllerData> controllerDataAction_ = [this](const ControllerData& data) {
        doAction(data);
    };
};

#endif // CONTROLLER_H
