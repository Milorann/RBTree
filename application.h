
#ifndef APPLICATION_H
#define APPLICATION_H

#include "controller.h"
#include "rbtree.h"
#include "widget.h"

namespace rbtree {
class Application;
}

class Application
{
public:
    Application();
    ~Application();

private:
    RBTree *tree;
    Widget w;
    Controller *controller;
};

#endif // APPLICATION_H
