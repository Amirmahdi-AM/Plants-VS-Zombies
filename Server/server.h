#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QVector>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    QVector<QTcpSocket *> clients;
    void onReadyRead(QTcpSocket *clientSocket);
    void onDisconnected(QTcpSocket *clientSocket);
    bool signUp(const QString &_name, const QString &_username, const QString &_password, const QString &_phoneNumber, const QString &_email);
    bool checkExistingAccounts(const QString &_username);
    bool signIn(const QString &_username, const QString &_password);
    bool validateCredentials(const QString &_username, const QByteArray &_password);
    QString Person(QString _username);
    void editPerson(const QString &_name, const QString &_username, const QString &_password, const QString &_phoneNumber, const QString &_email);
};

#endif // SERVER_H
