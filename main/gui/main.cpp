#include "roleselectionwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RoleSelectionWindow w;
    w.show();
    return QApplication::exec();
}
