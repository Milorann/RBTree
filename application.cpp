
#include "application.h"
namespace rbtree {
RBTree Application::getDefaultTree()
{
    return RBTree({7, 3, 11, 1, 13, 5, 9, 15, 12});
    //return RBTree({30, 20, 40, 15, 25, 35, 45, 12, 17, 23, 28, 34, 38, 42, 49, 27, 29, 33, 39, 47, 50});
}

Application::Application()
    : tree_(getDefaultTree())
    , controller_(&tree_)
{
    tree_.subscribe(widget_.view());
    widget_.subscribe(controller_.view());
}
} // namespace rbtree
