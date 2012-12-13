#ifndef TOOLS_H
#define TOOLS_H

#include <exception>
#include <string>

namespace Gost
{
/*!
 * \brief La classe BadAllocation est une surcouche de std::bad_alloc, qui pourrait transmettre des messages personnalisés.
 */
class BadAllocation : public std::bad_alloc
{
public:
    /*!
     * \brief Le constructeur initialise m_message à la valeur de message
     * \param message Le message qui sera contenu dans l'objet. Vaut "Allocation failed" par défaut.
     */
    BadAllocation(const std::string &message="Allocation failed.") {m_message = message;}

    /*!
     * \return Le message contenu dans l'objet.
     */
    const char* what() const throw() {return m_message.c_str();}
    ~BadAllocation() throw() {}///< Le destructeur.

private:
    std::string m_message;
};
}

#endif // TOOLS_H
