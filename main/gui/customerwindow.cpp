#include "customerwindow.h"
#include "ui_customerwindow.h"

#include <QMessageBox>
#include <QDebug>
#include <QTime>

#include "../Restaurantdb.h"
#include "../Menudb.h"
#include "../Orderdb.h"
#include "Login_and_SignUp.h"
#include "customer.h"

const double COUPON_VALUE = 10.0;


customerwindow::customerwindow(QString username , QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::customerwindow)
{

    ui->setupUi(this);

    ui->desclbl->setWordWrap(true);
    ui->lblDetailDesc->setWordWrap(true);
    ui->lblTotalPrice->setWordWrap(true);

    connect(ui->btnBack, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(0);
        updateLevelInfo();

    });

    connect(ui->btnBack_2, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(1);
    });

    connect(ui->btnBack_5, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(0);
        updateLevelInfo();

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



    vector<Restaurant> allRestaurants = rstdb.getRestaurants();


    for(auto & res : allRestaurants){
        if(res.getActive() != 0) {
            displayedRestaurants.push_back(res);
            ui->restaurantListWidget->addItem(QString::fromStdString(res.getName()));
        }
    }

    if (ui->restaurantListWidget->count() > 0){
        ui->restaurantListWidget->setCurrentRow(0);
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
    isCouponApplied = false;
    ui->btnUseCoupon->setStyleSheet("");
    cartTotal = 0;

    ui->lblTotalPrice->setText("Total = $ 0");

    ui->lblDetailName_2->clear();
    ui->lblDetailPrice->clear();
    ui->lblDetailDesc->clear();
    ui->lblDetailStatus->clear();

    DataBase dbmain;
    MenuItemDAO menuitm(dbmain);
    LOGINDAO dbaslog(dbmain);

    string Username = CurrentCustomerUsername.toStdString();
    Customer* CurrentCustomer = dbaslog.getCustomerByUsername(Username);
    string UserLevel = "NULL";

    if (CurrentCustomer) {
        if (CurrentCustomer->getMembership()) {
            UserLevel = CurrentCustomer->getMembership()->getLevelName();
        }
        delete CurrentCustomer;
    }

    currentMenuItems.clear();
    currentMenuItems = menuitm.MenuForRestaurant(restaurantId);



    for(auto& item : currentMenuItems) {
        qDebug() << "S" ;
        if (item->getIsSpecial() == 1 && (UserLevel == "Normal" || UserLevel == "Silver")) {
            continue;
        }

        QString name = QString::fromStdString(item->getName());
        QString price = "$ " + QString::number(item->getPrice());

        QString finalLine;

        if(item->getIsSpecial() == 1)
        {
            finalLine = "🌟 " + name + "\n" + price;
        }

        else{
            finalLine = name + "\n" + price;
        }

        ui->menuListWidget->addItem(finalLine);

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

    bool found = 0;

    for (int i = 0; i < ui->cartListWidget->count(); i++){
        QListWidgetItem *cartItem = ui->cartListWidget->item(i);
        if (cartItem->data(Qt::UserRole + 1).toString() == name){

            int quantity = cartItem->data(Qt::UserRole + 2).toInt();
            quantity ++;
            cartItem->setData(Qt::UserRole + 2, quantity);
            cartItem->setText(icon + name + "       X" + QString::number(quantity) + "");
            found = 1;
            break;
        }
    }

    if(found == 0){
        QListWidgetItem *cartItem = new QListWidgetItem(icon + name);
        cartItem->setData(Qt::UserRole, price);
        cartItem->setData(Qt::UserRole + 1, name);
        cartItem->setData(Qt::UserRole + 2, 1);
        cartItem->setData(Qt::UserRole + 3, icon);
        ui->cartListWidget->addItem(cartItem);
    }

    cartTotal += price;

    ui->lblTotalPrice->setText("Shipping = $ " + QString::number(CurrentShippingCost, 'f', 2) + "\n\nTotal = $ " + QString::number(CurrentShippingCost + cartTotal, 'f', 2));

    RefreshCart();
}


void customerwindow::removeFromCart(QListWidgetItem *item)
{
    double itemPrice = item->data(Qt::UserRole).toDouble();
    QString name = item->data(Qt::UserRole + 1).toString();
    int quantity = item->data(Qt::UserRole + 2).toInt();
    QString icon = item->data(Qt::UserRole + 3).toString();

    cartTotal -= itemPrice;

    if(quantity > 1)
    {
        quantity --;
        item->setData(Qt::UserRole + 2, quantity);

        item->setText(icon + name + "       X" + QString::number(quantity) + "");
    }
    else{
        delete item;
    }

    RefreshCart();
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
        delete CustINFO;
    }

    double discountAmount = cartTotal * (ItemsDisc);
    double FinalItemsPrice = cartTotal - discountAmount;
    double finalShippingCost = CurrentShippingCost * (shippingDisc);
    double FinalTotalCost = FinalItemsPrice + finalShippingCost;

    if(isCouponApplied){
        FinalTotalCost -= COUPON_VALUE;
        if(FinalTotalCost < 0) FinalTotalCost = 0;
    }

    string levelAfterOrder = "Normal";

    int ORDERID = ord.AddOrder(UserID , selectedRestaurantId ,RestaurantName, FinalTotalCost , "Awaiting restaurant approval" , levelAfterOrder);

    for (int i = 0; i < ui->cartListWidget->count() ; i++){

        QListWidgetItem *item = ui->cartListWidget->item(i);

        double Price = item->data(Qt::UserRole).toDouble();
        string Name = item->data(Qt::UserRole + 1).toString().toStdString();
        int quantity = item->data(Qt::UserRole + 2).toInt();

        int TotalPrice = Price * quantity;

        ordItm.AddOrderItem(ORDERID ,Name , quantity , TotalPrice);
    }

    if (isCouponApplied){
        Customer* CurrentUser = dbaslog.getCustomerByUsername(CurrentCustomerUsername.toStdString());
        if (CurrentUser)
        {
            int Coupons = CurrentUser->getCoupons();
            dbaslog.updateCoupons(CurrentUser->getID(), Coupons - 1);
            delete CurrentUser;
        }
    }
    QMessageBox::information(this, "Success", QString::fromStdString("Your order has been placed successfully."));



    ui->cartListWidget->clear();
    cartTotal = 0;
    ui->lblTotalPrice->setText("Total = $ 0");

    isCouponApplied = false;
    ui->btnUseCoupon->setStyleSheet("");

    updateLevelInfo();
    AwardBadges();
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

    vector<Order> UserOrders = orderDao.AllOrders("UserID", UserID);

    Order* FoundOrder = nullptr;
    for (Order &order : UserOrders) {
        if (order.getID() == OrderId) {
            FoundOrder = &order;
            break;
        }
    }

    QString status;
    QString OrderLevel;

    if (FoundOrder){
        ui->lblDetailName->setText(QString::fromStdString(FoundOrder->getRestaurantName()));
        ui->lblDetailPrice_2->setText(QString::number(FoundOrder->getPrice(), 'f', 2) + " $");
        status = QString::fromStdString(FoundOrder->getStatus());
        ui->lblDetailStatus_2->setText(status);

        OrderLevel = QString::fromStdString(FoundOrder->getUserLevel());

        if(status == "Cancel")OrderLevel = " ? ";

        ui->lblOrderLevel->setText("Level After Order: " + OrderLevel);

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
        int quantity = orderItem.getQuantity();

        ui->listWidgetItems->addItem(FoodName + "      X" + QString::number(quantity));
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

    string UserBadges = CurrentUser->getBadges();
    int flag = 0;

    QString badgeText = "";

    if (UserBadges.find("Night") != string::npos) {
        badgeText += "Night Customer  | ";
        flag = 1;
    }
    if (UserBadges.find("Frequent") != string::npos) {
        badgeText += "Frequent Buyer | ";
        flag = 1;
    }


    if (flag == 0) {
        ui->lblBadges->setText("Badges: None");
    }
    else{
        ui->lblBadges->setText("Badges: " + badgeText);
    }

    delete CurrentUser;
}


void customerwindow::RefreshCart()
{
    if (cartTotal <= 0.01) {
        ui->lblTotalPrice->setText("Total = $ 0");
        isCouponApplied = false;
        ui->btnUseCoupon->setStyleSheet("");
        return;
    }

    DataBase dbmain;
    LOGINDAO dbaslog(dbmain);
    Customer* CurrentUser = dbaslog.getCustomerByUsername(CurrentCustomerUsername.toStdString());


    double ItemsDisc = 1.0;
    double shippingDisc = 1.0;
    QString levelName = "Normal";

    double getPointsMulti = 1;

    if (CurrentUser) {
        int availableCoupons = CurrentUser->getCoupons();
        if (availableCoupons >= 0 && !isCouponApplied) {
            ui->btnUseCoupon->setText(QString("%1 Coupon").arg(availableCoupons));
            ui->btnUseCoupon->setEnabled(true);
        }
        else if(availableCoupons < 0){
            ui->btnUseCoupon->setText("0 Coupons");
            ui->btnUseCoupon->setEnabled(false);
        }

        MembershipLevel* level = CurrentUser->getMembership();
        if (level) {
            ItemsDisc = level->getDiscount();
            shippingDisc = level->getShippingCostdisc();
            levelName = QString::fromStdString(level->getLevelName());
            getPointsMulti = level->getPoints();
        }

        delete CurrentUser;
    }


    double discountAmount = cartTotal * (ItemsDisc);
    double FinalItemsPrice = cartTotal - discountAmount;
    double finalShippingCost = CurrentShippingCost * (shippingDisc);
    double FinalTotal = FinalItemsPrice + finalShippingCost;

    if (isCouponApplied) {
        FinalTotal -= COUPON_VALUE;
        if (FinalTotal < 0) FinalTotal = 0;
    }

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


    int EarnedPoint = FinalTotal * getPointsMulti;

    if (EarnedPoint > 0) {
        receipt += QString("\n Points you'll earn: +%1").arg(EarnedPoint);
    }

    ui->lblTotalPrice->setText(receipt);

}

void customerwindow::on_btnUseCoupon_clicked()
{
    DataBase dbmain;
    LOGINDAO dbaslog(dbmain);
    Customer* CurrentUser = dbaslog.getCustomerByUsername(CurrentCustomerUsername.toStdString());

    if (!CurrentUser) return;
    int availableCoupons = CurrentUser->getCoupons();

    if(!isCouponApplied) {

        if (availableCoupons > 0){
            isCouponApplied = true;

            ui->btnUseCoupon->setText("Cancel Coupon");
            ui->btnUseCoupon->setStyleSheet("background-color: #ff4757; color: white; border-radius: 8px; font-weight: bold;");

            QMessageBox::information(this, "Success", "Coupon Applied! $10 Discount");

            RefreshCart();
        }
        else{
            QMessageBox::warning(this , "Error" , "You don't have any coupons left.");
        }

    }

    else{
        isCouponApplied = false;
        ui->btnUseCoupon->setText(QString("%1 Coupon").arg(availableCoupons));
        ui->btnUseCoupon->setStyleSheet("");

        QMessageBox::information(this, "Cancelled", "Coupon removed from this order.");
        RefreshCart();
    }

    if (CurrentUser) delete CurrentUser;
}

void customerwindow :: AwardBadges(){
    DataBase dbmain;
    LOGINDAO dbaslog(dbmain);
    OrderDAO ord(dbmain);

    string Username = CurrentCustomerUsername.toStdString();
    int UserID = dbaslog.getUserIdByUsername(Username);
    Customer* CurrentUser = dbaslog.getCustomerByUsername(Username);

    if(!CurrentUser) return;

    string CurrentBadges = CurrentUser->getBadges();
    bool getNewBadges = false;

    if (CurrentBadges.find("Night") == string::npos){
        QTime now = QTime::currentTime();
        if(now.hour() >= 23 || now.hour() <= 5){
            if (!CurrentBadges.empty())CurrentBadges += ",";
            CurrentBadges += "Night";
            getNewBadges = true;

        }
    }

    if (CurrentBadges.find("Frequent") == string::npos){
        vector<Order> UserOrders = ord.AllOrders("UserID", UserID);
        if (UserOrders.size() >= 5) {

            if (!CurrentBadges.empty()) CurrentBadges += ",";
            CurrentBadges += "Frequent";
            getNewBadges = true;
        }
    }

    if (getNewBadges) {
        dbaslog.updateBadges(UserID, CurrentBadges);
        QMessageBox::information(this, "Achievement" , "Congratulations! You earned new badges" );
    }

    delete CurrentUser;

}
