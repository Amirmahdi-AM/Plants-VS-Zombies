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
#include <QDateTime>

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
    mute.lock();
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
    else if (fields[0] == "12") {
        if(signIn(fields[1], fields[2])) {
            QString respone = "113,";
            respone += Person(fields[1]);
            clientSocket->write(respone.toUtf8());
        }
        else
            clientSocket->write("114");
    }
    else if (fields[0] == "13") {
        players.push_back(clientSocket);
        playerNames.push_back(fields[1]);
        player1 = fields[1];
        if (players.size() == 2) {
            player2 = fields[1];
            int player1 = rand () % 2;
            int player2 = 1 - player1;
            clients[0]->write(QString::number(player1).toUtf8());
            clients[1]->write(QString::number(player2).toUtf8());
        }
    }
    else if (fields[0] == "14") {
        editPerson(fields[1], fields[2], fields[3], fields[4], fields[5]);
        QString respone = "113,";
        respone += fields[2]+","+fields[3]+","+fields[1]+","+fields[4]+","+fields[5];
        clientSocket->write(respone.toUtf8());
    }
    else if (fields[0] == "15") {//forgot password-check email pass
        if(checkemail_pass(fields[1], fields[2])){
            clientSocket->write("1151");
        }
        else{
            clientSocket->write("1152");
        }
    }
    else if (fields[0] == "16") {//Resetpass
        QString line = findUser_WithEmail(fields[1],fields[2]);
        QStringList parts = line.split(",");
        parts[1] = fields[3];
        editPerson(parts[2], parts[0], parts[1], parts[3], parts[4]);
        QString output = "116";
        clientSocket->write(output.toUtf8());
    }
    else if (fields[0] == "card") {//Resetpass
        for(auto temp : players){
            temp->write(receivedData.toUtf8());
        }
    }
    else if (fields[0] == "Round1") {
        firstRoundWinner = fields[1];
        firstRoleWinner = fields[2];
        for (auto temp : players) {
            QString output = "NextRound,";
            output += receivedData;
            temp->write(output.toUtf8());
        }
    }
    else if (fields[0] == "Round2") {
        secondRoundWinner = fields[1];
        secondRoleWinner = fields[2];
        for (auto temp : players) {
            QString output = "EOG";
            temp->write(output.toUtf8());
        }
        saveHistory();
        player1 = "";
        player2 = "";
        firstRoleWinner = "";
        firstRoundWinner = "";
        secondRoleWinner = "";
        secondRoundWinner = "";
        players.clear();
    }
    else if (fields[0] == "His") {
        QString output = "His," + readHistory(fields[1]);
        clientSocket->write(output.toUtf8());
    }
    else if (fields[0] == "Chat") {
        for (auto temp : players) {
            temp->write(receivedData.toUtf8());
        }
    }
    mute.unlock();
}

void Server::onDisconnected(QTcpSocket *clientSocket)
{
    auto it1 = std::find(clients.begin(), clients.end(), clientSocket);
    if (it1 != clients.end())
        clients.erase(it1);

    auto it2 = std::find(players.begin(), players.end(), clientSocket);
    if (it2 != players.end())
        players.erase(it2);

    int index = -1;
    for (int i = 0; i < players.size(); i++) {
        if (players[i] == clientSocket) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        QString winner = playerNames[index];
        firstRoundWinner = winner;
        firstRoleWinner = "Plants";
        secondRoundWinner = winner;
        secondRoleWinner = "Zombies";
        saveHistory();
    }

    clientSocket->deleteLater();
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

bool Server::checkemail_pass(const QString &_phoneNumber, const QString &_email)
{

    QFile accFile("Accounts.txt");
    if (accFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&accFile);
        while(!in.atEnd()){
            QString line = in.readLine();
            QStringList fields = line.split(",");
            if (fields[3] == _phoneNumber && fields[4] == _email){
                return true;
            }
        }
        accFile.close();
    }
    return false;
}

QString Server::findUser_WithEmail(const QString &_phoneNumber, const QString &_email){
    QFile accFile("Accounts.txt");
    if (accFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&accFile);
        while(!in.atEnd()){
            QString line = in.readLine();
            QStringList fields = line.split(",");
            if (fields[3] == _phoneNumber && fields[4] == _email){
                return line;
            }
        }
        accFile.close();
    }
    return "";
}

void Server::saveHistory()
{
    QFile hisFile("History.txt");
    if (hisFile.open(QIODevice::Append | QIODevice::Text)){
        QDateTime currentDateTime = QDateTime::currentDateTime();
        QString dateTimeString = currentDateTime.toString("yyyy-MM-dd HH:mm:ss");
        QTextStream out(&hisFile);
        QString matchWinner;
        if(firstRoundWinner==secondRoundWinner){
            matchWinner=firstRoundWinner;
        }
        else{
            matchWinner="Tie";
        }
        out <<dateTimeString<<"/"<<player1 << "/" << player2 << "/" <<firstRoundWinner <<"/"<< firstRoleWinner << "/" << secondRoundWinner <<"/"<< secondRoleWinner << "/"<<matchWinner<<"\n";
        hisFile.close();
    }
}

QString Server::readHistory(QString _username)
{
    QString historty = "";
    QFile hisFile("History.txt");
    if (hisFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&hisFile);
        while(!in.atEnd()){
            QString line = in.readLine();
            QStringList fields = line.split("/");
            if (fields[1] == _username || fields[2] == _username) {
                historty +="|  " + fields[0] + "  |" + "\n" + fields[1] + " vs " + fields[2] + "\n" + "Round 1 : " + fields[3] + " - " + "Role : " + fields[4] + "\n" + "Round 2 : " + fields[5] + " - " + "Role : " + fields[6] + "\n" + "Match winner : " + fields[7] + "\n" + "-----------------------------------------------------------\n";
            }
        }
        hisFile.close();
    }
    return historty;
}









