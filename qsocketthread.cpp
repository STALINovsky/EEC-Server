#include "qsocketthread.h"

ServerSocket::ServerSocket(int descriptror, QObject *parent) : QObject(parent),socketDescriptor(descriptror)
{
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(descriptror);
    connect(socket,SIGNAL(disconnected()),this,SLOT(onDisconnected()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(onReadyRead()),Qt::DirectConnection);
}

ServerSocket::~ServerSocket()
{
    //Удаление объекта сокета
    delete socket;
}

void ServerSocket::onReadyRead()
{
    QDataStream in(socket);
    QString command;
    while (socket->bytesAvailable() < sizeof (int))
        socket->waitForReadyRead();

    int metaDataSize;
    in >> metaDataSize;
    while (socket->bytesAvailable() < metaDataSize)
        socket->waitForReadyRead();
    in >> command;
    qDebug() << "I got a command" << command;
    emit commandCame(command);
}

QString ServerSocket::loadMessage()
{
    QDataStream in(socket);
    QString message;
    in >> message;
    return message;
}

void ServerSocket::sendFilesToClient(QString command,QStringList filePaths)
{
    QDataStream out(socket);
    QDataStream in (socket);

    int metaDataSize = 2*(command.length() + 1) + sizeof (int);
    out << metaDataSize  << command << (int)filePaths.size();
    foreach(QString filepath,filePaths){
        QFile file(filepath);
        file.open(QFile::ReadOnly);
        QByteArray fileArr = file.readAll();
        file.close();

        QByteArray myHash = QCryptographicHash::hash(fileArr,QCryptographicHash::Md5);
        int metaDataSize = int(2*(filepath.size() + 1) + myHash.size());
        out << metaDataSize << myHash << filepath; /*<< fileArr;*/

        while(myHash.length() > socket->bytesAvailable())
        {
            socket->waitForReadyRead();
        }

        QByteArray clientHash;
        in >> clientHash;
        if(clientHash != myHash)
        {
            out << fileArr;
        }
    }
}

void ServerSocket::sendMessageToClient(QString command,QString message)
{
    QDataStream out(socket);
    int messageSize = 2 * (command.length() + message.length() + 2);
    out << messageSize << command << message;
}

QStringList ServerSocket::getAllFilePaths(QString dirPath)
{
    QStringList filePaths;
    QDir dir(dirPath);
    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDot|QDir::NoDotDot);
    QFileInfoList info = dir.entryInfoList();
    for (auto it = info.begin(); it != info.end();++it)
    {
           if(it->isDir())
           {
               filePaths << getAllFilePaths(it->path()+"/" + it->fileName());
           }
           else {
               filePaths << it->path() + "/" + it->fileName();
           }
    }
    return  filePaths;
}

void ServerSocket::onDisconnected()
{
    //Закрытие сокета
    emit disconnected();
    socket->close();
    this->deleteLater();
}


