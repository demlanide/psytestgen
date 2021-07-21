#ifndef QUESTION_FORM_H
#define QUESTION_FORM_H

#include <QWidget>
#include <QMessageBox>
#include <QtSql/QtSql>
#include <QFile>
#include <QXmlStreamReader>
#include <QMap>
#include <QDomDocument>
#include <QXmlQuery>
#include <QThread>
//#include "mainwindow.h"
#include "auth_window.h"

namespace Ui {
class question_form;
}

class question_form : public QWidget
{
    Q_OBJECT

public:
    explicit question_form(QWidget *parent = nullptr);
    ~question_form();
    void setFirst(QList<QString>, QString, int);
    bool connectDB();

private slots:
    void on_goPushButton_clicked();

private:
    Ui::question_form *ui;
    QSqlDatabase mw_db;
    QString db_input;               // строка для отправки запроса к БД
    QString xdb_input;
    QMap<QString, float> answers;
    QList<QString> testsList;
    float result;
    int i = 0;
    int j = 1;
    QString user_name;
    QString pathw = "/Users/dameli/Desktop/diploma/generator";
};

#endif // QUESTION_FORM_H
