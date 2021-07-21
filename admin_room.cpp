#include "admin_room.h"
#include "ui_admin_room.h"

admin_room::admin_room(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::admin_room)
{
    ui->setupUi(this);
    ui->addUserWidget->setVisible(false);
    ui->assignWidget->setVisible(false);
    ui->resultWidget->setVisible(false);
}

admin_room::~admin_room()
{
    delete ui;
}

void admin_room::on_appointPushButton_clicked() //кнопка Назначить тесты
{
    ui->addUserWidget->setVisible(false);
    ui->resultWidget->setVisible(false);
    ui->assignWidget->setVisible(true);
    emit appoint_button_clicked();
}

void admin_room::on_addPushButton_clicked() //кнопка Добавить пользователя
{
    ui->addUserWidget->setVisible(true);
    ui->assignWidget->setVisible(false);
    ui->resultWidget->setVisible(false);
}

void admin_room::on_showPushButton_clicked() // кнопка Посмотреть результаты
{
    ui->addUserWidget->setVisible(false);
    ui->assignWidget->setVisible(false);
    ui->resultWidget->setVisible(true);
    emit show_button_clicked();
}

void admin_room::on_addUserPushButton_clicked() // кнопка Добавить в форме добавления пользователя
{
    emit add_user_button_clicked();
}

void admin_room::on_nameLineEdit_textEdited(const QString &arg1) // передача имени в форме добавления пользователя
{
    admin_room::addUserName = arg1;
}

void admin_room::on_statusComboBox_activated(int index) // передача статуса в форме добавления пользователя
{
    admin_room::addUserStatus = index;
}

void admin_room::on_loginLineEdit_textEdited(const QString &arg1) // передача логина в форме добавления пользователя
{
    admin_room::addUserLogin = arg1;
}

void admin_room::on_passwordLineEdit_textEdited(const QString &arg1) // передача пароля в форме добавления пользователя
{
    admin_room::addUserPassword = arg1;
}

QString admin_room::getAddUsername() // запись имени в переменную для добавления в БД
{
    return admin_room::addUserName;
}

int admin_room::getAddUserstatus() // запись статуса в переменную для добавления в БД
{
    return admin_room::addUserStatus;
}

QString admin_room::getAddUserlogin() // запись логина в переменную для добавления в БД
{
    return admin_room::addUserLogin;
}

QString admin_room::getAddUserpassword() // запись пароля в переменную для добавления в БД
{
    return admin_room::addUserPassword;
}

void admin_room::addNameCombo(QString str) // добавить имя в выпадающий список в форме назначения тестов
{
    ui->userChooseComboBox->addItem(str);
}

void admin_room::addNameCombo1(QString str) // добавить имя в выпадающий список в форме просмотра результатов
{
    ui->resultChooseComboBox->addItem(str);
}

void admin_room::addTests(QString str) // добавить тест в листвиджет в форме назначения тестирования
{
    ui->testChooseWidget->addItem(str);
}

void admin_room::on_userChooseComboBox_activated(const QString &arg1) // передача выбранного имени в форме назначения тестов
{
    admin_room::userChoose = arg1;
}

QString admin_room::getUserChoose() // запись выбранного имени в переменную в форме назначения тестирования
{
    return admin_room::userChoose;
}

QString admin_room::getUserChooseResults() // запись имени выбранного пользователя в переменную в форме просмотра результатов
{
    return admin_room::userResultsChoose;
}

void admin_room::setUserResults(QString &arg1) // показ результатов выбранного пользователя
{
    ui->result_label->setText(arg1);
}

void admin_room::on_resultChooseComboBox_textActivated(const QString &arg1) // выбор пользователя для просмотра результатов
{
    admin_room::userResultsChoose = arg1;
    emit userChooseResultsSignal();
}

void admin_room::on_assignPushButton_clicked()
{
    QList<QListWidgetItem *> list = ui->testChooseWidget->selectedItems ();
    QStringList tmp;
    for(int i=0; i<list.count(); i++)
      tmp << list[i]->text();
    admin_room::testChoose = tmp;
    emit testAssignSignal();
}

QStringList admin_room::getTestChosen()
{
    return admin_room::testChoose;
}

void admin_room::hideTestChoose()
{
    ui->assignWidget->setVisible(false);
}

void admin_room::hideReg()
{
    ui->addUserWidget->setVisible(false);
}

