#include "Server.h"
#include <QRegularExpression>
#include "mainwindow.h"
#include "ui_mainwindow.h"

Server::Server(QObject *parent) : QTcpServer(parent)
{
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    clientSocket = new QTcpSocket(this);
    if (clientSocket->setSocketDescriptor(socketDescriptor)) {
        connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
        connect(clientSocket, &QTcpSocket::disconnected, this, &Server::onDisconnected);
    } else {
        delete clientSocket;
    }
}

void Server::onReadyRead()
{
    QByteArray data = clientSocket->readAll();
    QString receivedData(data);

    QStringList fields = receivedData.split(",");
}

void Server::onDisconnected()
{
    clientSocket->deleteLater();
}
