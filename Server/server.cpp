#include "Server.h"
#include <QRegularExpression>
#include <QFile>
#include <QTextStream>
#include <QByteArray>
#include <QCryptographicHash>
#include "mainwindow.h"
#include "ui_mainwindow.h"

Server::Server(QObject *parent) : QTcpServer(parent)
{
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *new_client = new QTcpSocket(this);

    if (new_client->setSocketDescriptor(socketDescriptor)) {
        connect(new_client, &QTcpSocket::readyRead, this, [this, new_client]() { onReadyRead(new_client); });
        connect(new_client, &QTcpSocket::disconnected, this, [this, new_client]() { onDisconnected(new_client); });
        clients.append(new_client);
        numCli++;

    }
    else {
        delete new_client;
    }
}

void Server::onReadyRead(QTcpSocket *clientSocket)
{
    QByteArray data = clientSocket->readAll();
    QString receivedData(data);

    QStringList fields = receivedData.split(",");

    if (fields[0] == "11") {
        signUp(fields[1], fields[2], fields[3], fields[4], fields[5], clientSocket);
    }
    if (fields[0] == "12") {
        signIn(fields[1], fields[2], clientSocket);
    }
}

void Server::onDisconnected(QTcpSocket *clientSocket)
{
    clientSocket->deleteLater();
}

void Server::signUp(const QString &_name, const QString &_username, const QString &_password, const QString &_phoneNumber, const QString &_email, QTcpSocket *client) {
    if (checkExistingAccounts(_username)) {
        client->write("111");
        return;
    }

    QByteArray passwordHash = QCryptographicHash::hash(_password.toUtf8(), QCryptographicHash::Sha256);
    QFile accFile(":/Files/Accounts.txt");
    if (accFile.open(QIODevice::Append | QIODevice::Text)){
        QTextStream out(&accFile);
        out << _username << "," << passwordHash << "," << _name << "," << _phoneNumber << "," << _email << "\n";
        accFile.close();
    }
    client->write("112");
}

bool Server::checkExistingAccounts(const QString &_username) {
    QFile accFile(":/Files/Address.txt");
    bool result = false;
    if (accFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&accFile);
        while(!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(",");
            if (fields[0] == _username) {
                result = true;
                break;
            }
        }
        accFile.close();
    }
    return result;
}

void Server::signIn(const QString &_username, const QString &_password, QTcpSocket *client)
{
    QByteArray passwordHash = QCryptographicHash::hash(_password.toUtf8(), QCryptographicHash::Sha256);
    if (validateCredentials(_username, passwordHash.toHex())){
        client->write("113");
    }
    else{
        client->write("114");
    }
}

bool Server::validateCredentials(const QString &_username, const QByteArray &_password)
{
    QFile accFile(":/Files/Accounts.txt");
    if (accFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&accFile);
        while(!in.atEnd()){
            QString line = in.readLine();
            QStringList fields = line.split(",");
            if (fields[0] == _username && fields[1] == _password){
                return true;
            }
        }
        accFile.close();
    }
    return false;
}
