#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H
#include <qsocketthread.h>
#include <QObject>
class RequestHandler : public ServerSocket
{
    Q_OBJECT
public:
    explicit RequestHandler(int descriptor,QObject* parent = nullptr);
    void sendTestList();
    void sendTest(QString testName);
    void sendLectures();
    void sendLabs();

private slots:
    void newCommand(QString command);
    void processAnswerData(QString answerData);
    void safeAnswerData(QString answerData);
};

#endif // REQUESTHANDLER_H
