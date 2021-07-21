#include "user_room.h"
#include "ui_user_room.h"

user_room::user_room(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::user_room)
{
    if(!connectDB())
    {
        qDebug() << "Failed to connect DB";
    }

    ui->setupUi(this);
    ui->changedata->setVisible(false);
    ui->resultWidget->setVisible(false);
}

user_room::~user_room()
{
    delete ui;
}

void user_room::on_changedataButton_clicked() // кнопка изменения данных (пароля)
{
    ui->changedata->setVisible(true);
    ui->resultWidget->setVisible(false);
    emit showdatasignal();
}

void user_room::on_showresButton_clicked() // кнопка показа результатов
{
    ui->changedata->setVisible(false);
    ui->resultWidget->setVisible(true);
    emit showResultsSignal();
}

bool user_room::connectDB() // скрипт подсоединения к БД
{
    mw_db = QSqlDatabase::addDatabase("QSQLITE");
    mw_db.setDatabaseName("/Users/dameli/Desktop/diploma/generator/sources/gendb.db");
    if(!mw_db.open())
    {
        qDebug() << "Cannot open database: " << mw_db.lastError();
        return false;
    }
    return true;
}

void user_room::on_testingButton_clicked() // кнопка начала тестирования
{
    emit startTestSignal();
}

void user_room::on_pushButton_4_clicked() // кнопка изменить пароль в форме изменения данных (пароля)
{
    emit changePassSignal();
}

void user_room::on_passwordChangeLineedit_textEdited(const QString &arg1) // изменение строки содержащей пароль
{
    user_room::changePass = arg1;
}

QString user_room::getchange_pass() // передача измененного пароля в программу
{
    return user_room::changePass;
}

void user_room::setname(QString &arg1) // установка имени из бд
{
    ui->sql_name_label->setText(arg1);
}

void user_room::setlogin(QString &arg1) // установка логина из бд
{
    ui->sql_login_label->setText(arg1);
}

void user_room::setpassword(QString &arg1) // установка пароля из бд
{
    ui->passwordChangeLineedit->setText(arg1);
}

void user_room::setResults(QString &arg1) // установка результатов в лейбл показа результатов
{
    ui->resultsShowLabel->setText(arg1);
}
