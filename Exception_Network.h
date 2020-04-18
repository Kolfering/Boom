#ifndef EXCEPTION_NETWORK_H
#define EXCEPTION_NETWORK_H

#include <exception>
#include <iostream>

class Exception_Network : public std::exception {
public:
    const char* what();

};

#endif // EXCEPTION_NETWORK_H
