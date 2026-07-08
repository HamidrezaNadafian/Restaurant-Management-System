#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QDialog>

namespace Ui {
class AdminWindow;
}

class AdminWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AdminWindow(QWidget *parent = nullptr);
    ~AdminWindow();

private slots:
    void onSaveRestaurantClicked();

    void loadRestaurantsTable();
    void onToggleStatusClicked();


private:
    Ui::AdminWindow *ui;
};

#endif // ADMINWINDOW_H
