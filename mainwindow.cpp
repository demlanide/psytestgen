#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "auth_window.h"
#include <QtDebug>
#include <QList>
#include <iostream>
#include <QCoreApplication>
#include <QtCore/QtCore>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_Main(new Ui::MainWindow)
{
    m_loginSuccesfull = false;
    connect(&ui_Auth, SIGNAL(login_button_clicked()), this, SLOT(authorizeUser()));
    connect(&ui_Auth,SIGNAL(destroyed()), this, SLOT(show()));
    connect(&ui_Admin, SIGNAL(add_user_button_clicked()), this, SLOT(adduser()));
    connect(&ui_Admin, SIGNAL(appoint_button_clicked()), this, SLOT(appoint()));
    connect(&ui_Admin, SIGNAL(show_button_clicked()), this, SLOT(show()));
    connect(&ui_User, SIGNAL(showdatasignal()), this, SLOT(showdataslot()));
    connect(&ui_User, SIGNAL(changePassSignal()), this, SLOT(changePassSlot()));
    connect(&ui_User, SIGNAL(showResultsSignal()), this, SLOT(showResultsSlot()));
    connect(&ui_Admin, SIGNAL(userChooseResultsSignal()), this, SLOT(userChooseResultsSlot()));
    connect(&ui_Admin, SIGNAL(testAssignSignal()), this, SLOT(testAssignSlot()));
    connect(&ui_User, SIGNAL(startTestSignal()), this, SLOT(startTestSlot()));

    if(!connectDB())
    {
        qDebug() << "Failed to connect DB";
    }

    QSqlQuery query;

    ui_Main->setupUi(this);
}
void MainWindow::authorizeUser() // скрипт авторизации пользователя
{
    m_username          = ui_Auth.getLogin();
    m_userpass          = ui_Auth.getPass();

    QString str_t       = " SELECT * "
                          " FROM users "
                          " WHERE login = '%1'";
    //int db_number       = 0;

    QString username    = "";

    QString userpass    = "";

    QString status    = "";

    db_input    = str_t.arg(m_username);

    QSqlQuery query;

    QSqlRecord rec;

    if(!query.exec(db_input))
    {
        qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
    }
    rec = query.record();
    query.next();
    username    = query.value(rec.indexOf("login")).toString();
    userpass    = query.value(rec.indexOf("password")).toString();
    status    = query.value(rec.indexOf("status")).toString();
    if(m_username != username || m_userpass != userpass)
    {
        QMessageBox msgBox;
        msgBox.setText("Неверный логин/пароль");
        msgBox.exec();
        m_loginSuccesfull = false;
    }
    else
    {
        m_loginSuccesfull = true;
        ui_Auth.close();
        if (status == "admin") {
            ui_Admin.show();
        } else if (status == "user") {
            ui_User.show();
        } else {
            this->show();
        }
    }
}

void MainWindow::display()                                                              //реализация пользотвальского метода отображения главного окна
{

    ui_Auth.show();                                                                     //отобразить окно авторизации(НЕ главное окно)
}

MainWindow::~MainWindow()
{
    delete ui_Main;
}

bool MainWindow::connectDB() // скрипт подсоединения к БД
{
    mw_db = QSqlDatabase::addDatabase("QSQLITE");
    mw_db.setDatabaseName(pathw + "/sources/gendb.db");
    if(!mw_db.open())
    {
        qDebug() << "Cannot open database: " << mw_db.lastError();
        return false;
    }
    return true;
}

void MainWindow::wipeDB() // закрытие БД
{
    mw_db.removeDatabase("generator");
}

void MainWindow::adduser() // скрипт добавления нового пользователя в кабинете админа
{
    addUserName = ui_Admin.getAddUsername();
    addUserStatus = ui_Admin.getAddUserstatus();
    addUserLogin = ui_Admin.getAddUserlogin();
    addUserPassword = ui_Admin.getAddUserpassword();
    QString status = "";
    QMessageBox msgBox;

    if (addUserStatus == 0)
    {
        status = "admin";
    } else {
        status = "user";
    }

    QSqlQuery query;
    query.exec("SELECT login FROM users");
    int n = 0;
    while(query.next())
    {
        if (addUserLogin == query.value(0).toString())
            n = 1;
    }
    if (n == 0)
    {
        if (addUserName.length() == 0 || addUserPassword.length() == 0 || addUserLogin.length() == 0)
        {
            msgBox.setText("Заполните все поля!");
            msgBox.exec();
        }
        else
        {
            QString str_t = "INSERT INTO users (name, status, login, password) VALUES ('%1', '%2', '%3', '%4')";

            db_input = str_t.arg(addUserName).arg(status).arg(addUserLogin).arg(addUserPassword);

            if(!query.exec(db_input))
            {
                qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
                msgBox.setText("Не удалось");
                msgBox.exec();
            }
            else {
                msgBox.setText("Профиль создан");
                msgBox.exec();
                ui_Admin.hideReg();
            }
        }
    }
    else
    {
        msgBox.setText("Пользователь с таким логином существует");
        msgBox.exec();
    }
}

void MainWindow::appoint() // скрипт назначения тестирования в кабинете админа
{
    QString str_t = "SELECT name FROM users WHERE status == 'user'";

    db_input = str_t;

    QSqlQuery query;

    QSqlRecord rec;
    QStringList lst;

    if(!query.exec(db_input))
    {
        qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
    }

    while (query.next())
    {
        ui_Admin.addNameCombo(query.value(0).toString());
    }

    str_t = "SELECT name FROM traits";

    db_input = str_t;

    if(!query.exec(db_input))
    {
        qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
    }

    while (query.next())
    {
        ui_Admin.addTests(query.value(0).toString());
    }

    QStringList chosen_tests;


}

void MainWindow::show() // скрипт заполнения выпадающего списка именами пользователей для формы просмотра результатов
{
    QString str_t = "SELECT name FROM users WHERE status == 'user'";

    db_input = str_t;

    QSqlQuery query;

    QSqlRecord rec;
    QStringList lst;

    if(!query.exec(db_input))
    {
        qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
    }

    while (query.next())
    {
        ui_Admin.addNameCombo1(query.value(0).toString());
    }
}

void MainWindow::showdataslot() // показ данных пользователю
{
    QString str_t       = " SELECT * "
                          " FROM users "
                          " WHERE login = '%1'";
    //int db_number       = 0;

    QString username    = "";

    QString userpass    = "";

    QString name    = "";

    db_input    = str_t.arg(m_username);

    QSqlQuery query;

    QSqlRecord rec;

    if(!query.exec(db_input))
    {
        qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
    }
    rec = query.record();
    query.next();
    username    = query.value(rec.indexOf("login")).toString();
    userpass    = query.value(rec.indexOf("password")).toString();
    name    = query.value(rec.indexOf("name")).toString();

    ui_User.setlogin(username);
    ui_User.setname(name);
    ui_User.setpassword(userpass);
}

void MainWindow::changePassSlot() // изменение пароля пользователем
{
    QString str_t = "UPDATE users SET password = '%1' WHERE login = '%2'";
    QMessageBox msgBox;
    QString changepass = ui_User.getchange_pass();

    if (changepass.length() == 0)
    {
        msgBox.setText("Поле пароля не заполнено");
        msgBox.exec();
    }
    else
    {
        db_input = str_t.arg(changepass).arg(m_username);

        QSqlQuery query;

        if(!query.exec(db_input))
        {
            qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
        }
        else
        {
            msgBox.setText("Ваш пароль изменен");
            msgBox.exec();
        }
    }
}

void MainWindow::showResultsSlot() // показ своих результатов пользователю
{
    QString str_t = " SELECT re.text FROM users u JOIN results r "
                    "ON u.id_user = r.id_user JOIN results_encrypt re "
                    "ON r.id_trait = re.id_trait AND (r.result >= re.min_value AND r.result <= re.max_value) "
                    "WHERE login = '%1'";

    QSqlQuery query;

    QString res = "";

    db_input = str_t.arg(m_username);

    if(!query.exec(db_input))
    {
        qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
    }

    while (query.next())
    {
        res = res + query.value(0).toString() + '\n';
    }
    ui_User.setResults(res);
}

void MainWindow::userChooseResultsSlot()
{
    QString loginChoose = ui_Admin.getUserChooseResults();

    QString str_t = " SELECT re.text FROM users u JOIN results r "
                    "ON u.id_user = r.id_user JOIN results_encrypt re "
                    "ON r.id_trait = re.id_trait AND (r.result >= re.min_value AND r.result <= re.max_value) "
                    "WHERE name = '%1'";

    QSqlQuery query;

    QString res = "";

    db_input = str_t.arg(ui_Admin.getUserChooseResults());

    if(!query.exec(db_input))
    {
        qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
    }

    while (query.next())
    {
        res = res + query.value(0).toString() + '\n';
    }
    ui_Admin.setUserResults(res);
}

void MainWindow::testAssignSlot()
{
    QString name = ui_Admin.getUserChoose();
    QStringList lst = ui_Admin.getTestChosen();

    QSqlQuery query;
    QString str_t;

    for (int i=0; i<lst.count(); i++)
    {
//        str_t = "INSERT INTO results "
//                "("
//                "id_user, "
//                "id_trait"
//                ")"
//                "SELECT * FROM "
//                "("
//                "SELECT"
//                "(SELECT id_user FROM users WHERE name = '%1') AS id_user, "
//                "(SELECT id_trait FROM traits WHERE name = '%2')) AS id_trait "
//                "WHERE (SELECT result FROM results "
//                "WHERE id_user = (SELECT id_user FROM users WHERE name = '%1') AND "
//                "id_trait = (SELECT id_trait FROM traits WHERE name = '%2')) == ( 999 OR NULL)"
//                "OR NOT EXISTS "
//                "("
//                "SELECT * "
//                "FROM results "
//                "WHERE "
//                "id_user = (SELECT id_user FROM users WHERE name = '%1') AND "
//                "id_trait = (SELECT id_trait FROM traits WHERE name = '%2')"
//                ")";
//        db_input = str_t.arg(name).arg(lst[i]);
        str_t = "SELECT id_user FROM users WHERE name = '%1'";
        db_input = str_t.arg(name);
        query.exec(db_input);
        query.next();
        QString iduser = query.value(0).toString();
        str_t = "SELECT id_trait FROM traits WHERE name = '%1'";
        db_input = str_t.arg(lst[i]);
        query.exec(db_input);
        query.next();
        QString idtrait = query.value(0).toString();
        str_t = "SELECT count(*) FROM results WHERE id_user = "
                "'%1' AND id_trait = '%2'";
        db_input = str_t.arg(iduser).arg(idtrait);
        if(!query.exec(db_input))
        {
            qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
        }
        query.next();
        int exists = query.value(0).toInt();
        if (exists == 0)
        {
            str_t = "INSERT INTO results (id_user, id_trait) "
                    "VALUES ('%1', '%2')";
            db_input = str_t.arg(iduser).arg(idtrait);
            query.exec(db_input);
        }
        else
        {
            str_t = "UPDATE results SET result = NULL WHERE "
                    "id_user = '%1' AND id_trait = '%2'";
            db_input = str_t.arg(iduser).arg(idtrait);
            query.exec(db_input);
        }
    }

    QMessageBox msgBox;
    msgBox.setText("Тесты назначены");
    msgBox.exec();
    ui_Admin.hideTestChoose();

//    system("/bin/bash -c \"$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)\"");
//    system("brew install tree");
//    system("apt get install pip");
//    system("pip install openpyxl");

//    QString program_name = pathw + "/sources/python/genscript/gen.py";
//    QString cmd = "python " + program_name;
//    system(qPrintable(cmd));

//    program_name = pathw + "/sources/python/genscript/create_lists.py";
//    QString strt = "SELECT id_user FROM users WHERE name = '%1'";
//    QString db_input = strt.arg(name);
//    if(!query.exec(db_input))
//    {
//        qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
//    }
//    query.next();
//    QString iduser = query.value(0).toString();
//    cmd = "python " + program_name + " " + iduser;
//    system(qPrintable(cmd));

//    QString program_name = pathw + "/sources/python/genscript/gen.py";
//    QStringList arguments { program_name};
//    QProcess p;
//    p.start("python3", arguments);
//    qDebug() << "Let's Go";
//    Monitor m;

//    QObject::connect(&p, SIGNAL(finished(int,QProcess::ExitStatus)), &m, SLOT(finished(int,QProcess::ExitStatus)));
//    QObject::connect(&p, SIGNAL(readyReadStandardOutput()), &m, SLOT(readyReadStandardOutput()));
//    QObject::connect(&p, SIGNAL(started()), &m, SLOT(started()));
//    p.waitForFinished();

//    program_name = pathw + "/sources/python/genscript/create_lists.py";
//    QString strt = "SELECT id_user FROM users WHERE name = '%1'";
//    QString db_input = strt.arg(name);
//    if(!query.exec(db_input))
//    {
//        qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
//    }
//    query.next();
//    QString iduser = query.value(0).toString();
//    QStringList arguments1 { program_name, iduser};
//    p.start("python3", arguments1);
//    qDebug() << "Let's Go";

//    QObject::connect(&p, SIGNAL(finished(int,QProcess::ExitStatus)), &m, SLOT(finished(int,QProcess::ExitStatus)));
//    QObject::connect(&p, SIGNAL(readyReadStandardOutput()), &m, SLOT(readyReadStandardOutput()));
//    QObject::connect(&p, SIGNAL(started()), &m, SLOT(started()));
//    p.waitForFinished();
}

void MainWindow::startTestSlot()
{
    QString str_t = "SELECT count(*) FROM results WHERE id_user = "
                    "(SELECT id_user FROM users WHERE login = '%1') AND result is NULL";
    db_input = str_t.arg(m_username);
    QSqlQuery query;
    QSqlRecord rec;
    QMessageBox msgBox;
    QString str_x;
    QXmlQuery xquery;

    QString question_text;
    QString answers_num;
    QString x;
    QString answer_text;
    QString points_str;
//    float points;
//    float result;
    QMap<QString, float> answers;

    if(!query.exec(db_input))
    {
        qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
    }
    rec = query.record();
    query.next();
    int n = query.value(rec.indexOf("count(*)")).toInt();
    if (n == 0)
    {
        msgBox.setText("Нет назначенных тестов. Обратитесь к администратору для назначения.");
        msgBox.exec();
    }
    else
    {
        str_t = "SELECT id_trait FROM results WHERE id_user = "
                "(SELECT id_user FROM users WHERE login = '%1') AND result is NULL";
        db_input = str_t.arg(m_username);

        if(!query.exec(db_input))
        {
            qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
        }

        QList<QString> lst; // занос id тестов которые нужно пройти в список
        while (query.next())
        {
            lst.append(query.value(0).toString());
        }

//        QFile db(QDir::homePath() + "/Desktop/diploma/generator/sources/tests.xml");
//            if ( ! db.exists()) {
//                msgBox.setText("Невозможно подключиться к тестам");
//                msgBox.exec();
//            }

//            if (!db.open(QIODevice::ReadOnly | QIODevice::Text))
//                return;
//        else
//        {
//            xquery.setFocus(&db);
//            for (int i = 0; i < lst.size(); i++)
//            {
//                str_t = "SELECT questions_num FROM traits WHERE id_trait = '%1'";
//                db_input = str_t.arg(lst[i]);
//                if(!query.exec(db_input))
//                {
//                    qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
//                }
//                query.next();
//                n = query.value(0).toInt();
//                result = 0;
//                int j = 1;
//                str_x = "/tests/test[@id='%1']/question[@num_q='%2']/question_text/text()";
//                xdb_input = str_x.arg(lst[i]).arg(j);
//                xquery.setQuery(xdb_input);
//                xquery.evaluateTo(&question_text);

//                str_x = "/tests/test[@id='%1']/question[@num_q='%2']/answers_number/text()";
//                xdb_input = str_x.arg(lst[i]).arg(j);
//                xquery.setQuery(xdb_input);
//                xquery.evaluateTo(&answers_num);

//                int w = answers_num.toInt();
//                for (int k = 1; k <= w; k++)
//                {
//                    str_x = "/tests/test[@id='%1']/question[@num_q='%2']/answers/answer[@num_a='%3']/answer_text/text()";
//                    xdb_input = str_x.arg(lst[i]).arg(j).arg(k);
//                    xquery.setQuery(xdb_input);
//                    xquery.evaluateTo(&answer_text);

//                    str_x = "/tests/test[@id='%1']/question[@num_q='%2']/answers/answer[@num_a='%3']/points/text()";
//                    xdb_input = str_x.arg(lst[i]).arg(j).arg(k);
//                    xquery.setQuery(xdb_input);
//                    xquery.evaluateTo(&points_str);
//                    points = points_str.toFloat();

//                    answers[answer_text] = points;
//                }
                ui_quest.setFirst(lst, m_username, 0);
                ui_quest.show();
//                question_form* form = new question_form();
//                form->show();
//                str_t = "UPDATE results SET result = '%1' WHERE id_user = (SELECT "
//                        "id_user FROM users WHERE login = '%2') AND id_trait = '%3'";
//                db_input = str_t.arg(result).arg(m_username).arg(lst[i]);
//                if(!query.exec(db_input))
//                {
//                    qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
//                }
//            }
//        }
    }
}

//void MainWindow::pushResults(float n, QString tst)
//{
//    QSqlQuery query;
//    QString str_t = "UPDATE results SET result = '%1' WHERE id_user = (SELECT "
//            "id_user FROM users WHERE login = '%2') AND id_trait = '%3'";
//    QString db_input = str_t.arg(n).arg(m_username).arg(tst);
//    if(!query.exec(db_input))
//    {
//        qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
//    }
//}
