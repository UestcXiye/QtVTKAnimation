#include "QtVTKAnimation.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtVTKAnimation w;
    w.show();
    return a.exec();
}
