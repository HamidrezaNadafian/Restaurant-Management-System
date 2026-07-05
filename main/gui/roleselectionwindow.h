#ifndef ROLESELECTIONWINDOW_H
#define ROLESELECTIONWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class RoleSelectionWindow;
}
QT_END_NAMESPACE

class RoleSelectionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RoleSelectionWindow(QWidget *parent = nullptr);
    ~RoleSelectionWindow() override;

private slots:
    void on_customerButton_clicked();

    void on_ownerButton_clicked();

    void on_adminButton_clicked();

private:
    Ui::RoleSelectionWindow *ui;
};
#endif // ROLESELECTIONWINDOW_H
