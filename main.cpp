
#include "application.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    rbtree::Application app;
    return a.exec();
}
