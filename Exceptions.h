#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

#include <iostream>
#include <exception>
#include <string>

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

class UnaccessibleIndexFile : public std::exception {   

    public:
        const char* what();

}; // End class

class UnexistentTable : public std::exception {   

    private:    
        std::string specified;

    public:
        UnexistentTable( std::string specified );
        const char* what();

}; // End class

class UnrecognizedDatatype : public std::exception {   

    private:
        std::string specified;

    public:
        UnrecognizedDatatype( const std::string specified );
        const char* what();

}; // End class

#endif