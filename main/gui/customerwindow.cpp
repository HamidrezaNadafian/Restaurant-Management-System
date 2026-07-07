#include "customerwindow.h"
#include "ui_customerwindow.h"

#include <QMessageBox>
#include <QDebug>

#include "../Restaurantdb.h"
#include "../Menudb.h"
#include "../Orderdb.h"
#include "Login_and_SignUp.h"

customerwindow::customerwindow(QString username , QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::customerwindow)
{

    ui->setupUi(this);

    connect(ui->btnBack, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(ui->btnBack_2, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(1);
    });

    connect(ui->btnBack_5, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(ui->menuListWidget, &QListWidget::currentRowChanged,
            this, &customerwindow::showItemDetails);

    connect(ui->menuListWidget, &QListWidget::itemDoubleClicked,
            this, &customerwindow::addToCart);

    connect(ui->cartListWidget, &QListWidget::itemDoubleClicked,
            this, &customerwindow::removeFromCart);

    connect(ui->btnCheckout, &QPushButton::clicked,
            this, &customerwindow::processCheckout);

    CurrentCustomerUsername = username;
    ui->stackedWidget->setCurrentIndex(Pages::HOME);

    connect(ui->listWidgetOrders, &QListWidget::itemClicked, this, &customerwindow::onOrderClicked);
}

customerwindow::~customerwindow()
{
    delete ui;
}

void customerwindow::on_btnBrowseRestaurants_clicked()
{
    ui->stackedWidget->setCurrentIndex(Pages::RESTAURANTSLIST);



    ui->restaurantListWidget->clear();
    displayedRestaurants.clear();

    DataBase dbmain;
    RestaurantDAO rstdb(dbmain);



    displayedRestaurants = rstdb.getRestaurants();


    for(auto & res : displayedRestaurants){
        if(res.getActive()!= 0) ui->restaurantListWidget->addItem(QString::fromStdString(res.getName()));
    }

}

void customerwindow::on_restaurantListWidget_currentRowChanged(int currentRow)
{
    if (currentRow < 0 || currentRow >= displayedRestaurants.size()) {
        return;
    }

    Restaurant selectedRest = displayedRestaurants[currentRow];

    ui->Namelbl->setText(QString::fromStdString(selectedRest.getName()));
    ui->desclbl->setText(QString::fromStdString(selectedRest.getDescription()));
    ui->Addresslbl->setText(QString::fromStdString(selectedRest.getAddress()));
    ui->Phnumberlbl->setText(QString::fromStdString(selectedRest.getPhone()));
}

void customerwindow::on_btnEnterRestaurant_clicked()
{
    int currentRow = ui->restaurantListWidget->currentRow();
    if (currentRow == -1) {
        QMessageBox::warning(this, "Selection Error", "Please select a restaurant first!");
        return;
    }

    selectedRestaurantId = displayedRestaurants[currentRow].getID();
    RestaurantName = displayedRestaurants[currentRow].getName();



    loadMenu(selectedRestaurantId);
    ui->stackedWidget->setCurrentIndex(Pages::MENULIST);
}

void customerwindow::on_btnMyOrders_clicked()
{
    loadOrdersList();
    ui->stackedWidget->setCurrentIndex(Pages::MYORDER);
}

void customerwindow::loadMenu(int restaurantId)
{
    ui->menuListWidget->clear();
    ui->cartListWidget->clear();
    cartTotal = 0;
    ui->lblDetailPrice->setText("Total = $ 0");

    ui->lblDetailName_2->clear();
    ui->lblDetailPrice->clear();
    ui->lblDetailDesc->clear();
    ui->lblDetailStatus->clear();

    DataBase dbmain;
    MenuItemDAO menuitm(dbmain);
    menuitm.CreateTable();

    currentMenuItems = menuitm.MenuForRestaurant(restaurantId);



    for (const auto& item : currentMenuItems) {

        QString name = QString::fromStdString(item->getName());
        QString price = "$ " + QString::number(item->getPrice());

        ui->menuListWidget->addItem(name + "\n" + price);

    }
}

void customerwindow::showItemDetails(int currentRow)
{
    if(currentRow < 0 || currentRow >= currentMenuItems.size()) return;

    const auto& selectedItem = currentMenuItems[currentRow];

    ui->lblDetailName_2->setText(QString::fromStdString(selectedItem->getName()));
    ui->lblDetailPrice->setText("$ " + QString::number(selectedItem->getPrice()));

    ui->lblDetailDesc->setText(QString::fromStdString(selectedItem->getDescription()));

    if (selectedItem->getAvailable() > 0) {
        ui->lblDetailStatus->setStyleSheet("color: #2ed573;");
        ui->lblDetailStatus->setText("Available");
    }

    else{
        ui->lblDetailStatus->setStyleSheet("color: #ff4757;");
        ui->lblDetailStatus->setText("NotAvailable");
    }

}


void customerwindow::addToCart(QListWidgetItem *item)
{
    int row = ui->menuListWidget->row(item);

    const auto& selectedItem = currentMenuItems[row];

    if (selectedItem->getAvailable() <= 0){
        QMessageBox::warning(this,"ItemError" ,  "Item not available.");
        return ;
    }

    QString icon = (selectedItem->FoodOrDrink() == "Food") ? "🍔 " : "🥤 ";
    QString name = QString::fromStdString(selectedItem->getName());
    double price = selectedItem->getPrice();

    QListWidgetItem *cartItem = new QListWidgetItem(icon + name);

    cartItem->setData(Qt::UserRole, price);
    cartItem->setData(Qt::UserRole + 1, name);

    ui->cartListWidget->addItem(cartItem);


    cartTotal += price;
    ui->lblTotalPrice->setText("Total = $ " + QString::number(cartTotal));

}


void customerwindow::removeFromCart(QListWidgetItem *item)
{
    double itemPrice = item->data(Qt::UserRole).toDouble();
    cartTotal -= itemPrice;

    ui->lblTotalPrice->setText("Total = $ " + QString::number(cartTotal));

    delete item;
}

void customerwindow::processCheckout()
{
    if (ui->cartListWidget->count() == 0)
    {
        QMessageBox::warning(this,"CartError","Your cart is empty.");
        return;
    }

    DataBase dbmain;
    OrderDAO ord(dbmain);
    OrderItemsDAO ordItm(dbmain);

    ord.CreateOrderTable();
    ordItm.CreateOrderItemsTable();

    LOGINDAO dbaslog(dbmain);
    dbaslog.CreateLOGINTable();

    string Username = CurrentCustomerUsername.toStdString();
    int UserID = dbaslog.getUserIdByUsername(Username);

    int ORDERID = ord.AddOrder(UserID , selectedRestaurantId ,RestaurantName, cartTotal , "Awaiting restaurant approval");

    for (int i = 0; i < ui->cartListWidget->count() ; i++){

        QListWidgetItem *item = ui->cartListWidget->item(i);

        double Price = item->data(Qt::UserRole).toDouble();
        string Name = item->data(Qt::UserRole + 1).toString().toStdString();

        ordItm.AddOrderItem(ORDERID ,Name , 1 , Price);
    }

    ui->cartListWidget->clear();
    cartTotal = 0;
    ui->lblTotalPrice->setText("Total = $ 0");

}

void customerwindow::loadOrdersList()
{
    ui->listWidgetOrders->clear();

    DataBase dbmain;
    OrderDAO ord(dbmain);


    ord.CreateOrderTable();


    LOGINDAO dbaslog(dbmain);
    dbaslog.CreateLOGINTable();

    string Username = CurrentCustomerUsername.toStdString();
    int UserID = dbaslog.getUserIdByUsername(Username);

    vector<Order> userOrders = ord.AllOrders("UserID", UserID);


    for (auto &order : userOrders) {

        QString RestName = QString::fromStdString(order.getRestaurantName());
        int orderId = order.getID();

        QListWidgetItem *item = new QListWidgetItem(RestName + "\nOrder ID: #" + QString::number(orderId));

        item->setData(Qt::UserRole, orderId);
        ui->listWidgetOrders->addItem(item);
    }

}

void customerwindow::onOrderClicked(QListWidgetItem *item)
{
    ui->listWidgetItems->clear();

    int OrderId = item->data(Qt::UserRole).toInt();

    DataBase dbmain;
    OrderDAO orderDao(dbmain);

    LOGINDAO dbaslog(dbmain);

    string Username = CurrentCustomerUsername.toStdString();
    int UserID = dbaslog.getUserIdByUsername(Username);

    vector<Order> userOrders = orderDao.AllOrders("UserID", UserID);

    Order* FoundOrder = nullptr;
    for (Order &order : userOrders) {
        if (order.getID() == OrderId) {
            FoundOrder = &order;
            break;
        }
    }

    ui->lblDetailName->setText(QString::fromStdString(FoundOrder->getRestaurantName()));
    ui->lblDetailPrice_2->setText(QString::number(FoundOrder->getPrice(), 'f', 2) + " $");

    QString status = QString::fromStdString(FoundOrder->getStatus());
    ui->lblDetailStatus_2->setText(status);

    if (status == "Delivered"){
        ui->lblDetailStatus_2->setStyleSheet("color: #2ed573; font-weight: bold; font-size: 14px;");
    }
    else if(status[0] == 'A'){
         ui->lblDetailStatus_2->setStyleSheet("color: #f1c40f; font-weight: bold; font-size: 14px;");
    }
    else{
        ui->lblDetailStatus_2->setStyleSheet("color: #ff4757; font-weight: bold; font-size: 14px;");
    }

    OrderItemsDAO orderItemsDao(dbmain);
    std::vector<OrderItem> items = orderItemsDao.GetItemsForOrder(OrderId);

    for (auto &orderItem : items) {

        QString FoodName = QString::fromStdString(orderItem.getItemName());
        ui->listWidgetItems->addItem(FoodName);
    }

}
