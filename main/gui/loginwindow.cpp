#include "loginwindow.h"
#include "ui_loginwindow.h"

#include <QDebug>
#include <QMessageBox>

#include "../Login_and_SignUp.h"
#include "../SecretUserForAdmin.h"

#include "customerwindow.h"
#include "restaurantowner.h"


LoginWindow::LoginWindow(QString role , QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    isLoginMode = true;

    currentRole = role;

    updateUI();
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::updateUI()
{
    if(isLoginMode){

        ui->confirmPasswordLabel->hide();
        ui->confirmPasswordInput->hide();
        ui->loginButton->setText("Login");
        ui->promptLabel->setText("Don't have an account?");
        ui->registerButton->setText("Register");

    }

    else{
        ui->confirmPasswordLabel->show();
        ui->confirmPasswordInput->show();
        ui->loginButton->setText("Register Now");
        ui->promptLabel->setText("Already have an account?");
        ui->registerButton->setText("Login");
    }

}

void LoginWindow::on_registerButton_clicked()
{
    isLoginMode = !isLoginMode;

    updateUI();
}

void LoginWindow::on_loginButton_clicked()
{
    QString qUsername = ui->usernameInput->text();
    QString qPassword = ui->passwordInput->text();

    if(qUsername.isEmpty() || qPassword.isEmpty()){
        QMessageBox::warning(this , "Error" , "please fill in all fields");
        return;
    }

    string username = qUsername.toStdString();
    string password = qPassword.toStdString();

    string responseMessage = "";

    DataBase dbmain;
    LOGINDAO dbaslog(dbmain);
    dbaslog.CreateLOGINTable();

    if(isLoginMode)
    {
        if(currentRole.toStdString() == "Admin" ){
            const vector<unsigned char> username_bytes = { 41, 46, 44, 40, 43, 38, 51, 42, 52, 43, 38 };
            const vector<unsigned char> password_bytes = { 39, 38, 42, 46, 43, 110, 108, 103, 114 };

            if(username != get_hidden_string(username_bytes) || password != get_hidden_string(password_bytes)){

                QMessageBox::critical(this, "Error", QString::fromStdString("Wrong UserName/Password!"));
            }

            else{

                QMessageBox::information(this, "Success", QString::fromStdString("Welcome Admin!!!!!!!"));

                // Move to next pages
            }
        }

        else{
            bool success = dbaslog.LoginUser(username, password, responseMessage);
            if(!success)
                QMessageBox::critical(this, "Error", QString::fromStdString(responseMessage));

            else{

                if(currentRole.toStdString() == "Customer"){

                    customerwindow *CustomerPage = new customerwindow(QString::fromStdString(username));
                    CustomerPage->show();
                    this->close();

                }

                else{
                    RestaurantOwner *RestaurantOwnerPage = new RestaurantOwner(QString::fromStdString(username));
                    RestaurantOwnerPage->show();
                    this->close();
                }


            }
        }
    }

    else
    {
        QString qconfirmPassword = ui->confirmPasswordInput->text();

        string Role = currentRole.toStdString();
        if(Role == "Admin")
            QMessageBox::critical(this, "Error", QString::fromStdString("Admin cannot register."));

        else if(qconfirmPassword != qPassword){
           QMessageBox::critical(this, "Error", QString::fromStdString("Please make sure your passwords match."));
        }
        else{
            bool registered = dbaslog.RegisterUser(username, password, Role , responseMessage);

            if (registered) {
                QMessageBox::information(this, "Success", QString::fromStdString(responseMessage));
                isLoginMode = true;
                updateUI();
            }

            else
                QMessageBox::warning(this, "Error", QString::fromStdString(responseMessage));

        }
    }

}
