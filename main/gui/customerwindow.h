#ifndef CUSTOMERWINDOW_H
#define CUSTOMERWINDOW_H

#include <QDialog>
#include <QWidget>
#include <QString>

#include "../Restaurantdb.h"
#include "../Restaurant.h"

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

private:

    Ui::customerwindow *ui;

    QString CurrentCustomerUsername;

    vector<Restaurant> displayedRestaurants;
    int selectedRestaurantId;

    enum Pages {
        HOME = 0,
        RESTAURANTSLIST = 1,
        MENULIST = 2,
        MYORDER = 3
    };

};

#endif // CUSTOMERWINDOW_H
