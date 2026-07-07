#include "restaurantowner.h"
#include "ui_restaurantowner.h"

#include <QMessageBox>
#include <QDebug>

#include "Login_and_SignUp.h"
#include "Restaurantdb.h"

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

        QString status;

        if(res.getActive() == 0)status = "🔴 NotActive";
        if(res.getActive() == 1)status = "🟢 Active";

        QString cardText = "Name : " + name + "\nAddress : " + address + "\n\nDescription : " + desc + "\n\nPhone Number : " + Phonenmbr + "\n" + status;

        QListWidgetItem *item = new QListWidgetItem(cardText);

        item->setData(Qt::UserRole, resId);
        item->setData(Qt::UserRole + 1, name);
        item->setData(Qt::UserRole + 2, address);
        item->setData(Qt::UserRole + 3, Phonenmbr);
        item->setData(Qt::UserRole + 4, desc);

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

    ui->txtRestName->setText(OldName);
    ui->txtRestAddress->text();
    ui->txtRestAddress->setText(OldAddress);
    ui->txtRestPhone->setText(OldPhone);
    ui->txtRestDesc->setText(OldDesc);


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
}

void RestaurantOwner::on_btnManageOrders_clicked()
{
    QListWidgetItem *selectedItem = ui->listWidgetOwnerRestaurants->currentItem();

    if (!selectedItem) {
        QMessageBox::warning(this , "Error" , "Please select a restaurant from the list first.");
        return;
    }

    EditRestaurantId = selectedItem->data(Qt::UserRole).toInt();

    ui->stackedWidget->setCurrentIndex(3);
}
void RestaurantOwner::on_btnSaveRestaurant_clicked()
{
    QString NewName = ui->txtRestName->text().trimmed();
    QString NewAddress = ui->txtRestAddress->text().trimmed();
    QString NewPhone = ui->txtRestPhone->text().trimmed();
    QString NewDesc = ui->txtRestDesc->text().trimmed();

    QListWidgetItem *selectedItem = ui->listWidgetOwnerRestaurants->currentItem();

    std::string OldName = selectedItem->data(Qt::UserRole + 1).toString().toStdString();
    std::string OldAddress = selectedItem->data(Qt::UserRole + 2).toString().toStdString();
    std::string OldPhone = selectedItem->data(Qt::UserRole + 3).toString().toStdString();
    std::string OldDesc = selectedItem->data(Qt::UserRole + 4).toString().toStdString();

    std::string FinalName = NewName.isEmpty() ? OldName : NewName.toStdString();
    std::string FinalAddress = NewAddress.isEmpty() ? OldAddress : NewAddress.toStdString();
    std::string FinalPhone = NewPhone.isEmpty() ? OldPhone : NewPhone.toStdString();
    std::string FinalDesc = NewDesc.isEmpty() ? OldDesc : NewDesc.toStdString();

    DataBase dbmain;
    RestaurantDAO rstdb(dbmain);

    rstdb.UpdateINFO(EditRestaurantId , 0 , FinalName);
    rstdb.UpdateINFO(EditRestaurantId , 1 , FinalAddress);
    rstdb.UpdateINFO(EditRestaurantId , 3 , FinalPhone);
    rstdb.UpdateINFO(EditRestaurantId , 4 , FinalDesc);

    QMessageBox::information(this, "Success", "Restaurant information updated successfully.");

    ui->stackedWidget->setCurrentIndex(0);
    loadOwnerRestaurants();
}