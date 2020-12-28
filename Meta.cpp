#include "Meta.h"

// ------------------------------------------------------------
// CONSTRUCTORS 
// ------------------------------------------------------------
Meta::Meta( unsigned short      tableNameLength,
            char*               tableName,
            unsigned short      tupleFieldCount,
            unsigned short*     tupleFieldTypes, 
            unsigned int*       tupleFieldSizes, 
            unsigned short      primaryKeyFieldCount,
            unsigned short*     primaryKeyFields,
            unsigned short*     fieldNameSizes, 
            char**              fieldNames  )
{
    // Set attributes that are given
    this->tableNameLength = tableNameLength;
    this->tableName = tableName;
    this->tupleFieldCount = tupleFieldCount;
    this->tupleFieldTypes = tupleFieldTypes;
    this->tupleFieldSizes = tupleFieldSizes;
    this->primaryKeyFieldCount = primaryKeyFieldCount;
    this->primaryKeyFields = primaryKeyFields;
    this->fieldNames = fieldNames;
    this->fieldNameSizes = fieldNameSizes;
    this->fieldNames = fieldNames;

    // Initialize byte positions array and byte size
    this->tupleFieldPositions = (unsigned int*) malloc( sizeof( tupleFieldCount * sizeof(unsigned int) ) );
    this->tupleByteSize = 0;

    // Iterate over all fields of the tuples
    for (int i = 0; i < tupleFieldCount; i++)
    {
        // Indicate starting positions of each field
        this->tupleFieldPositions[i] = this->tupleByteSize;

        // Add to tuple byte size 
        this->tupleByteSize += tupleFieldSizes[i];

    } // End for

} // End constructor


// ------------------------------------------------------------
// GETTERS 
// ------------------------------------------------------------

// Without parameters
unsigned short  Meta::getTableNameLength()      { return tableNameLength; }
char*           Meta::getTableName()            { return tableName; }
unsigned int    Meta::getTupleByteSize()        { return tupleByteSize; }
unsigned short  Meta::getFieldCount()           { return tupleFieldCount; }
unsigned short  Meta::getPrimaryFieldCount()    { return primaryKeyFieldCount; }


// With parameters
unsigned short  Meta::getFieldType( unsigned short position )       { return tupleFieldTypes[ position ]; }
unsigned short  Meta::getFieldSize( unsigned short position )       { return tupleFieldSizes[ position ]; }
unsigned short  Meta::getFieldStart( unsigned short position )      { return tupleFieldPositions[ position ]; }
unsigned short  Meta::getFieldNameSize( unsigned short position )   { return fieldNameSizes[ position ]; }
char*           Meta::getFieldName( unsigned short position )       { return fieldNames[ position ]; }

// ------------------------------------------------------------
// MANDATORY METHOD [VIRTUAL] 
// ------------------------------------------------------------

// For information about this method refer to Serializable.h
unsigned int Meta::getSerialFormSize()
{
    return  
            sizeof( tupleByteSize ) + sizeof( tupleFieldCount ) +
            sizeof( unsigned short ) * tupleFieldCount +
            sizeof( unsigned int ) * tupleFieldCount * 2 + 
            sizeof( primaryKeyFieldCount ) + 
            sizeof( unsigned short ) * primaryKeyFieldCount ;

} // End getSerialFormSize

// For information about this method refer to Serializable.h
char* Meta::serialize()
{
    // Variables;
    unsigned int serializedSize;
    char* serialization;
    char* offset;

    // Calculate the size of the serialized form
    serializedSize = getSerialFormSize();

    // Reserve heap memory and create an iteration pointer
    serialization = (char*) calloc( 0, serializedSize );
    offset = serialization;

    // Copy values onto the heap-reserved memory:
    
    // Copy byte size indicator
    memcpy( offset, &tupleByteSize, sizeof( tupleByteSize ) );
    offset = (char*) (offset + sizeof( tupleByteSize ));

    // Copy tuple field count
    memcpy( offset, &tupleFieldCount, sizeof( tupleFieldCount ) );
    offset = (char*) (offset + sizeof( tupleFieldCount ));

    // Copy array with field types
    memcpy( offset, tupleFieldTypes, sizeof( unsigned short ) * tupleFieldCount );
    offset = (char*) (offset + sizeof( unsigned short ) * tupleFieldCount );

    // Copy array with field types
    memcpy( offset, tupleFieldSizes, sizeof( unsigned int ) * tupleFieldCount );
    offset = (char*) (offset + sizeof( unsigned int ) * tupleFieldCount );

    // Copy array with field starting positions
    memcpy( offset, tupleFieldPositions, sizeof( unsigned int ) * tupleFieldCount );
    offset = (char*) (offset + sizeof( unsigned int ) * tupleFieldCount );

    // Copy indicator of the primary key size
    memcpy( offset, &primaryKeyFieldCount, sizeof( primaryKeyFieldCount ) );
    offset = (char*) (offset + sizeof( primaryKeyFieldCount ));

    // Copy array of the fields that compose the primary key 
    memcpy( offset, primaryKeyFields, sizeof( unsigned short ) * primaryKeyFieldCount );
    offset = (char*) (offset + sizeof( unsigned short ) * primaryKeyFieldCount );

    return serialization;

} // End serialize


