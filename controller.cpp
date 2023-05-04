
#include "controller.h"

namespace rbtree {
Controller::Controller()
    : tree_(nullptr)
{}

Controller::Controller(RBTree *tree)
    : tree_(tree)
{}

Observer<Controller::ControllerData> *Controller::view()
{
    return &controllerDataAction_;
}

void Controller::doAction(const ControllerData &controllerData)
{
    switch (controllerData.action) {
    case ControllerData::Action::Insert:
        tree_->insert(controllerData.x);
        break;
    case ControllerData::Action::Erase:
        tree_->erase(controllerData.x);
        break;
    case ControllerData::Action::Find:
        tree_->find(controllerData.x);
        break;
    case ControllerData::Action::LowerBound:
        tree_->lowerBound(controllerData.x);
        break;
    case ControllerData::Action::UpperBound:
        tree_->upperBound(controllerData.x);
        break;
    case ControllerData::Action::Clear:
        tree_->clear();
        break;
    default:
        break;
    }
}
} // namespace rbtree
