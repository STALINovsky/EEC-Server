#include <QtCore/QCoreApplication>
#include "qsimpleserver.h"

int main(int argc, char *argv[])
{
    system("Chcp 1251");
    //Создание объекта консольного приложения
    QCoreApplication a(argc, argv);
    //Создание объекта сервера
    QSimpleServer server;
    //Запуск цикла обработки событий
    return a.exec();
}
