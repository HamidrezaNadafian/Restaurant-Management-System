#include "customerwindow.h"
#include "ui_customerwindow.h"

#include <QMessageBox>
#include <QDebug>

#include "../Restaurantdb.h"
#include "../Menudb.h"
#include "../Orderdb.h"
#include "Login_and_SignUp.h"
#include "customer.h"


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

    updateLevelInfo();
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

    double shippingCost = selectedRest.getshippingCost();

    ui->Namelbl->setText(QString::fromStdString(selectedRest.getName()));
    ui->desclbl->setText(QString::fromStdString(selectedRest.getDescription()) + "\n\nShipping Cost: $ " + QString::number(shippingCost, 'f', 2));
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

    CurrentShippingCost = displayedRestaurants[currentRow].getshippingCost();

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

    currentMenuItems = menuitm.MenuForRestaurant(restaurantId);



    for(const auto& item : currentMenuItems) {

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

    QString ItemExtrainfo = "";

    if (selectedItem->FoodOrDrink() == "Food"){
        ItemExtrainfo = "⏱ Cook Time: " + QString::number(selectedItem->CookOrVol()) + " mins\n\n";
    }
    else{
        ItemExtrainfo = "🧃 Volume: " + QString::number(selectedItem->CookOrVol()) + " ML\n\n";
    }


    ui->lblDetailDesc->setText(ItemExtrainfo + "\n" +QString::fromStdString(selectedItem->getDescription()));

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

    ui->lblTotalPrice->setText("Shipping = $ " + QString::number(CurrentShippingCost, 'f', 2) + "\n\nTotal = $ " + QString::number(CurrentShippingCost + cartTotal, 'f', 2));

    RefreshCart();
}


void customerwindow::removeFromCart(QListWidgetItem *item)
{
    double itemPrice = item->data(Qt::UserRole).toDouble();
    cartTotal -= itemPrice;

    ui->lblTotalPrice->setText("Total (Shipping) = $ " + QString::number(cartTotal + CurrentShippingCost, 'f', 2));

    RefreshCart();

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

    LOGINDAO dbaslog(dbmain);

    string Username = CurrentCustomerUsername.toStdString();
    int UserID = dbaslog.getUserIdByUsername(Username);

    Customer* CustINFO = dbaslog.getCustomerByUsername(Username);
    double ItemsDisc = 0.0;
    double shippingDisc = 1.0;


    if (CustINFO) {
        MembershipLevel* UserLevel = CustINFO->getMembership();
        if (UserLevel) {
            ItemsDisc = UserLevel->getDiscount();
            shippingDisc = UserLevel->getShippingCostdisc();
        }
    }

    double discountAmount = cartTotal * (ItemsDisc);
    double FinalItemsPrice = cartTotal - discountAmount;
    double finalShippingCost = CurrentShippingCost * (shippingDisc);
    double FinalTotalCost = FinalItemsPrice + finalShippingCost;


    int ORDERID = ord.AddOrder(UserID , selectedRestaurantId ,RestaurantName, FinalTotalCost , "Awaiting restaurant approval");

    for (int i = 0; i < ui->cartListWidget->count() ; i++){

        QListWidgetItem *item = ui->cartListWidget->item(i);

        double Price = item->data(Qt::UserRole).toDouble();
        string Name = item->data(Qt::UserRole + 1).toString().toStdString();

        ordItm.AddOrderItem(ORDERID ,Name , 1 , Price);
    }




    if(CustINFO){
        CustINFO->AddPoints(FinalTotalCost);
        int FinalPoints = CustINFO->getPoints();
        dbaslog.updateLoyalty(UserID, FinalPoints, "NULL");
        delete CustINFO;
    }

    ui->cartListWidget->clear();
    cartTotal = 0;
    ui->lblTotalPrice->setText("Total = $ 0");


    updateLevelInfo();
}

void customerwindow::loadOrdersList()
{
    ui->listWidgetOrders->clear();

    DataBase dbmain;
    OrderDAO ord(dbmain);

    LOGINDAO dbaslog(dbmain);


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

    QString status;
    if (FoundOrder){
        ui->lblDetailName->setText(QString::fromStdString(FoundOrder->getRestaurantName()));
        ui->lblDetailPrice_2->setText(QString::number(FoundOrder->getPrice(), 'f', 2) + " $");
        status = QString::fromStdString(FoundOrder->getStatus());
        ui->lblDetailStatus_2->setText(status);
    }






    if (status == "Delivered"){
        ui->lblDetailStatus_2->setStyleSheet("color: #2ed573; font-weight: bold; font-size: 14px;");
    }
    else if(status == "Awaiting restaurant approval"){
        ui->lblDetailStatus_2->setStyleSheet("color: #ff55ff; font-weight: bold; font-size: 14px;");
    }

    else if(status == "Preparing"){
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

void customerwindow :: updateLevelInfo()
{
    ui->lblCurrentLevel->clear();
    ui->lblArrow->clear();
    ui->lblNextLevel->clear();

    DataBase dbmain;
    LOGINDAO dbaslog(dbmain);

    string Username = CurrentCustomerUsername.toStdString();

    Customer* CurrentUser = dbaslog.getCustomerByUsername(Username);


    if(!CurrentUser) return;

    int points = CurrentUser->getPoints();
    MembershipLevel* UserMembership = CurrentUser->getMembership();

    if (!UserMembership) {
        qDebug() << "Error: User membership is null!";
        delete CurrentUser;
        return;
    }

    string UserLevel = UserMembership->getLevelName();

    ui->lblCurrentLevel->setText(QString("👑 %1 (%2)").arg(QString::fromStdString(UserLevel)).arg(points));

    ui->lblCurrentLevel->setStyleSheet(
        QString("background-color: %1;")
            .arg(QString::fromStdString(UserMembership->getColorCode()))
        );


    if(UserMembership->getLevelName() == "VIP"){
        ui->lblArrow->hide();
        ui->lblNextLevel->setText("🎉 You are at the Maximum Level!");
        ui->lblNextLevel->setStyleSheet("background-color: transparent;");
    }

    else{
        ui->lblArrow->show();
        ui->lblArrow->setText(" ➔ ➔ ➔ ➔ ➔ ");

        QString NextLevel = QString::fromStdString(UserMembership->getNextLevelName());
        QString NextColor = QString::fromStdString(UserMembership->getNextLevelColorCode());

        int pointsNeed = UserMembership->getTargetPoints() - points;

        ui->lblNextLevel->setText(QString("%1 (Needs %2 points)").arg(NextLevel).arg(pointsNeed));
        ui->lblNextLevel->setStyleSheet(
            QString("background-color: %1;")
                .arg(QString::fromStdString(UserMembership->getNextLevelColorCode()))
            );
    }
    delete CurrentUser;
}


void customerwindow::RefreshCart()
{
    if (cartTotal <= 0.01) {
        ui->lblTotalPrice->setText("Total = $ 0");
        return;
    }

    DataBase dbmain;
    LOGINDAO dbaslog(dbmain);
    Customer* CurrentUser = dbaslog.getCustomerByUsername(CurrentCustomerUsername.toStdString());

    double ItemsDisc = 1.0;
    double shippingDisc = 1.0;
    QString levelName = "Normal";

    if(CurrentUser){
        MembershipLevel* level = CurrentUser->getMembership();
        if (level) {
            ItemsDisc = level->getDiscount();
            shippingDisc = level->getShippingCostdisc();
            levelName = QString::fromStdString(level->getLevelName());
        }
        delete CurrentUser;
    }

    double discountAmount = cartTotal * (ItemsDisc);
    double FinalItemsPrice = cartTotal - discountAmount;
    double finalShippingCost = CurrentShippingCost * (shippingDisc);
    double FinalTotal = FinalItemsPrice + finalShippingCost;

    QString receipt = QString("Subtotal: $ %1").arg(cartTotal, 0, 'f', 2);


    if (ItemsDisc > 0) {
        receipt += QString(" -> $ %1\n").arg(FinalItemsPrice, 0, 'f', 2);
    }
    else{
        receipt += "\n";
    }


    if (shippingDisc < 1.0) {
        receipt += QString("Shipping: $ %1 -> $ %2 \n") .arg(CurrentShippingCost, 0, 'f', 2) .arg(finalShippingCost, 0, 'f', 2);
    }
    else{
        receipt += QString("Shipping: $ %1\n").arg(CurrentShippingCost, 0, 'f', 2);
    }

    receipt += QString("\nTotal: $ %1").arg(FinalTotal, 0, 'f', 2);

    ui->lblTotalPrice->setText(receipt);

}
