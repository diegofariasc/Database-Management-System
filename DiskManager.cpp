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


    // Check if the system is able to access the file to store
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
