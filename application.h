
#ifndef APPLICATION_H
#define APPLICATION_H

#include "controller.h"
#include "rbtree.h"
#include "widget.h"

namespace rbtree {
class Application
{
public:
    Application();

private:
    static RBTree getDefaultTree();

    RBTree tree_;
    Widget widget_;
    Controller controller_;
};
} // namespace rbtree

#endif // APPLICATION_H
