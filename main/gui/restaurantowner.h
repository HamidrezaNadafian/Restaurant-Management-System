#ifndef RESTAURANTOWNER_H
#define RESTAURANTOWNER_H

#include <QDialog>
#include <QListWidgetItem>

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

    int EditMenuId = -1;

    int SelectedOrderId = -1;

private slots:

    void loadOwnerRestaurants();
    void on_btnEditRestaurant_clicked();
    void on_btnManageMenu_clicked();
    void on_btnManageOrders_clicked();

    void on_btnSaveRestaurant_clicked();

    void loadRestaurantMenu();
    void on_listMenu_itemClicked(QListWidgetItem *item);
    void on_btnAddNewMenuItem_clicked();
    void on_btnSaveMenuItem_clicked();
    void on_btnDeleteMenuItem_clicked();

    void loadRestaurantOrders();
    void on_listOrders_itemClicked(QListWidgetItem *item);

    void ChangeOrderStatus(const std::string& newStatus);

    void on_btnPreparing_clicked();
    void on_btnDelivered_clicked();
    void on_btnCancelOrder_clicked();


};

#endif // RESTAURANTOWNER_H
