#include "question_form.h"
#include "ui_question_form.h"

question_form::question_form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::question_form)
{
    ui->setupUi(this);
}

bool question_form::connectDB() // скрипт подсоединения к БД
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

question_form::~question_form()
{
    delete ui;
}

void question_form::on_goPushButton_clicked()
{
    QMessageBox msgBox;
    QSqlQuery query;
    QSqlRecord rec;
    QString str_x;
    QXmlQuery xquery;

    QString question_text;
    QString answers_num;
    QString x;
    QString answer_text;
    QString points_str;
    float points;
    QString check = ui->answersListWidget->currentItem()->text();
    if (check.length() == 0)
    {
        msgBox.setText("Выберите вариант ответа!");
        msgBox.exec();
    }
    else
        result += answers[ui->answersListWidget->currentItem()->text()];
    QString str_t = "SELECT questions_num FROM traits WHERE id_trait = '%1'";
    db_input = str_t.arg(testsList[i]);
    if(!query.exec(db_input))
    {
        qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
    }
    query.next();
    int n = query.value(0).toInt();
    if (j < n)
    {
        j++;
        QFile db(pathw + "/sources/tests.xml");
        if ( ! db.exists()) {
            msgBox.setText("Невозможно подключиться к тестам");
            msgBox.exec();
        }
        db.open(QIODevice::ReadOnly | QIODevice::Text);
        xquery.setFocus(&db);
        str_x = "/tests/test[@id='%1']/question[@num_q='%2']/question_text/text()";
        xdb_input = str_x.arg(testsList[i]).arg(j);
        xquery.setQuery(xdb_input);
        xquery.evaluateTo(&question_text);

        str_x = "/tests/test[@id='%1']/question[@num_q='%2']/answers_number/text()";
        xdb_input = str_x.arg(testsList[i]).arg(j);
        xquery.setQuery(xdb_input);
        xquery.evaluateTo(&answers_num);

        int w = answers_num.toInt();
        answers.clear();
        for (int k = 1; k <= w; k++)
        {
            str_x = "/tests/test[@id='%1']/question[@num_q='%2']/answers/answer[@num_a='%3']/answer_text/text()";
            xdb_input = str_x.arg(testsList[i]).arg(j).arg(k);
            xquery.setQuery(xdb_input);
            xquery.evaluateTo(&answer_text);

            str_x = "/tests/test[@id='%1']/question[@num_q='%2']/answers/answer[@num_a='%3']/points/text()";
            xdb_input = str_x.arg(testsList[i]).arg(j).arg(k);
            xquery.setQuery(xdb_input);
            xquery.evaluateTo(&points_str);
            points = points_str.toFloat();

            answers[answer_text] = points;
        }
        ui->questionLabel->setText(question_text);
        ui->answersListWidget->clear();
        foreach (QString key, answers.keys())
            ui->answersListWidget->addItem(key);
    }
    else
    {
        QString str_t = "UPDATE results SET result = '%1' WHERE id_user = (SELECT "
                "id_user FROM users WHERE login = '%2') AND id_trait = '%3'";
        QString db_input = str_t.arg(n).arg(user_name).arg(testsList[i]);
        if(!query.exec(db_input))
        {
            qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
        }
        if (i < testsList.length() - 1)
        {
            i++;
            j = 1;
            setFirst(testsList, user_name, i);
            result = 0;
        }
        else
        {
//            QString program_name = pathw + "/sources/python/genscript/calculate.py";
//            QString strt = "SELECT id_user FROM users WHERE login = '%1'";
//            QString db_input = strt.arg(user_name);
//            if(!query.exec(db_input))
//            {
//                qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
//            }
//            query.next();
//            QString iduser = query.value(0).toString();
//            QStringList arguments { program_name, iduser};
//            QProcess p;
//            p.start("python3", arguments);
//            p.waitForFinished();
            msgBox.setText("Вы прошли все назначенные тесты!");
            msgBox.exec();
            this->close();
        }
    }
}

void question_form::setFirst(QList<QString> lst, QString username, int index)
{
    QMessageBox msgBox;
    QSqlQuery query;
    QSqlRecord rec;
    QString str_x;
    QXmlQuery xquery;

    QString question_text;
    QString answers_num;
    QString x;
    QString answer_text;
    QString points_str;
    float points;

    i = index;
    testsList = lst;
    user_name = username;
    QFile db(pathw + "/sources/tests.xml");
    if ( ! db.exists()) {
        msgBox.setText("Невозможно подключиться к тестам");
        msgBox.exec();
    }

    if (!db.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    else
    {
        xquery.setFocus(&db);
        str_x = "/tests/test[@id='%1']/question[@num_q='%2']/question_text/text()";
        xdb_input = str_x.arg(lst[i]).arg(1);
        xquery.setQuery(xdb_input);
        xquery.evaluateTo(&question_text);

        str_x = "/tests/test[@id='%1']/question[@num_q='%2']/answers_number/text()";
        xdb_input = str_x.arg(lst[i]).arg(1);
        xquery.setQuery(xdb_input);
        xquery.evaluateTo(&answers_num);

        int w = answers_num.toInt();
        answers.clear();
        for (int k = 1; k <= w; k++)
        {
            str_x = "/tests/test[@id='%1']/question[@num_q='%2']/answers/answer[@num_a='%3']/answer_text/text()";
            xdb_input = str_x.arg(lst[i]).arg(1).arg(k);
            xquery.setQuery(xdb_input);
            xquery.evaluateTo(&answer_text);

            str_x = "/tests/test[@id='%1']/question[@num_q='%2']/answers/answer[@num_a='%3']/points/text()";
            xdb_input = str_x.arg(lst[i]).arg(1).arg(k);
            xquery.setQuery(xdb_input);
            xquery.evaluateTo(&points_str);
            points = points_str.toFloat();

            answers[answer_text] = points;
        }
        result = 0;
        ui->questionLabel->setText(question_text);
        ui->answersListWidget->clear();
        foreach (QString key, answers.keys())
            ui->answersListWidget->addItem(key);
        result = 0;
    }
}
