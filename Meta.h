#ifndef __META_H__
#define __META_H__

#include <stdio.h>
#include <cstdlib>
#include <cstring>

#include "Serializable.h"
#include "Types.h"


class Meta : Serializable
{

    // ------------------------------------------------------------
    // ATTRIBUTES
    // ------------------------------------------------------------
    private:

        unsigned int    tupleByteSize;              // Total size of the tuples in bytes
        unsigned short  tupleFieldCount;            // Total number of fields
        unsigned short* tupleFieldTypes;            // Array with the types of each field
        unsigned int*   tupleFieldSizes;            // Array with the sizes of each field
        unsigned int*   tupleFieldPositions;        // Positions at which each field starts
        unsigned short  primaryKeyFieldCount;       // Number of fields in the primary key
        unsigned short* primaryKeyFields;           // Array with the number of fields at which the primary key is found
        unsigned int*   fieldNameSizes;             // Sizes of each field in bytes
        char**          fieldNames;                 // Names of the fields


    // ------------------------------------------------------------
    // METHODS
    // ------------------------------------------------------------
    public:

        // Constructor methods
        Meta();                                 
        Meta( unsigned short tupleFieldCount, unsigned short* tupleFieldTypes, 
              unsigned int* tupleFieldSizes, unsigned short primaryKeyFieldCount, 
              unsigned short* primaryKeyFields, char** fieldNames );

        unsigned int    getTupleByteSize();                         // Recover the size of the tuples in bytes
        unsigned short  getFieldCount();                            // Get the number of fields in the tuple
        unsigned short  getFieldType( unsigned short position );    // Get the type of the n-th field in the tuple
        unsigned short  getFieldSize( unsigned short position );    // Get the size of the n-th field in the tuple
        unsigned short  getFieldStart( unsigned short position );   // Get the size of the n-th field in the tuple
        unsigned short  getPrimaryFieldCount();                     // Get the size of the primary key

        char*           serialize();                                // Comply with the virtual serialization method
        unsigned int    getSerialFormSize();                        // Comply with the virtual serial form size calculation method

}; // End class

#endif