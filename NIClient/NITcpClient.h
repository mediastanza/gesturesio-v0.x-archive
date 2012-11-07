#ifndef NITCPCLIENT_H
#define NITCPCLIENT_H

#include <QObject>
#include <QString>
#include <QTcpSocket>

class NITcpClient: public QObject
{
    Q_OBJECT

public:
    NITcpClient(QObject* parent=0);
    ~NITcpClient();
    void connectServer(QString address, quint16 port);
    void disconnectServer();
    void transfer(QString msg);

public slots:
    void serverConnected();
    void startRead();

private:
    QTcpSocket client;
    bool isConnected;
    static const int BUFFER_SIZE = 1024;
};

#endif // NITCPCLIENT_H
