#ifndef USER_ROOM_H
#define USER_ROOM_H

#include <QWidget>
#include <QMessageBox>
#include <QtSql/QtSql>
#include <QFile>
#include <QXmlStreamReader>
#include <QMap>
#include <QDomDocument>
#include <QXmlQuery>
#include <QThread>

//#include "question_form.h"
//#include "ui_question_form.h"
#include "auth_window.h"

namespace Ui {
class user_room;
}

class user_room : public QWidget
{
    Q_OBJECT

public:
    explicit user_room(QWidget *parent = nullptr);
    ~user_room();
    QString getchange_pass();
    void setname(QString &arg1);

    void setlogin(QString &arg1);

    void setpassword(QString &arg1);

    void setResults(QString &arg1);

    bool connectDB();

signals:
    void showdatasignal();
    void changePassSignal();
    void showResultsSignal();
    void startTestSignal();

private slots:
    void on_testingButton_clicked();

    void on_changedataButton_clicked();

    void on_showresButton_clicked();

    void on_pushButton_4_clicked();

    void on_passwordChangeLineedit_textEdited(const QString &arg1);

private:
    Ui::user_room *ui;
//    Ui::question_form *ui_quest;
    auth_window ui_Auth;
    QString changePass;

    QString username;
    QSqlDatabase mw_db;
    QString db_input;               // строка для отправки запроса к БД
    QString xdb_input;
};

#endif // USER_ROOM_H
