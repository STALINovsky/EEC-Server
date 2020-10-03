#ifndef QSOCKETTHREAD_H
#define QSOCKETTHREAD_H

//Подсоединение используемых в коде классов
#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QDataStream>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QCryptographicHash>

//Пользовательский класс потока
class ServerSocket : public QObject
{
    Q_OBJECT
signals:
    void disconnected();
    void commandCame(QString command);
    void messageLoaded(QString command,QString message);
public:
    explicit ServerSocket(int descriptror, QObject *parent = 0);
    ~ServerSocket();

public slots:
    //Слот обмена данными с сокетом
    void onReadyRead();    
    QString loadMessage();
    void sendFilesToClient(QString command,QStringList filePaths);
    void sendMessageToClient(QString command,QString message);
    //Слот закрытия сокета
    void onDisconnected();

protected:
    QStringList getAllFilePaths(QString dirPath);
    int socketDescriptor;
    QTcpSocket* socket;
};

#endif // QSOCKETTHREAD_H
