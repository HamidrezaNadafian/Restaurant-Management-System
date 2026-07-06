#include "roleselectionwindow.h"
#include "./ui_roleselectionwindow.h"

#include <QMessageBox>


#include "loginwindow.h"

RoleSelectionWindow::RoleSelectionWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RoleSelectionWindow)
{
    ui->setupUi(this);
}

RoleSelectionWindow::~RoleSelectionWindow()
{
    delete ui;
}

void RoleSelectionWindow::on_customerButton_clicked()
{
    LoginWindow *loginPage = new LoginWindow("Customer");
    loginPage->show();
    this->close();
}


void RoleSelectionWindow::on_ownerButton_clicked()
{
    LoginWindow *loginPage  = new LoginWindow("RestaurantOwner");
    loginPage->show();
    this->close();
}


void RoleSelectionWindow::on_adminButton_clicked()
{
    LoginWindow *loginPage = new LoginWindow("Admin");
    loginPage->show();
    this->close();
}

