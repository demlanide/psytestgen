#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QtSql/QtSql>
#include "auth_window.h"
#include "admin_room.h"
#include "user_room.h"
#include "question_form.h"
#include "ui_question_form.h"
#include <QMessageBox>
#include <QFile>
#include <QXmlStreamReader>
#include <QMap>
#include <QDomDocument>
#include <QXmlQuery>
#include <QThread>
#include <QProcess>
#include "monitor.h"
#include <stdlib.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void display();                 // прототип пользовательской функции отображения
    bool connectDB();
    void wipeDB();
    void pushResults(float n, QString tst);
    QString m_username;

private:
    Ui::MainWindow *ui_Main;

    auth_window ui_Auth;
    admin_room ui_Admin;
    user_room ui_User;
    question_form ui_quest;

    //QString m_username;             // строки для обработки
    QString m_userpass;             // пользовательского ввода

    QString addUserName;
    int addUserStatus;
    QString addUserLogin;
    QString addUserPassword;

    QString db_input;               // строка для отправки запроса к БД
    QString xdb_input;

    QSqlDatabase mw_db;             // экземпляр подключения к БД

    bool m_loginSuccesfull;
    QString pathw = "/Users/dameli/Desktop/diploma/generator";

signals:
    void pushTheButton();

private slots:
    void authorizeUser();           // пользовательские слоты
    void adduser();
    void appoint();
    void show();
    void showdataslot();
    void changePassSlot();
    void showResultsSlot();
    void userChooseResultsSlot();
    void testAssignSlot();
    void startTestSlot();
};
#endif // MAINWINDOW_H
