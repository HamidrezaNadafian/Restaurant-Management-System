#ifndef CUSTOMERWINDOW_H
#define CUSTOMERWINDOW_H

#include <QDialog>
#include <QWidget>
#include <QString>
#include <QListWidgetItem>
#include <memory>
#include <vector>


#include "../Restaurant.h"
#include "../Menu.h"
#include "../Order.h"


namespace Ui {
class customerwindow;
}

class customerwindow : public QDialog
{
    Q_OBJECT

public:
    explicit customerwindow(QString username , QWidget *parent = nullptr);
    ~customerwindow();

private slots:

    void on_btnBrowseRestaurants_clicked();
    void on_btnMyOrders_clicked();

    void on_restaurantListWidget_currentRowChanged(int currentRow);

    void on_btnEnterRestaurant_clicked();

    void loadMenu(int restaurantId);

    void loadOrdersList();
    void onOrderClicked(QListWidgetItem *item);

    void showItemDetails(int currentRow);
    void addToCart(QListWidgetItem *item);

    void removeFromCart(QListWidgetItem *item);

    void processCheckout();

private:

    Ui::customerwindow *ui;

    QString CurrentCustomerUsername;

    vector<Restaurant> displayedRestaurants;
    int selectedRestaurantId;
    string RestaurantName;

    enum Pages {
        HOME = 0,
        RESTAURANTSLIST = 1,
        MENULIST = 2,
        MYORDER = 3
    };

    vector<std::unique_ptr<MenuItem>> currentMenuItems;
    double cartTotal = 0.0;

};

#endif // CUSTOMERWINDOW_H
