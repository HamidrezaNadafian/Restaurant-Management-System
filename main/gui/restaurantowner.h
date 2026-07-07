#ifndef RESTAURANTOWNER_H
#define RESTAURANTOWNER_H

#include <QDialog>

namespace Ui {
class RestaurantOwner;
}

class RestaurantOwner : public QDialog
{
    Q_OBJECT

public:
    explicit RestaurantOwner(QString username , QWidget *parent = nullptr);
    ~RestaurantOwner();

private:
    Ui::RestaurantOwner *ui;

    int EditRestaurantId = -1;
    QString Username;

private slots:

    void loadOwnerRestaurants();
    void on_btnEditRestaurant_clicked();
    void on_btnManageMenu_clicked();
    void on_btnManageOrders_clicked();

    void on_btnSaveRestaurant_clicked();

};

#endif // RESTAURANTOWNER_H
