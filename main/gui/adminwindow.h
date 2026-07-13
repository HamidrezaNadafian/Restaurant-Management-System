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


    void onToggleStatusClicked();



    void on_btnDistributeCoupons_clicked();
    void on_btnSetNormal_clicked();
    void on_btnSetSilver_clicked();
    void on_btnSetGold_clicked();
    void on_btnSetVIP_clicked();



private:
    Ui::AdminWindow *ui;

    void ChangeUserLevel(const QString& levelName, int newPoints);
    void loadRestaurantsTable();
    void loadStatistics();
    void loadUsersList();
    void loadUserStatistics();

};

#endif // ADMINWINDOW_H
