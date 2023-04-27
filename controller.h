
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "rbtree.h"


class Controller
{
public:
    Controller();
    Controller(RBTree *tree);

private:
    RBTree *tree;
};

#endif // CONTROLLER_H
