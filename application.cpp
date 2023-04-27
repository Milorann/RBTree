
#include "application.h"

Application::Application()
{
    tree = new RBTree({1, 3, 5, 7, 9, 11});
    controller = new Controller(tree);
    tree->subscribe(w.view());
    w.subscribe(controller->view());
    w.show();
}

Application::~Application()
{
    delete tree;
    delete controller;
}

