#ifndef SERVER_H
#define SERVER_H
#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtDebug>
#include "client.h"
#include <QList>
#include "remoteserver.h"

class Server: public QObject{
    Q_OBJECT
private:
    QTcpServer* _server;
    uint _port;
    QList<Client*>* _clients;
    QList<RemoteServer*>* _remoteSrevers;

public:
    Server(uint port);
    void run();

private slots:
    void processConnection();
    void clientAuthenticated();

public slots:
    void sendBroadcast(const QString& message);
    void clientDisconnected();
};

#endif // SERVER_H
