#include <QApplication>
#include "roleselectionwindow.h"

#include "Menudb.h"
#include "Restaurantdb.h"
#include "Orderdb.h"
#include "Login_and_SignUp.h"


void dbLoad()
{
    DataBase dbmain;
    RestaurantDAO rstdb(dbmain);
    MenuItemDAO menuitm(dbmain);
    OrderDAO ord(dbmain);
    OrderItemsDAO ordItm(dbmain);
    LOGINDAO dbaslog(dbmain);


    rstdb.CreateRestaurantTable();
    menuitm.CreateTable();
    ord.CreateOrderTable();
    dbaslog.CreateLOGINTable();
    ordItm.CreateOrderItemsTable();

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    dbLoad();

    RoleSelectionWindow *roleWindow = new RoleSelectionWindow();
    roleWindow->show();
    return QApplication::exec();
}
