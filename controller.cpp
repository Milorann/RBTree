
#include "controller.h"

Controller::Controller()
    : tree(nullptr)
{}

Controller::Controller(RBTree *tree)
    : tree(tree)
{}

Observer<Controller::ControllerData> *Controller::view()
{
    return &controllerDataAction_;
}

void Controller::doAction(const ControllerData &controllerData)
{
    switch (controllerData.action) {
    case ControllerData::Action::INSERT:
        tree->insert(controllerData.x);
        break;
    case ControllerData::Action::ERASE:
        tree->erase(controllerData.x);
        break;
    case ControllerData::Action::FIND:
        tree->find(controllerData.x);
        break;
    case ControllerData::Action::LOWERBOUND:
        tree->lowerBound(controllerData.x);
        break;
    case ControllerData::Action::UPPERBOUND:
        tree->upperBound(controllerData.x);
        break;
    case ControllerData::Action::CLEAR:
        tree->clear();
        break;
    case ControllerData::Action::PRINT:
        tree->inOrder(tree->root);
        break;
    default:
        break;
    }
}
