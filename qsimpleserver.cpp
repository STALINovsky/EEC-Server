#include "qsimpleserver.h"

QSimpleServer::QSimpleServer(QObject *parent) :
    QTcpServer(parent)
{
    //Включает прослушивание сервером 80 порта
    if(listen(QHostAddress::Any, 80)){
        qDebug() << "Listening...";
    }
    else {
        qDebug() << "Error while starting: " << errorString();
    }
}

void QSimpleServer::incomingConnection(qintptr handle)
{
    //Создание объекта потока
    RequestHandler* handler = new RequestHandler(handle);
    QThread *thread = new QThread(this);
    handler->moveToThread(thread);
    thread->start();

    //Соединение сигнала завершения потока с слотом отложенного удаления
    QObject::connect(handler, SIGNAL(disconnected()), thread, SLOT(quit()));
    //Запуск потока
}
