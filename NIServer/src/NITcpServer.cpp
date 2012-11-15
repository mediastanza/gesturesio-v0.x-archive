
#include "NITcpServer.h"
#include <iostream>
#include "Utils.h"

using namespace std;

NITcpServer::NITcpServer(QObject *parent): QObject(parent)
{
    connect(&server,SIGNAL(newConnection()),this,SLOT(acceptConnection()));

    // Once command from client is received, forward it to the command parser.
    connect(this,SIGNAL(queryAvaiable(QString)),&parser,SLOT(parse(QString)));

    // Once command from client is parsed, send back the data client asks for.
    connect(&parser,SIGNAL(parsed(QString)),this,SLOT(startWrite(QString)));
}

void NITcpServer::Start(int port)
{
    server.listen(QHostAddress::Any, port);
}

void NITcpServer::Stop()
{
   server.close();
}

NITcpServer::~NITcpServer()
{
}

void NITcpServer::acceptConnection()
{
    client = server.nextPendingConnection();
    connect(client,SIGNAL(readyRead()),this,SLOT(startRead()));
    connect(client,SIGNAL(disconnected()),this,SLOT(clientDisconnected()));

    // Report to webserver that a new client connected.
    HttpRequest(clientIn.c_str());
}

void NITcpServer::startRead()
{
    char buffer[BUFFER_SIZE] = {0};
    client->read(buffer,client->bytesAvailable());
    QString str(buffer);

    emit queryAvaiable(str);
}

void NITcpServer::startWrite(QString echo)
{
    if(client)
        client->write(echo.toAscii(),echo.length());
}

void NITcpServer::clientDisconnected()
{
    // Report to webserver that the client is disconnected.
    HttpRequest(clientOut.c_str());
}

void NITcpServer::SetClientLog(string in, string out)
{
    clientIn = in;
    clientOut = out;
}
