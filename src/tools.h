#ifndef TOOLS_H
#define TOOLS_H

#include <exception>
#include <string>

/*!
 * \brief La classe BadAllocation est une surcouche de std::bad_alloc, qui pourrait transmettre des messages personnalisés.
 */
class BadAllocation : public std::bad_alloc
{
public:
    BadAllocation(const std::string &message="Allocation failed.") {m_message = message;}
    const char* what() const throw() {return m_message.c_str();}
    ~BadAllocation() throw() {}

private:
    std::string m_message;
};

#endif // TOOLS_H
