#include "mainwindow.h"
#include <QApplication>
#include "auth_window.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;                                   //создание главного окна
    if(!w.connectDB())
        exit(1);
    //w.wipeDB();
    w.display();                                    //пользовательская функция вызова главного окна

    return a.exec();
}
