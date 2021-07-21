#ifndef ADMIN_ROOM_H
#define ADMIN_ROOM_H

#include <QWidget>

namespace Ui {
class admin_room;
}

class admin_room : public QWidget
{
    Q_OBJECT

public:
    explicit admin_room(QWidget *parent = nullptr);
    ~admin_room();
    QString getAddUsername();
    int getAddUserstatus();
    QString getAddUserlogin();
    QString getAddUserpassword();
    QString getUserChoose();
    void addNameCombo(QString);
    void addNameCombo1(QString);
    void addTests(QString);
    QString getUserChooseResults();
    void setUserResults(QString &arg1);
    QStringList getTestChosen();
    void hideTestChoose();
    void hideReg();

signals:
    void add_user_button_clicked();
    void appoint_button_clicked();
    void show_button_clicked();
    void userChooseResultsSignal();
    void testAssignSignal();

private slots:
    void on_appointPushButton_clicked();

    void on_addPushButton_clicked();

    void on_showPushButton_clicked();

    void on_addUserPushButton_clicked();

    void on_nameLineEdit_textEdited(const QString &arg1);

    void on_statusComboBox_activated(int index);

    void on_loginLineEdit_textEdited(const QString &arg1);

    void on_passwordLineEdit_textEdited(const QString &arg1);

    void on_userChooseComboBox_activated(const QString &arg1);

    void on_resultChooseComboBox_textActivated(const QString &arg1);

    void on_assignPushButton_clicked();

private:
    Ui::admin_room *ui;
    QString addUserName;
    int addUserStatus;
    QString addUserLogin;
    QString addUserPassword;
    QString userChoose;
    QString userResultsChoose;
    QStringList testChoose;
};

#endif // ADMIN_ROOM_H
