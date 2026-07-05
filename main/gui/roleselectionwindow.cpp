#include "roleselectionwindow.h"
#include "./ui_roleselectionwindow.h"
#include <QMessageBox>

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
    QMessageBox::information(this, "Test", "Customer clicked");
}


void RoleSelectionWindow::on_ownerButton_clicked()
{
    QMessageBox::information(this, "Test", "Owner clicked");
}


void RoleSelectionWindow::on_adminButton_clicked()
{
    QMessageBox::information(this, "Test", "Admin clicked");

}

