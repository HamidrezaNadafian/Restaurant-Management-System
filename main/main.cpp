#include "User.cpp"
#include "lib.h"

int main()
{
    system("cls");
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
    
    HandleUserType(rstdb , menuitm , ord , dbaslog , ordItm);

    
}