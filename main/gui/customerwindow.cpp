#include "customerwindow.h"
#include "ui_customerwindow.h"

#include <QMessageBox>

customerwindow::customerwindow(QString username , QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::customerwindow)
{

    ui->setupUi(this);


    qDebug() << "Pages count:" << ui->stackedWidget->count();


    connect(ui->btnBack, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(ui->btnBack_2, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(1);
    });

    connect(ui->btnBack_5, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    CurrentCustomerUsername = username;
    ui->stackedWidget->setCurrentIndex(Pages::HOME);
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

    ui->stackedWidget->setCurrentIndex(Pages::MENULIST);
}

void customerwindow::on_btnMyOrders_clicked()
{
    ui->stackedWidget->setCurrentIndex(Pages::MYORDER);
}




