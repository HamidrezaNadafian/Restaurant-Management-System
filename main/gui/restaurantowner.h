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
};

#endif // RESTAURANTOWNER_H
