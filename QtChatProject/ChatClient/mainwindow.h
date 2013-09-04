#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include "client.h"
#include <QMenu>
#include <QTextStream>
#include <QDir>


class MainWindow : public QWidget
{
    Q_OBJECT    
private:
    QPushButton* _sendButton;
    QPushButton* _connectButton;
    QPushButton* _smileButton;
    QListWidget* _list;
    Client* _client;
    QLineEdit* _input;

    QString _login;

public:
    MainWindow(QString host, uint port, QString login, QWidget *parent = 0):
        QWidget(parent), _login(login) {
        _sendButton = new QPushButton("send", this);
        _connectButton = new QPushButton("connect", this);
        _smileButton = new QPushButton("smile", this);
        _list = new QListWidget(this);
        _client = new Client(host, port);
        _input = new QLineEdit(this);
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(_list);
        layout->addWidget(_sendButton);
        layout->addWidget(_connectButton);
        QHBoxLayout* inputLayout = new QHBoxLayout;
        inputLayout->addWidget(_input);
        inputLayout->addWidget(_smileButton);
        layout->addLayout(inputLayout);
        QObject::connect(_sendButton, SIGNAL(clicked()), this, SLOT(send()));
        QObject::connect(_connectButton, SIGNAL(clicked()), this, SLOT(connectClient()));

        QMenu* m = createSmileMenu();
        _smileButton->setMenu(m);

        setLayout(layout);
    }

    ~MainWindow(){}
private:
    QMenu* createSmileMenu(){
        QDir dir(":/resource/img");
        QStringList files = dir.entryList();
        QMenu* res = new QMenu(this);
        foreach(QString s, files){
            QString filepath = dir.path() + QString("/") + s;
            QIcon icon(filepath);
            QAction* action = res->addAction(icon, "", this, SLOT(appendInput()));
            action->setData(filepath);
        }
        return res;
    }

public slots:
    void send(){
        _client->sendMessage(_input->text());
        _input->clear();
    }
    void connectClient(){
        bool b = _client->auth(_login);
        if(b){
            _connectButton->hide();
            connect(_client, SIGNAL(messageReceive(QString)), SLOT(recv(QString)));
        } else {
            qDebug()<<"auth error";
        }
    }
    void recv(const QString& message){
        qDebug()<<message;
        QListWidgetItem* item = new QListWidgetItem(_list);

        item->setText(message);
        _list->addItem(item);
        update();
    }

private slots:
    void appendInput(){
        QAction* a = dynamic_cast<QAction*>(sender());
        QFile htmlFile(":resource/page.html");
        htmlFile.open(QIODevice::ReadOnly);
        QTextStream ts(&htmlFile);
        QString s = ts.readAll();
        qDebug() << s;
        QString filename = a->data().value<QString>();
        s = s.replace("@img@", filename);
        QString res;
        QTextStream ts2(&res);
        ts2<<_input->text()<<s;
        _input->setText(res);
    }
};

#endif // MAINWINDOW_H
