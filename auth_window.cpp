#include "auth_window.h"
#include "ui_auth_window.h"

auth_window::auth_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::auth_window)
{
    ui->setupUi(this);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);

}

auth_window::~auth_window()
{
    delete ui;
}

void auth_window::on_lineEdit_textEdited(const QString &arg1) // передача логина в форме аутентификации
{
    auth_window::m_username = arg1;
}

void auth_window::on_lineEdit_2_textEdited(const QString &arg1) // передача пароля в форме аутентификации
{
    auth_window::m_userpass = arg1;
}

void auth_window::on_loginPushButton_clicked() // вызов сигнала при попытке авторизоваться
{
    emit login_button_clicked();
}

QString auth_window::getLogin() // запись логина в переменную
{
    return auth_window::m_username;
}

QString auth_window::getPass() // запись пароля в переменную
{
    return auth_window::m_userpass;
}
