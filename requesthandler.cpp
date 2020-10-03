#include "requesthandler.h"

RequestHandler::RequestHandler(int descriptor,QObject* parent) : ServerSocket(descriptor,parent)
{
    connect(this, SIGNAL(commandCame(QString)), this, SLOT(newCommand(QString)),Qt::DirectConnection);
}

void RequestHandler::newCommand(QString command)
{
    disconnect(socket,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    if(command.contains("Get test list",Qt::CaseSensitivity::CaseInsensitive))
    {
        this->sendTestList();
    }
    else if(command.contains("Get test",Qt::CaseSensitivity::CaseInsensitive))
    {
        QString testName = command.split('\n').at(1);
        sendTest(testName);
    }
    else if(command.contains("Get lessions",Qt::CaseSensitivity::CaseInsensitive))
    {
        sendLectures();
    }
    else if(command.contains("Get labs",Qt::CaseSensitivity::CaseInsensitive))
    {
        sendLabs();
    }
    else if (command.contains("Set answer",Qt::CaseSensitivity::CaseInsensitive))
    {
        QString answerData = loadMessage();
        processAnswerData(answerData);
    }
    else {
        qDebug() << "undefinded command" << command;
    }
    qDebug() << socket->bytesAvailable();
    connect(socket,SIGNAL(readyRead()),this,SLOT(onReadyRead()),Qt::DirectConnection);
    if(socket->bytesAvailable() > 0)
    {
        emit socket->readyRead();
    }
}

void RequestHandler::sendTestList()
{
    QDir dir("Tests");
    dir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    dir.setSorting(QDir::SortFlag::Time);
    QFileInfoList testsDirs = dir.entryInfoList();
    QStringList testNames;
    foreach(auto item,testsDirs)
    {
        testNames.append(item.fileName());
    }
    QString testList = testNames.join('\n');
    sendMessageToClient("Load test list",testList);
}

void RequestHandler::sendTest(QString testName)
{
    QString testPath = "Tests/" + testName;
    QStringList testPaths = getAllFilePaths(testPath);
    foreach (auto path, testPaths) {
        if(path.contains("Answers.txt"))
        {
            testPaths.removeOne(path);
        }
    }
    this->sendFilesToClient("Load test",testPaths);
}

void RequestHandler::sendLectures()
{
    QString lecturesPath = "Lessions";
    QStringList testPaths = getAllFilePaths(lecturesPath);
    this->sendFilesToClient("Load lessions",testPaths);
}

void RequestHandler::sendLabs()
{
    QString labsPath = "Labs";
    QStringList testPaths = getAllFilePaths(labsPath);
    this->sendFilesToClient("Load labs",testPaths);
}



void RequestHandler::processAnswerData(QString answerData)
{

    QTextStream in(&answerData);
    QString testName = in.readLine();

    QFile decisionFile("Tests/" + testName + "/AboutTest/Answers.txt");
    QString decisionMessage;
    if(decisionFile.open(QIODevice::ReadOnly))
    {
        decisionMessage = decisionFile.readAll();
    }
    else {
        qDebug() << "Answers File is not finded";
        QString answerMessage = "Error";
    }
    QString comand = "Decision";
    this->sendMessageToClient(comand,decisionMessage);
    safeAnswerData(answerData);
}

void RequestHandler::safeAnswerData(QString answerData)
{
    QTextStream in(&answerData);
    QString testName = in.readLine();
    QString surname = in.readLine();
    QString name = in.readLine();
    QString group = in.readLine();

    QStringList answers = in.readAll().split('\n');
    QDir dirMaker;
    dirMaker.mkpath(QStringList{"Tests Results",testName,group}.join("/"));

    QString fileName = QStringList{surname,name}.join(' ') + ".txt";
    QString filePath =  QStringList{"Tests Results",testName,group,fileName}.join("/");

    QFile file(filePath);
    if(file.open(QIODevice::WriteOnly))
    {
        for(int i = 0;i<answers.length();i++)
        {
            file.write((QString::number(i + 1) + " - " + answers[i] + '\n').toUtf8());
        }
        file.close();
    }
    else
    {
        qDebug() << "file Open Error";
    }
}

