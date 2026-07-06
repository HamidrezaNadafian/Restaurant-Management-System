#include <QApplication>
#include "roleselectionwindow.h"

#include "main.cpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    initialize_database_tables();


    RoleSelectionWindow *roleWindow = new RoleSelectionWindow();
    roleWindow->show();
    return QApplication::exec();
}
