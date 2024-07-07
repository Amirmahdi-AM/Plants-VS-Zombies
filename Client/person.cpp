#include "person.h"

Person::Person() {}

void Person::set_username(QString _username)
{
    username = _username;
}
void Person::set_name(QString _name)
{
    name = _name;
}

void Person::set_password(QString _password)
{
    password = _password;
}

void Person::set_email(QString _email)
{
    email = _email;
}

void Person::set_phoneNumber(QString _phoneNumber)
{
    phoneNumber = _phoneNumber;
}

QString Person::get_username() const
{
    return username;
}

QString Person::get_name() const
{
    return name;
}

QString Person::get_password() const
{
    return password;
}

QString Person::get_email() const
{
    return email;
}

QString Person::get_phoneNumber() const
{
    return phoneNumber;
}

Person::Person(const QString _username,const  QString _name,const  QString _password,const  QString _email,const  QString _phoneNumber)
    :username(_username)
    ,name(_name)
    ,password(_password)
    ,email(_email)
    ,phoneNumber(_phoneNumber)
{}

