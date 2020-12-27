#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

#include <iostream>
#include <exception>


class BoundsCheckOverflow : public std::exception {   

    public:
        const char* what();

}; // End class

class UnaccessibleDatabaseMetadata : public std::exception {   

    public:
        const char* what();

}; // End class

class UnaccessibleDatabaseDataFile : public std::exception {   

    public:
        const char* what();

}; // End class

#endif