
#include "application.h"

Application::Application()
{
    tree = new RBTree({1, 3, 5, 7, 9, 11});
    controller = Controller(tree);
    w.show();
}

Application::~Application()
{
    delete tree;
}

