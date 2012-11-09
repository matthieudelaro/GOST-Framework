#ifndef TOOLS_H
#define TOOLS_H

#include <exception>
#include <QtCore>
#include <QDomDocument>

class XMLFormatException : public std::exception
{
public:
    XMLFormatException(const QString &message="The XML doesn't match the required pattern.") {m_message = message;}
    const char* what() const throw() {return m_message.toStdString().c_str();}
    ~XMLFormatException() throw() {}

private:
    QString m_message;
};

class BadAllocation : public std::bad_alloc
{
public:
    BadAllocation(const QString &message="Allocation failed.") {m_message = message;}
    const char* what() const throw() {return m_message.toStdString().c_str();}
    ~BadAllocation() throw() {}

private:
    QString m_message;
};

#endif // TOOLS_H

/*#ifndef TOOLS_H
#define TOOLS_H

#include <QtCore>
#include <QDomDocument>

class XMLFormatException : public QtConcurrent::Exception
{
public:
    XMLFormatException(const QString &message="The XML doesn't match the required pattern.") {m_message = message;}
    ~XMLFormatException() throw() {throw *this;}
    void raise() const { throw *this; }
    Exception *clone() const { return new XMLFormatException(*this);}
    const QString& getMessage() const {return m_message;}

    //const char* what() const {return m_message.toStdString().c_str();}

private:
    QString m_message;
};

class BadAllocation : public QtConcurrent::Exception
{
public:
    BadAllocation(const QString &message="Allocation failed.") {m_message = message;}
    ~BadAllocation() throw() {throw *this;}
    void raise() const { throw *this; }
    Exception *clone() const { return new BadAllocation(*this);}
    const QString& getMessage() const {return m_message;}

private:
    QString m_message;
};

#endif // TOOLS_H
*/
