#include "DiskManager.h"
#include "Exceptions.h"

#include <iostream>

/* The method allows to write a metadata object into a .meta
file for further usage
Input:  meta with the meta object to store
Output: void
Warning: the method overwrites .data file content with new metadata
*/
void DiskManager::storeMetadata( Meta* meta )
{

    // Variables
    FILE* pointer;
    char* serializedForm;
    std::string fileName( meta->getTableName() );

    // Open file
    fileName += ".meta";
    pointer = fopen( fileName.c_str() ,"wb");

    // Check if the system is able to access the file to store
    if ( pointer == NULL)
    {
        throw UnaccessibleDatabaseMetadata();
    } // End if 

    else 
    {
        // Acquire the serialized form with the built-in method
        serializedForm = meta->serialize();

        // Move the pointer to the beginning and write 
        fseek( pointer, 0L, SEEK_SET);
        fwrite( serializedForm, meta->getSerialFormSize(), 1, pointer );

        // Release resources
        free(serializedForm);
        fclose( pointer ); 

    } // End else

} // End storeMetadata


/* The method allows to append a tuple object into a .data
file for further usage. The method saves the storing location into 
the tuple object's diskLocation attribute
Input:  tuple with the tuple object to store
Output: void
*/
void DiskManager::insertTuple( Tuple* tuple )
{

    // Variables to use
    FILE* pointer;
    char* serializedForm;
    disk_pointer location;
    std::string fileName( tuple->meta->getTableName() );

    // Open file
    fileName += ".data";
    pointer = fopen( fileName.c_str() ,"ab+");

    // Check if the system is able to access the file to store
    if ( pointer == NULL)
    {
        throw UnaccessibleDatabaseDataFile();
    } // End if 

    else 
    {
        // Acquire the serialized form with the built-in method
        serializedForm = tuple->serialize();

        // Move the pointer to the end and write 
        fseek(pointer, 0L, SEEK_END);
        fwrite( serializedForm, tuple->getSerialFormSize(), 1, pointer );

        // Set in-disk location to the tuple for further usage
        location = ftell(pointer) / tuple->meta->getTupleByteSize();
        tuple->setDiskLocation( location );

        // Release resources
        free(serializedForm);
        fclose(pointer); 

    } // End else

} // End insertTuple


/* The method allows to read a tuple from a .data file
Input:  position with the position (in disk) of the required tuple
        meta with the metadata associated with the tuple to read
Output: Tuple* with a new tuple instance whose payload was read
*/
Tuple* DiskManager::readTupleAt( disk_pointer position, Meta* meta )
{
    
    // Variables
    FILE* pointer;
    Tuple* tuple;
    char* readPayload;
    std::string fileName( meta->getTableName() );

    // Open file
    fileName += ".data";
    pointer = fopen( fileName.c_str() ,"rb");


    // Check if the system is able to access the file to read
    if ( pointer == NULL)
    {
        throw UnaccessibleDatabaseDataFile();
    } // End if 


    else 
    {
        // Allocate space for storing read values
        readPayload = (char*) malloc( meta->getTupleByteSize() );

        // Move the pointer to the appropiate position and read
        fseek( pointer, 0L, meta->getTupleByteSize() * position );
        fread( readPayload, meta->getTupleByteSize(), 1, pointer );

        // Build new tuple 
        tuple = new Tuple( readPayload, meta, position );

        // Release resources
        fclose(pointer); 

    } // End else

    return tuple;

} // End readTupleAt


/* The method allows to read a metadata from a .meta file
Input:  tableName with the name of the table whose metadata must be searched
Output: Tuple* with a new tuple instance whose payload was read
*/
Meta* DiskManager::readMetadata( char* tableName )
{

    // Variables

    // For reading
    FILE*               pointer;
    Meta*               meta;
    unsigned long long  bytesSize;
    char*               readBytes;
    char*               offset;
    std::string         fileName( tableName );

    // For decoding
    unsigned short  tableNameLength;
    unsigned int    tupleByteSize;
    unsigned short  tupleFieldCount = 4;
    unsigned short* tupleFieldTypes; 
    unsigned int*   tupleFieldSizes; 
    unsigned int*   tupleFieldPositions;
    unsigned short  primaryKeyFieldCount; 
    unsigned short* primaryKeyFields;
    unsigned short* fieldNameSizes;
    char*           fieldName;
    char**          fieldNames;

    // Open file
    fileName += ".meta";
    pointer = fopen( fileName.c_str() ,"rb");


    // Check if the system is able to access the file to read
    if ( pointer == NULL)
    {
        throw UnaccessibleDatabaseMetadata();
    } // End if 


    else 
    {
        // Calculate the metadata length
        fseek(pointer, 0L, SEEK_END);
        bytesSize = ftell( pointer );
        readBytes = (char*) malloc( bytesSize );

        // Move the pointer to the appropiate position and read
        fseek( pointer, 0L, SEEK_SET );
        fread( readBytes, bytesSize, 1, pointer );

        // Extract values from raw bytes onto appropiate variables
        offset = readBytes;

        // Extract table name length indicator 
        memcpy( &tableNameLength, offset, sizeof( tableNameLength ) );
        offset = (char*) (offset + sizeof( tableNameLength ));
        
        // Skip table name
        offset = (char*) (offset + tableNameLength);

        // Extract byte size indicator
        memcpy( &tupleByteSize, offset, sizeof( tupleByteSize ) );
        offset = (char*) (offset + sizeof( tupleByteSize ));
        
        // Extract tuple field count
        memcpy( &tupleFieldCount, offset, sizeof( tupleFieldCount ) );
        offset = (char*) (offset + sizeof( tupleFieldCount ));

        // Extract array with field types
        tupleFieldTypes = (unsigned short*) malloc( sizeof( unsigned short ) * tupleFieldCount );
        memcpy( tupleFieldTypes, offset, sizeof( unsigned short ) * tupleFieldCount );
        offset = (char*) (offset + sizeof( unsigned short ) * tupleFieldCount );

        // Extract array with field sizes
        tupleFieldSizes = (unsigned int*) malloc( sizeof( unsigned int ) * tupleFieldCount );
        memcpy( tupleFieldSizes, offset, sizeof( unsigned int ) * tupleFieldCount );
        offset = (char*) (offset + sizeof( unsigned int ) * tupleFieldCount );

        // Extract array with field starting positions
        tupleFieldPositions = (unsigned int*) malloc( sizeof( unsigned int ) * tupleFieldCount );
        memcpy( tupleFieldPositions, offset, sizeof( unsigned int ) * tupleFieldCount );
        offset = (char*) (offset + sizeof( unsigned int ) * tupleFieldCount );

        // Extract indicator of the primary key size
        memcpy( &primaryKeyFieldCount, offset, sizeof( primaryKeyFieldCount ) );
        offset = (char*) (offset + sizeof( primaryKeyFieldCount ));

        // Extract array of the fields that compose the primary key 
        primaryKeyFields = (unsigned short*) malloc( sizeof( unsigned short ) * primaryKeyFieldCount );
        memcpy( primaryKeyFields, offset, sizeof( unsigned short ) * primaryKeyFieldCount );
        offset = (char*) (offset + sizeof( unsigned short ) * primaryKeyFieldCount );

        // Extract array of the field name lengths
        fieldNameSizes = (unsigned short*) malloc( sizeof( unsigned short ) * tupleFieldCount );
        memcpy( fieldNameSizes, offset, sizeof( unsigned short ) * tupleFieldCount );
        offset = (char*) (offset + sizeof( unsigned short ) * tupleFieldCount );

        // Initialize field names pointer-pointer array
        fieldNames = (char**) malloc( sizeof( char* ) * tupleFieldCount );

        // Extract field names
        for (int i = 0; i < tupleFieldCount; i++)
        {
            fieldName = (char*) malloc( fieldNameSizes[i] );
            memcpy( fieldName, offset, fieldNameSizes[i] );
            fieldNames[i] = fieldName;
            offset = (char*) (offset + fieldNameSizes[i] );
        } // End for

        // Build returning object
        meta = new Meta(tableNameLength, tableName, tupleByteSize, tupleFieldCount, 
                        tupleFieldTypes, tupleFieldSizes, tupleFieldPositions,
                        primaryKeyFieldCount, primaryKeyFields, fieldNameSizes, fieldNames );


        // Release resources
        free(readBytes);
        fclose(pointer); 
    } // End else

    return meta;

} // End readMetadata