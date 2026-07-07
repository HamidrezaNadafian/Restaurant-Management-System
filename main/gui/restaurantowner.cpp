#include "restaurantowner.h"
#include "ui_restaurantowner.h"

RestaurantOwner::RestaurantOwner(QString username , QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RestaurantOwner)
{
    ui->setupUi(this);
}

RestaurantOwner::~RestaurantOwner()
{
    delete ui;
}
