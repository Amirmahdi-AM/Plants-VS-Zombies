#ifndef PERSON_H
#define PERSON_H
#include <QString>

class Person
{
private:
    QString username,name, password, email, phoneNumber;
    int winRound;

public:
    Person();
    Person(const QString _username,const QString _name,const QString _password,const QString _email,const QString _phoneNumber);
    void set_name(QString _name);
    void set_username(QString _name);
    void set_password(QString _password);
    void set_email(QString _email);
    void set_phoneNumber(QString _phoneNumber);
    QString get_username() const;
    QString get_name() const;
    QString  get_password() const;
    QString get_email() const;
    QString get_phoneNumber() const;
};

#endif // PERSON_H
