#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QTextStream>


class Client: public QObject{
    Q_OBJECT
private:
    QTcpSocket* _socket;
    QString _login;

public:
    Client(QString host, uint port, QObject* parent = 0);
    bool auth(const QString& login);

private slots:
    void dataRead();

public slots:
    void sendMessage(const QString& message);

signals:
    void messageRecv(const QString&);
};

#endif // CLIENT_H
