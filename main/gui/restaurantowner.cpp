#include "restaurantowner.h"
#include "ui_restaurantowner.h"

#include <QMessageBox>
#include <QDebug>

#include "Login_and_SignUp.h"
#include "Restaurantdb.h"
#include "Menudb.h"
#include "Orderdb.h"

const int DOLLARS_PER_POINT = 1;

RestaurantOwner::RestaurantOwner(QString username , QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RestaurantOwner)
{
    ui->setupUi(this);

    Username = username;

    connect(ui->btnEditRestaurant, &QPushButton::clicked, this, &RestaurantOwner::on_btnEditRestaurant_clicked);
    connect(ui->btnManageMenu, &QPushButton::clicked, this, &RestaurantOwner::on_btnManageMenu_clicked);
    connect(ui->btnManageOrders, &QPushButton::clicked, this, &RestaurantOwner::on_btnManageOrders_clicked);

    connect(ui->btnCancelEdit, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(0);
    });


    connect(ui->btnBackFromMenu, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(ui->btnBackFromOrders, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    loadOwnerRestaurants();

    ui->stackedWidget->setCurrentIndex(0);
}

RestaurantOwner::~RestaurantOwner()
{
    delete ui;
}

void RestaurantOwner::loadOwnerRestaurants()
{
    ui->listWidgetOwnerRestaurants->clear();

    DataBase dbmain;
    LOGINDAO dbaslog(dbmain);
    RestaurantDAO rstdb(dbmain);

    string Ownerusername = Username.toStdString();
    int UserId = dbaslog.getUserIdByUsername(Ownerusername);

    vector<Restaurant> allRestaurants = rstdb.getRestaurants();



    for (auto &res : allRestaurants)
    {
        if (res.getManagerID() != UserId) continue;

        int resId = res.getID();
        QString name = QString :: fromStdString(res.getName());
        QString address = QString::fromStdString(res.getAddress());
        QString desc = QString::fromStdString(res.getDescription());
        QString Phonenmbr = QString::fromStdString(res.getPhone());
        double ShppingCost = res.getshippingCost();


        QString status;

        if(res.getActive() == 0)status = "🔴 NotActive";
        if(res.getActive() == 1)status = "🟢 Active";

        QString cardText = "Name : " + name + "\n\nAddress : " + address + "\n\nDescription : " + desc + "\n\nPhone Number : " + Phonenmbr + "\n\n"
                            "Shipping Cost : $ " + QString::number( ShppingCost ,'f', 2 ) +"\n\n" + status ;
            ;

        QListWidgetItem *item = new QListWidgetItem(cardText);

        item->setData(Qt::UserRole, resId);
        item->setData(Qt::UserRole + 1, name);
        item->setData(Qt::UserRole + 2, address);
        item->setData(Qt::UserRole + 3, Phonenmbr);
        item->setData(Qt::UserRole + 4, desc);
        item->setData(Qt::UserRole + 5, ShppingCost);

        ui->listWidgetOwnerRestaurants->addItem(item);
    }
}


void RestaurantOwner::on_btnEditRestaurant_clicked()
{
    QListWidgetItem *selectedItem = ui->listWidgetOwnerRestaurants->currentItem();

    if (!selectedItem) {
        QMessageBox::warning(this , "Error" , "Please select a restaurant from the list first.");
        return;
    }
    EditRestaurantId = selectedItem->data(Qt::UserRole).toInt();

    QString OldName = selectedItem->data(Qt::UserRole + 1).toString();
    QString OldAddress = selectedItem->data(Qt::UserRole + 2).toString();
    QString OldPhone = selectedItem->data(Qt::UserRole + 3).toString();
    QString OldDesc = selectedItem->data(Qt::UserRole + 4).toString();
    double OldShppingCost = selectedItem->data(Qt::UserRole + 5).toDouble();

    ui->txtRestName->setText(OldName);
    ui->txtRestAddress->text();
    ui->txtRestAddress->setText(OldAddress);
    ui->txtRestPhone->setText(OldPhone);
    ui->txtRestDesc->setText(OldDesc);
    ui->spinshippingcost->setValue(OldShppingCost);

    ui->stackedWidget->setCurrentIndex(1);
}

void RestaurantOwner::on_btnManageMenu_clicked()
{
    QListWidgetItem *selectedItem = ui->listWidgetOwnerRestaurants->currentItem();

    if (!selectedItem) {
        QMessageBox::warning(this , "Error" , "Please select a restaurant from the list first.");
        return;
    }

    EditRestaurantId = selectedItem->data(Qt::UserRole).toInt();

    ui->stackedWidget->setCurrentIndex(2);
    loadRestaurantMenu();
}

void RestaurantOwner::on_btnManageOrders_clicked()
{
    QListWidgetItem *selectedItem = ui->listWidgetOwnerRestaurants->currentItem();

    if (!selectedItem) {
        QMessageBox::warning(this , "Error" , "Please select a restaurant from the list first.");
        return;
    }

    EditRestaurantId = selectedItem->data(Qt::UserRole).toInt();

    loadRestaurantOrders();
    ui->stackedWidget->setCurrentIndex(3);

}
void RestaurantOwner::on_btnSaveRestaurant_clicked()
{
    QString NewName = ui->txtRestName->text().trimmed();
    QString NewAddress = ui->txtRestAddress->text().trimmed();
    QString NewPhone = ui->txtRestPhone->text().trimmed();
    QString NewDesc = ui->txtRestDesc->text().trimmed();
    double NewShppingCost = ui->spinshippingcost->value();

    QListWidgetItem *selectedItem = ui->listWidgetOwnerRestaurants->currentItem();

    string OldName = selectedItem->data(Qt::UserRole + 1).toString().toStdString();
    string OldAddress = selectedItem->data(Qt::UserRole + 2).toString().toStdString();
    string OldPhone = selectedItem->data(Qt::UserRole + 3).toString().toStdString();
    string OldDesc = selectedItem->data(Qt::UserRole + 4).toString().toStdString();

    string FinalName = NewName.isEmpty() ? OldName : NewName.toStdString();
    string FinalAddress = NewAddress.isEmpty() ? OldAddress : NewAddress.toStdString();
    string FinalPhone = NewPhone.isEmpty() ? OldPhone : NewPhone.toStdString();
    string FinalDesc = NewDesc.isEmpty() ? OldDesc : NewDesc.toStdString();



    DataBase dbmain;
    RestaurantDAO rstdb(dbmain);

    rstdb.UpdateINFO(EditRestaurantId , 0 , FinalName);
    rstdb.UpdateINFO(EditRestaurantId , 1 , FinalAddress);
    rstdb.UpdateINFO(EditRestaurantId , 3 , FinalPhone);
    rstdb.UpdateINFO(EditRestaurantId , 4 , FinalDesc);
    rstdb.UpdateINFO(EditRestaurantId , 6 , to_string(NewShppingCost));

    QMessageBox::information(this, "Success", "Restaurant information updated successfully.");

    ui->stackedWidget->setCurrentIndex(0);
    loadOwnerRestaurants();
}

void RestaurantOwner::loadRestaurantMenu()
{
    ui->listMenu->clear();

    EditMenuId = -1;

    DataBase dbmain;
    MenuItemDAO menudb(dbmain);

    vector<unique_ptr<MenuItem>> MenuItems = menudb.MenuForRestaurant(EditRestaurantId);

    for(const auto& item : MenuItems) {

        QString name = QString::fromStdString(item->getName());
        QString price = QString::number(item->getPrice());
        QString desc = QString::fromStdString(item->getDescription());
        QString avaible = QString::number(item->getAvailable());

        QString category = QString::fromStdString(item->FoodOrDrink());
        int cookOrVol = item->CookOrVol();
        int isSpecial = item->getIsSpecial();

        int itemId = item->getID();

        QString cardText = name + "\n$ " + price;
        QListWidgetItem *listItem = new QListWidgetItem(cardText);

        listItem->setData(Qt::UserRole, itemId);
        listItem->setData(Qt::UserRole + 1, name);
        listItem->setData(Qt::UserRole + 2, price);
        listItem->setData(Qt::UserRole + 3, desc);
        listItem->setData(Qt::UserRole + 4, avaible);
        listItem->setData(Qt::UserRole + 5, category);
        listItem->setData(Qt::UserRole + 6, cookOrVol);
        listItem->setData(Qt::UserRole + 7, isSpecial);

        ui->listMenu->addItem(listItem);

    }
}

void RestaurantOwner::on_listMenu_itemClicked(QListWidgetItem *item)
{
    EditMenuId = item->data(Qt::UserRole).toInt();

    QString name = item->data(Qt::UserRole + 1).toString();
    double price = item->data(Qt::UserRole + 2).toDouble();
    QString desc = item->data(Qt::UserRole + 3).toString();
    QString Available = item->data(Qt::UserRole + 4).toString();

    QString category = item->data(Qt::UserRole + 5).toString();
    int cookOrVol = item->data(Qt::UserRole + 6).toInt();
    int isSpecial = item->data(Qt::UserRole + 7).toInt();

    ui->txtFoodName->setText(name);
    ui->spinFoodPrice->setValue(price);
    ui->txtFoodDesc->setPlainText(desc);

    if(Available == "1"){
        ui->comboBox->setCurrentText("Available");
    }
    else{
        ui->comboBox->setCurrentText("NotAvailable");
    }

    ui->cmbCategory->setCurrentText(category);
    ui->spinCookOrVol->setValue(cookOrVol);
    ui->chkIsSpecial->setChecked(isSpecial == 1);

}



void RestaurantOwner::on_btnAddNewMenuItem_clicked()
{
    EditMenuId = -1;

    ui->txtFoodName->clear();
    ui->spinFoodPrice->setValue(0.0);
    ui->txtFoodDesc->clear();
    ui->comboBox->setCurrentIndex(0);

    ui->cmbCategory->setCurrentIndex(0);
    ui->spinCookOrVol->setValue(0);
    ui->chkIsSpecial->setChecked(false);
}


void RestaurantOwner::on_btnSaveMenuItem_clicked()
{
    QString name = ui->txtFoodName->text().trimmed();
    double price = ui->spinFoodPrice->value();
    QString desc = ui->txtFoodDesc->toPlainText().trimmed();
    QString avaible = ui->comboBox->currentText();
    QString category = ui->cmbCategory->currentText();
    int cookOrVol = ui->spinCookOrVol->value();
    int isSpecial = ui->chkIsSpecial->isChecked() ? 1 : 0;

    if(price <= 0){
        QMessageBox::warning(this, "Error" ,"Price must be positive.");
        return;
    }

    DataBase dbmain;
    MenuItemDAO menudb(dbmain);

    int Isavaible;
    if(avaible == "Available")Isavaible = 1;
    else Isavaible = 0;

    if(EditMenuId == -1)
    {
        menudb.AddMenuItem(EditRestaurantId , name.toStdString() , desc.toStdString() , price , Isavaible , category.toStdString() , cookOrVol , isSpecial);
        QMessageBox::information(this, "Success" , "Menu item added successfully.");
    }
    else{

        menudb.UpdateItem(EditMenuId, name.toStdString(), desc.toStdString(), price , Isavaible , cookOrVol , isSpecial);
        QMessageBox::information(this, "Success" , "Changes saved successfully.");
    }
    loadRestaurantMenu();
    on_btnAddNewMenuItem_clicked();
}


void RestaurantOwner::on_btnDeleteMenuItem_clicked()
{
    if (EditMenuId == -1){
        QMessageBox::warning(this, "Error" ,"Please select a food item from the list first.");
        return;
    }

    DataBase dbmain;
    MenuItemDAO menudb(dbmain);

    menudb.DeleateMenuItem(EditMenuId);

    QMessageBox::information(this,"Success" ,"Item deleted successfully.");

    loadRestaurantMenu();
    on_btnAddNewMenuItem_clicked();
}

void RestaurantOwner::loadRestaurantOrders()
{
    SelectedOrderId = -1;

    ui->listOrders->clear();
    ui->listOrderDetails->clear();
    ui->lblCustomerID->setText(0);

    ui->lblTotalPrice->setText("Total Cost : 0.00" );

    DataBase dbmain;
    OrderDAO ord(dbmain);

    vector<Order> AllOrders = ord.AllOrders("restaurantId" , EditRestaurantId);


    for (auto &ord : AllOrders)
    {
        int OrderId = ord.getID();

        QString CustomerID = QString::number(ord.getUserID());

        double TotalPrice = ord.getPrice();
        QString status = QString::fromStdString(ord.getStatus());

        QString cardText = "Order ID : " + QString::number(OrderId) + "\n" + status;
        QListWidgetItem *item = new QListWidgetItem(cardText);

        item->setData(Qt::UserRole, OrderId);
        item->setData(Qt::UserRole + 1, CustomerID);
        item->setData(Qt::UserRole + 2, TotalPrice);
        item->setData(Qt::UserRole + 3, status);

        ui->listOrders->addItem(item);
    }
}

void RestaurantOwner::on_listOrders_itemClicked(QListWidgetItem *item)
{
    ui->listOrderDetails->clear();

    SelectedOrderId = item->data(Qt::UserRole).toInt();
    int CustomerID = item->data(Qt::UserRole + 1).toInt();
    double totalPrice = item->data(Qt::UserRole + 2).toDouble();

    ui->lblCustomerID->setText("Customer ID : " + QString::number(CustomerID));
    ui->lblTotalPrice->setText("Total Cost : $ " + QString::number(totalPrice));


    DataBase dbmain;
    OrderItemsDAO ordItm(dbmain);

    vector<OrderItem> OrdItems = ordItm.GetItemsForOrder(SelectedOrderId);

    for (auto &Item : OrdItems){
        QString ItemName = QString::fromStdString(Item.getItemName());
        int quantity = Item.getQuantity();

        QString cardText = ItemName + "    X" + QString::number(quantity);

        ui->listOrderDetails->addItem(cardText);
    }
}

void RestaurantOwner::ChangeOrderStatus(const std::string& NewStatus)
{
    if(SelectedOrderId == -1)
    {
        QMessageBox::warning(this, "Error" , "Please select an order first.");
        return;
    }

    QListWidgetItem *selectedItem = ui->listOrders->currentItem();
    if (!selectedItem) return;

    QString CurrentStatus = selectedItem->data(Qt::UserRole + 3).toString();

    if (CurrentStatus == "Cancel") {
        QMessageBox::warning(this, "Error", "This order is already canceled and cannot be modified!");
        return;
    }

    if (CurrentStatus.toStdString() == NewStatus) {
        QMessageBox::information(this, "Info", "This order is already marked as " + QString::fromStdString(NewStatus));
        return;
    }


    DataBase dbmain;
    OrderDAO ord(dbmain);
    LOGINDAO dbaslog(dbmain);

    if (NewStatus == "Cancel") {
        int CustomerId = selectedItem->data(Qt::UserRole + 1).toString().toInt();
        double totalPrice = selectedItem->data(Qt::UserRole + 2).toDouble();
        string Username = dbaslog.getUsernameById(CustomerId);

        Customer* customer = dbaslog.getCustomerByUsername(Username);

        if(!customer){
            return;
        }

        int CurrentPoints = customer->getPoints();

        int NewPoints = CurrentPoints - (totalPrice / DOLLARS_PER_POINT);

        if (NewPoints < 0) NewPoints = 0;
        qDebug() << NewPoints << "\n";
        dbaslog.updateLoyalty(CustomerId , NewPoints , "NULL");
        delete customer;
    }

    ord.UpdateStatusOrder(NewStatus , SelectedOrderId);

    QMessageBox::information(this , "Success" , "Order status updated successfully.");

    loadRestaurantOrders();
}


void RestaurantOwner::on_btnPreparing_clicked()
{
    ChangeOrderStatus("Preparing");
}

void RestaurantOwner::on_btnDelivered_clicked()
{
    ChangeOrderStatus("Delivered");
}
void RestaurantOwner :: on_btnCancelOrder_clicked()
{
    ChangeOrderStatus("Cancel");

}