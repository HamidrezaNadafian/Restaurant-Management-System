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

        QString status;

        if(res.getActive() == 0)status = "🔴 NotActive";
        if(res.getActive() == 0)status = "🟢 Active";

        QString cardText = name + "\n" + address + "\n" + status;

        QListWidgetItem *item = new QListWidgetItem(cardText);

        item->setData(Qt::UserRole, resId);
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
