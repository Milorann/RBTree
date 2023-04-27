
#ifndef APPLICATION_H
#define APPLICATION_H

#include "controller.h"
#include "widget.h"
#include "rbtree.h"


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
