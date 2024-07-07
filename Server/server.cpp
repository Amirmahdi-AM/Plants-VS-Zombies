#include "Server.h"
#include <QRegularExpression>
#include <QFile>
#include <QTextStream>
#include <QByteArray>
#include <QCryptographicHash>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QtAlgorithms>
#include <algorithm>
#include <stdlib.h>
#include <QVector>

Server::Server(QObject *parent) : QTcpServer(parent)
{
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *new_client = new QTcpSocket(this);

    if (new_client->setSocketDescriptor(socketDescriptor)) {
        connect(new_client, &QTcpSocket::readyRead, this, [this, new_client]() { onReadyRead(new_client); });
        connect(new_client, &QTcpSocket::disconnected, this, [this, new_client]() { onDisconnected(new_client); });
        clients.push_back(new_client);

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
        if (signUp(fields[1], fields[2], fields[3], fields[4], fields[5])) {
            QString respone = "113,";
            respone += receivedData;
            clientSocket->write(respone.toUtf8());
        }
        else
            clientSocket->write("111");
    }
    if (fields[0] == "12") {
        if(signIn(fields[1], fields[2])) {
            QString respone = "113,";
            respone += Person(fields[1]);
            clientSocket->write(respone.toUtf8());
        }
        else
            clientSocket->write("114");
    }
    if (fields[0] == "13") {
        if (clients.size() == 2) {
            int player1 = rand () % 2;
            int player2 = player1 - 1;
            clients[0]->write(QString::number(player1).toUtf8());
            clients[1]->write(QString::number(player2).toUtf8());
        }
    }

    if (fields[0] == "14") {
        editPerson(fields[1], fields[2], fields[3], fields[4], fields[5]);
    }
}

void Server::onDisconnected(QTcpSocket *clientSocket)
{
    clientSocket->deleteLater();
    clients.erase(std::find(clients.begin(), clients.end(), clientSocket));
}

bool Server::signUp(const QString &_name, const QString &_username, const QString &_password, const QString &_phoneNumber, const QString &_email) {
    if (checkExistingAccounts(_username)) {
        return false;
    }

    QByteArray passwordHash = QCryptographicHash::hash(_password.toUtf8(), QCryptographicHash::Sha256);
    QFile accFile("Accounts.txt");
    if (accFile.open(QIODevice::Append | QIODevice::Text)){
        QTextStream out(&accFile);
        out << _username << "," << passwordHash.toHex() << "," << _name << "," << _phoneNumber << "," << _email << "\n";
        accFile.close();
    }
    return true;
}

bool Server::checkExistingAccounts(const QString &_username) {
    QFile accFile("Accounts.txt");
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

bool Server::signIn(const QString &_username, const QString &_password)
{
    QByteArray passwordHash = QCryptographicHash::hash(_password.toUtf8(), QCryptographicHash::Sha256);
    if (validateCredentials(_username, passwordHash.toHex())){
        return true;
    }
    else{
        return false;
    }
}

bool Server::validateCredentials(const QString &_username, const QByteArray &_password)
{
    QFile accFile("Accounts.txt");
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

QString Server::Person(QString _username)
{
    QFile accFile("Accounts.txt");
    if (accFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&accFile);
        while(!in.atEnd()){
            QString line = in.readLine();
            QStringList fields = line.split(",");
            if (fields[0] == _username){
                return line;
            }
        }
        accFile.close();
    }
    return "";
}

void Server::editPerson(const QString &_name, const QString &_username, const QString &_password, const QString &_phoneNumber, const QString &_email)
{
    QVector<QString> lines;
    QFile accFile("Accounts.txt");
    if (accFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&accFile);
        while(!in.atEnd()){
            QString line = in.readLine();
            QStringList fields = line.split(",");
            if (fields[0] == _username){
                QString temp = "";
                temp += _username + ",";
                QByteArray passwordHash = QCryptographicHash::hash(_password.toUtf8(), QCryptographicHash::Sha256);
                temp += passwordHash.toHex() + ",";
                temp += _name + ",";
                temp += _phoneNumber + ",";
                temp += _email;
                lines.push_back(temp);
            } else {
                lines.push_back(line);
            }
        }
        accFile.close();

        if (accFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            QTextStream out(&accFile);
            for (const QString &line : lines) {
                out << line << '\n';
            }
            accFile.close();
        }
    }
}
