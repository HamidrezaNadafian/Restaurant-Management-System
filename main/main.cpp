#include "Menudb.h"
#include "Menu.h"
#include "Restaurant.h"
#include "Restaurantdb.h"
#include "Order.h"
#include "Orderdb.h"
#include "Login_and_SignUp.h"

void initialize_database_tables() {
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

int main()
{
    return 0;
}