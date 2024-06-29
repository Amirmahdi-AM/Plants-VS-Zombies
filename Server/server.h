#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QList>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    QList<QTcpSocket *> clients;
    int numCli = 0;
    void onReadyRead(QTcpSocket *clientSocket);
    void onDisconnected(QTcpSocket *clientSocket);
    void signUp(const QString &_name, const QString &_username, const QString &_password, const QString &_phoneNumber, const QString &_email, QTcpSocket *client);
    bool checkExistingAccounts(const QString &_username);
    void signIn(const QString &_username, const QString &_password, QTcpSocket *client);
    bool validateCredentials(const QString &_username, const QByteArray &_password);
};

#endif // SERVER_H
