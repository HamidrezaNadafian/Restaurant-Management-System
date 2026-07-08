#include "adminwindow.h"
#include "ui_adminwindow.h"

#include <QMessageBox>
#include "Restaurantdb.h"

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
    });

    connect(ui->btnViewStatistics, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(3);
    });

    connect(ui->btnAddRestaurant_2, &QPushButton::clicked, this, &AdminWindow::onSaveRestaurantClicked);

    connect(ui->btnBackToMenu, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });


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
