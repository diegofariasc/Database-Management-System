#include "Meta.h"

// ------------------------------------------------------------
// CONSTRUCTORS AND INIT METHOD
// ------------------------------------------------------------
void Meta::init(
    unsigned short  tableNameLength, 
    char*           tableName, 
    unsigned int    tupleByteSize,
    unsigned short  tupleFieldCount, 
    unsigned short* tupleFieldTypes, 
    unsigned int*   tupleFieldSizes, 
    unsigned int*   tupleFieldPositions,
    unsigned short  primaryKeyFieldCount, 
    unsigned short* primaryKeyFields, 
    unsigned int    primaryKeyByteSize,
    unsigned short* fieldNameSizes, 
    char**          fieldNames,
    unsigned short  notNullFieldsCount,
    unsigned short* notNullFields,
    disk_pointer*   minimums,
    disk_pointer*   maximums,
    char*           summations)
    {

    this->tableNameLength = tableNameLength;
    this->tableName = tableName;
    this->tupleByteSize = tupleByteSize;
    this->tupleFieldCount = tupleFieldCount;
    this->tupleFieldTypes = tupleFieldTypes;
    this->tupleFieldSizes = tupleFieldSizes;
    this->tupleFieldPositions = tupleFieldPositions;
    this->primaryKeyFieldCount = primaryKeyFieldCount;
    this->primaryKeyFields = primaryKeyFields;
    this->primaryKeyByteSize = primaryKeyByteSize;
    this->fieldNameSizes = fieldNameSizes;
    this->fieldNames = fieldNames;
    this->notNullFieldsCount = notNullFieldsCount;
    this->notNullFields = notNullFields;
    this->minimums = minimums;
    this->maximums = maximums;
    this->summations = summations;
    

} // End init

Meta::Meta( 
    unsigned short  tableNameLength, 
    char*           tableName, 
    unsigned int    tupleByteSize,
    unsigned short  tupleFieldCount, 
    unsigned short* tupleFieldTypes, 
    unsigned int*   tupleFieldSizes, 
    unsigned int*   tupleFieldPositions,
    unsigned short  primaryKeyFieldCount, 
    unsigned short* primaryKeyFields, 
    unsigned int    primaryKeyByteSize,
    unsigned short* fieldNameSizes, 
    char**          fieldNames,
    unsigned short  notNullFieldsCount,
    unsigned short* notNullFields,
    disk_pointer*   minimums,
    disk_pointer*   maximums,
    char*           summations )
{

    init (  tableNameLength, 
            tableName, 
            tupleByteSize,
            tupleFieldCount, 
            tupleFieldTypes, 
            tupleFieldSizes, 
            tupleFieldPositions,
            primaryKeyFieldCount, 
            primaryKeyFields, 
            primaryKeyByteSize,
            fieldNameSizes, 
            fieldNames,
            notNullFieldsCount,
            notNullFields,
            minimums,
            maximums,
            summations );
            

} // End constructor

Meta::Meta( unsigned short      tableNameLength,
            char*               tableName,
            unsigned short      tupleFieldCount,
            unsigned short*     tupleFieldTypes, 
            unsigned int*       tupleFieldSizes, 
            unsigned short      primaryKeyFieldCount,
            unsigned short*     primaryKeyFields,
            unsigned short*     fieldNameSizes, 
            char**              fieldNames,
            unsigned short      notNullFieldsCount,
            unsigned short*     notNullFields  )
{

    // Initialize byte positions array and byte size
    unsigned int*   tupleFieldPositions;
    unsigned int    tupleByteSize;
    unsigned int    primaryKeyByteSize;
    disk_pointer*   minimums;
    disk_pointer*   maximums;
    char*           summations;

    tupleFieldPositions = (unsigned int*) malloc( sizeof( tupleFieldCount * sizeof(unsigned int) ) );
    tupleByteSize = 0;

    // Iterate over all fields of the tuples
    for (int i = 0; i < tupleFieldCount; i++)
    {
        // Indicate starting positions of each field
        tupleFieldPositions[i] = tupleByteSize;

        // Add to tuple byte size 
        tupleByteSize += tupleFieldSizes[i];

    } // End for

    primaryKeyByteSize = 0;
    // Iterate over all primary key fields of the tuples
    for (int i = 0; i < primaryKeyFieldCount; i++)
    {
        // Add to primary key byte size 
        primaryKeyByteSize += tupleFieldSizes[ primaryKeyFields[i] ];

    } // End for

    // Initialize minimums, maximums and summations arrays
    minimums = (disk_pointer*) malloc( sizeof(disk_pointer) * tupleFieldCount);
    maximums = (disk_pointer*) malloc( sizeof(disk_pointer) * tupleFieldCount);
    summations = (char*) calloc( tupleByteSize, sizeof(char) );

    for ( int i = 0; i < tupleFieldCount; i++ )
    {
        minimums[i] = NULL_DISK_POINTER;
        maximums[i] = NULL_DISK_POINTER;
    } // End for

    init (  tableNameLength, 
            tableName, 
            tupleByteSize,
            tupleFieldCount, 
            tupleFieldTypes, 
            tupleFieldSizes, 
            tupleFieldPositions,
            primaryKeyFieldCount, 
            primaryKeyFields, 
            primaryKeyByteSize,
            fieldNameSizes, 
            fieldNames,
            notNullFieldsCount,
            notNullFields,
            minimums,
            maximums,
            summations );

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
unsigned int    Meta::getPrimaryKeyByteSize()   { return primaryKeyByteSize; }

// With parameters
unsigned short  Meta::getFieldType( unsigned short position )       { return tupleFieldTypes[ position ]; }
unsigned short  Meta::getFieldSize( unsigned short position )       { return tupleFieldSizes[ position ]; }
unsigned short  Meta::getFieldStart( unsigned short position )      { return tupleFieldPositions[ position ]; }
unsigned short  Meta::getFieldNameSize( unsigned short position )   { return fieldNameSizes[ position ]; }
char*           Meta::getFieldName( unsigned short position )       { return fieldNames[ position ]; }
unsigned short  Meta::getPrimaryField( unsigned short position )    { return primaryKeyFields[ position ]; }

// ------------------------------------------------------------
// SETTERS 
// ------------------------------------------------------------
void Meta::setTableName( char* newName ) { tableName = newName; }   // Set the name of the table
void Meta::setFieldName( unsigned short position,                   // Set a new name of a field
                         unsigned short nameSize,
                         char* newName) 
{ 
    fieldNameSizes[position] = nameSize;
    fieldNames[position] = newName;
} // End setFieldName

// ------------------------------------------------------------
// MANDATORY METHOD [VIRTUAL] 
// ------------------------------------------------------------

// For information about this method refer to Serializable.h
unsigned int Meta::getSerialFormSize()
{
    unsigned int length;
    length =sizeof( tableNameLength ) + tableNameLength +
            sizeof( tupleByteSize ) + sizeof( tupleFieldCount ) +
            sizeof( unsigned short ) * tupleFieldCount +
            sizeof( unsigned int ) * tupleFieldCount * 2 + 
            sizeof( primaryKeyFieldCount ) + 
            sizeof( unsigned short ) * primaryKeyFieldCount +
            sizeof( primaryKeyByteSize ) +
            sizeof( unsigned short ) * tupleFieldCount +
            sizeof( notNullFieldsCount ) +
            sizeof(unsigned short) * notNullFieldsCount +
            (2 * sizeof(disk_pointer) * tupleFieldCount) +
            tupleByteSize;
            
    // Sum to size the tuple field name sizes
    for ( int i = 0; i < tupleFieldCount; i ++ )
    {
        length += fieldNameSizes[i];
    } // End for

    return length;

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
    serialization = (char*) malloc( serializedSize );
    offset = serialization;

    // Copy values onto the heap-reserved memory:

    // Copy table name length indicator 
    memcpy( offset, &tableNameLength, sizeof( tableNameLength ) );
    offset = (char*) (offset + sizeof( tableNameLength ));

    // Copy table name
    memcpy( offset, tableName, tableNameLength);
    offset = (char*) (offset + tableNameLength);

    // Copy byte size indicator
    memcpy( offset, &tupleByteSize, sizeof( tupleByteSize ) );
    offset = (char*) (offset + sizeof( tupleByteSize ));

    // Copy tuple field count
    memcpy( offset, &tupleFieldCount, sizeof( tupleFieldCount ) );
    offset = (char*) (offset + sizeof( tupleFieldCount ));

    // Copy array with field types
    memcpy( offset, tupleFieldTypes, sizeof( unsigned short ) * tupleFieldCount );
    offset = (char*) (offset + sizeof( unsigned short ) * tupleFieldCount );

    // Copy array with field sizes
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

    // Copy indicator of the primary key size
    memcpy( offset, &primaryKeyByteSize, sizeof( primaryKeyByteSize ) );
    offset = (char*) (offset + sizeof( primaryKeyByteSize ));

    // Copy array of the field lengths
    memcpy( offset, fieldNameSizes, sizeof( unsigned short ) * tupleFieldCount );
    offset = (char*) (offset + sizeof( unsigned short ) * tupleFieldCount );

    // Copy field names
    for (int i = 0; i < tupleFieldCount; i++)
    {
        memcpy( offset, fieldNames[i], fieldNameSizes[i] );
        offset = (char*) (offset + fieldNameSizes[i] );
    } // End for

    // Copy number of not null fields
    memcpy( offset, &notNullFieldsCount, sizeof( notNullFieldsCount ) );
    offset = (char*) (offset + sizeof( notNullFieldsCount ) );

    // Copy not null fields array
    memcpy( offset, notNullFields, sizeof( unsigned short ) * notNullFieldsCount );
    offset = (char*) (offset + sizeof( unsigned short ) * notNullFieldsCount );

    // Copy minimum pointers array
    memcpy( offset, minimums, sizeof( disk_pointer ) * tupleFieldCount );
    offset = (char*) (offset + sizeof( disk_pointer ) * tupleFieldCount );

    // Copy maximum pointers array
    memcpy( offset, maximums, sizeof( disk_pointer ) * tupleFieldCount );
    offset = (char*) (offset + sizeof( disk_pointer ) * tupleFieldCount );

    // Copy array with summations
    memcpy( offset, summations, tupleByteSize );
    offset = (char*) (offset + tupleByteSize );

    return serialization;

} // End serialize


