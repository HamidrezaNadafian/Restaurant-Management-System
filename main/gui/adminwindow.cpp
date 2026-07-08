#include "adminwindow.h"
#include "ui_adminwindow.h"

#include <QMessageBox>
#include <QDebug>

#include "Restaurantdb.h"
#include "Login_and_SignUp.h"
#include "Orderdb.h"

AdminWindow::AdminWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AdminWindow)
{
    ui->setupUi(this);

    connect(ui->btnAddRestaurant, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(1);
    });

    connect(ui->btnManageStatus, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(2);
        loadRestaurantsTable();
    });

    connect(ui->btnViewStatistics, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(3);
        loadStatistics();
    });

    connect(ui->btnAddRestaurant_2, &QPushButton::clicked, this, &AdminWindow::onSaveRestaurantClicked);

    connect(ui->btnBackToMenu, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(ui->btnBackToMenu_2, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(ui->btnBack, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });


    connect(ui->btnToggleStatus, &QPushButton::clicked, this, &AdminWindow::onToggleStatusClicked);





    ui->stackedWidget->setCurrentIndex(0);
}


AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::onSaveRestaurantClicked()
{
    QString Name = ui->txtRestName_2->text().trimmed();
    int ManagerID = ui->spinManagerId->value();
    QString PhoneNumber = ui->txtPhone->text().trimmed();
    QString Address = ui->txtAddress->text().trimmed();
    QString description = ui->txtDescription->toPlainText().trimmed();

    int isActive = (ui->cmbStatus->currentText() == "Active");

    if (Name.isEmpty()) {
        QMessageBox::warning(this, "Error", "Name Field cannot be Empty.");
        return;
    }

    DataBase db;
    RestaurantDAO restdb(db);

    restdb.AddRestaurant(Name.toStdString() , ManagerID , Address.toStdString() , isActive , 0 , PhoneNumber.toStdString(), description.toStdString());

    QMessageBox::information(this, "Success", "Restaurant registered successfully!");
    ui->txtRestName_2->clear();
    ui->spinManagerId->setValue(1);
    ui->txtPhone->clear();
    ui->txtAddress->clear();
    ui->txtDescription->clear();
    ui->cmbStatus->setCurrentIndex(0);

}

void AdminWindow::loadRestaurantsTable()
{
    ui->tblRestaurants->setRowCount(0);

    DataBase db;
    RestaurantDAO Restdb(db);
    vector<Restaurant> Restaurants = Restdb.getRestaurants();

    for(int i = 0 ; i < (int)Restaurants.size() ; i++){

        ui->tblRestaurants->insertRow(i);

        QTableWidgetItem *IdItem = new QTableWidgetItem(QString::number(Restaurants[i].getID()));
        ui->tblRestaurants->setItem(i, 0, IdItem);

        QTableWidgetItem *NameItem = new QTableWidgetItem(QString::fromStdString(Restaurants[i].getName()));
        ui->tblRestaurants->setItem(i, 1, NameItem);

        QString StatusText;

        if(Restaurants[i].getActive() == 1)StatusText = "Active";
        else StatusText = "InActive";

        QTableWidgetItem *StatusItem = new QTableWidgetItem(StatusText);
        ui->tblRestaurants->setItem(i, 2, StatusItem);
    }
}

void AdminWindow::onToggleStatusClicked()
{
    int EditRow = ui->tblRestaurants->currentRow();

    if (EditRow == -1) {
        QMessageBox::warning(this, "Selection Error", "Please select a restaurant first.");
        return;
    }

    int RestaurantId = ui->tblRestaurants->item(EditRow, 0)->text().toInt();
    QString Status = ui->tblRestaurants->item(EditRow, 2)->text();

    string NewStatus;
    if(Status == "Active")NewStatus = "0";
    else NewStatus = "1";

    qDebug() << NewStatus << "    " << Status;

    DataBase db;
    RestaurantDAO Restdb(db);

    Restdb.UpdateINFO(RestaurantId, 5 , NewStatus);

    loadRestaurantsTable();
    QMessageBox::information(this, "Success", "Restaurant status updated successfully!");
}

void AdminWindow::loadStatistics()
{
    DataBase db;

    RestaurantDAO Restdb(db);

    int CountActiveRestaurant = 0;
    vector<Restaurant> Restaurants = Restdb.getRestaurants();
    for(int i = 0 ; i < (int)Restaurants.size() ; i++){
        if(Restaurants[i].getActive())CountActiveRestaurant++;
    }
    ui->lblValueRestaurants->setText(QString::number(CountActiveRestaurant));

    LOGINDAO Userdb(db);
    int TotalUsers = Userdb.getTotalUsers();
    ui->lblValueUsers->setText(QString::number(TotalUsers));

    OrderDAO ord(db);
    int TotalOrders = ord.getTotalOrders();
    double TotalRtevenue = ord.getTotalPrice();

    ui->lblValueOrders->setText(QString::number(TotalOrders));
    ui->lblValueRevenue->setText("$ " + QString::number(TotalRtevenue));
}