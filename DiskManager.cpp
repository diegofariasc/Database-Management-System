#include "DiskManager.h"


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
    pointer = fopen( fileName.c_str() ,"r+");

    // Attempt to create file if not loaded (maybe unexistent)
    if ( !pointer )
    {   
        pointer = fopen( fileName.c_str() ,"ab+");
    } // End if 

    // Check if the system is able to access the file to store
    if ( !pointer )
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
    if ( !pointer )
    {
        throw UnaccessibleDatabaseDataFile();
    } // End if 

    else 
    {
        // Acquire the serialized form with the built-in method
        serializedForm = tuple->serialize();

        // Move the pointer to the end and write 
        fwrite( serializedForm, tuple->getSerialFormSize(), 1, pointer );

        // Set in-disk location to the tuple for further usage
        location = (ftell(pointer) / tuple->meta->getTupleByteSize()) - 1;
        tuple->setDiskLocation( location );

        // Release resources
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
    if ( !pointer )
    {
        throw UnaccessibleDatabaseDataFile();
    } // End if 


    else 
    {
        // Allocate space for storing read values
        readPayload = (char*) malloc( meta->getTupleByteSize() );

        // Move the pointer to the appropiate position and read
        fseek( pointer, meta->getTupleByteSize() * position, SEEK_SET );
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
    unsigned short  tupleFieldCount;
    unsigned short* tupleFieldTypes; 
    unsigned int*   tupleFieldSizes; 
    unsigned int*   tupleFieldPositions;
    unsigned short  primaryKeyFieldCount; 
    unsigned short* primaryKeyFields;
    unsigned int    primaryKeyByteSize;
    unsigned short* fieldNameSizes;
    char*           fieldName;
    char**          fieldNames;
    unsigned short  notNullFieldsCount;
    unsigned short* notNullFields;
    disk_pointer*   minimums;
    disk_pointer*   maximums;
    char*           summations;

    // Open file
    fileName += ".meta";
    pointer = fopen( fileName.c_str() ,"rb");


    // Check if the system is able to access the file to read
    if ( !pointer )
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

        // Extract indicator of the primary key size in fields
        memcpy( &primaryKeyFieldCount, offset, sizeof( primaryKeyFieldCount ) );
        offset = (char*) (offset + sizeof( primaryKeyFieldCount ));

        // Extract array of the fields that compose the primary key 
        primaryKeyFields = (unsigned short*) malloc( sizeof( unsigned short ) * primaryKeyFieldCount );
        memcpy( primaryKeyFields, offset, sizeof( unsigned short ) * primaryKeyFieldCount );
        offset = (char*) (offset + sizeof( unsigned short ) * primaryKeyFieldCount );

        // Extract indicator of the primary key size in bytes
        memcpy( &primaryKeyByteSize, offset, sizeof( primaryKeyByteSize ) );
        offset = (char*) (offset + sizeof( primaryKeyByteSize ));

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

        // Extract number of not null fields
        memcpy( &notNullFieldsCount, offset, sizeof( notNullFieldsCount ) );
        offset = (char*) (offset + sizeof( notNullFieldsCount ) );

        // Extract not null fields array
        notNullFields = (unsigned short*) malloc( sizeof( unsigned short ) * notNullFieldsCount );
        memcpy( notNullFields, offset, sizeof( unsigned short ) * notNullFieldsCount );
        offset = (char*) (offset + sizeof( unsigned short ) * notNullFieldsCount );

        // Extract minimum pointers array
        minimums = (disk_pointer*) malloc( sizeof( disk_pointer ) * tupleFieldCount );
        memcpy( offset, minimums, sizeof( disk_pointer ) * tupleFieldCount );
        offset = (char*) (offset + sizeof( disk_pointer ) * tupleFieldCount );

        // Extract maximum pointers array
        maximums = (disk_pointer*) malloc( sizeof( disk_pointer ) * tupleFieldCount );
        memcpy( offset, maximums, sizeof( disk_pointer ) * tupleFieldCount );
        offset = (char*) (offset + sizeof( disk_pointer ) * tupleFieldCount );

        // Extract array with summations
        summations = (char*) malloc( tupleByteSize );
        memcpy( summations, offset, tupleByteSize );
        offset = (char*) (offset + tupleByteSize );

        // Build returning object
        meta = new Meta(tableNameLength, tableName, tupleByteSize, tupleFieldCount, 
                        tupleFieldTypes, tupleFieldSizes, tupleFieldPositions,
                        primaryKeyFieldCount, primaryKeyFields, primaryKeyByteSize,
                        fieldNameSizes, fieldNames, notNullFieldsCount, notNullFields,
                        minimums, maximums, summations );


        // Release resources
        free(readBytes);
        fclose(pointer); 
    } // End else

    return meta;

} // End readMetadata


void DiskManager::storeBPTree(BPTree* tree)
{
    // Variables
    FILE* pointer;
    char* serializedForm;
    std::string fileName( tree->meta->getTableName() );

    // Open file
    fileName += ".bpindexconf";
    pointer = fopen( fileName.c_str() ,"r+");

    // Attempt to create file if not loaded (maybe unexistent)
    if ( !pointer )
    {   
        pointer = fopen( fileName.c_str() ,"ab+");
    } // End if 

    // Check if the system is able to access the file to store
    if ( !pointer )
    {   
        throw UnaccessibleIndexFile();
    } // End if 

    else 
    {
        // Acquire the serialized form with the built-in method
        serializedForm = tree->serialize();

        // Move the pointer to the beginning and write 
        fseek( pointer, 0L, SEEK_SET);
        fwrite( serializedForm, tree->getSerialFormSize(), 1, pointer );

        // Release resources
        free(serializedForm);
        fclose( pointer ); 

    } // End else

} // End storeBPTree

void DiskManager::insertBPLeaf(BPTree::BPLeaf* leaf)
{
    // Variables
    FILE* pointer;
    char* serializedForm;
    std::string fileName( leaf->meta->getTableName() );
    disk_pointer position;

    // Open file
    fileName += ".bpindex";
    pointer = fopen( fileName.c_str() ,"ab+");

    // Check if the system is able to access the file to store
    if ( !pointer )
    {
        throw UnaccessibleIndexFile();
    } // End if 

    else 
    {

        // Acquire the serialized form with the built-in method
        serializedForm = leaf->serialize();

        // Move the pointer to the beginning and write 
        fwrite( serializedForm, leaf->getSerialFormSize(), 1, pointer );
        
        // Calculate insertion position and set it
        position = (ftell(pointer) / leaf->getSerialFormSize()) -1;
        leaf->diskLocation = position;

        // Release resources
        free(serializedForm);
        fclose(pointer); 
        

    } // End else

} // End insertBPLeaf

void DiskManager::updateBPLeaf(BPTree::BPLeaf* leaf)
{
    // Variables
    FILE* pointer;
    char* serializedForm;
    std::string fileName( leaf->meta->getTableName() );
    disk_pointer position;

    // Open file
    fileName += ".bpindex";
    pointer = fopen( fileName.c_str() ,"r+");

    // Check if the system is able to access the file to store
    if ( !pointer )
    {
        throw UnaccessibleIndexFile();
    } // End if 

    else 
    {
        // Acquire the serialized form with the built-in method
        serializedForm = leaf->serialize();

        // Move the pointer to the appropiate position and write
        fseek( pointer, leaf->diskLocation * leaf->getSerialFormSize(), SEEK_SET );
        fwrite( serializedForm, leaf->getSerialFormSize(), 1, pointer );

        // Release resources
        free(serializedForm);
        fclose(pointer); 

    } // End else

} // End insertBPLeaf


BPTree::BPLeaf* DiskManager::readBPLeafAt(disk_pointer position, Meta* meta)
{

    // Variables
    FILE* pointer;
    BPTree::BPLeaf* leaf;
    char* readBytes;
    char* offset;
    char* key;
    char** keys;
    unsigned int leafByteSize;
    unsigned short filling;
    bool isLeaf;
    disk_pointer* diskPointers;
    disk_pointer parent;
    std::string fileName( meta->getTableName() );

    // Open file
    fileName += ".bpindex";
    pointer = fopen( fileName.c_str() ,"rb");

    // Check if the system is able to access the file to read
    if ( !pointer )
    {
        throw UnaccessibleIndexFile();
    } // End if 

    else 
    {
        // Allocate space for storing read values
        leafByteSize =  sizeof(disk_pointer) * BP_POINTERS + 
                        meta->getPrimaryKeyByteSize() * (BP_POINTERS - 1) +
                        sizeof (unsigned short) + sizeof(disk_pointer) + sizeof(isLeaf);
        readBytes = (char*) malloc( leafByteSize );

        // Move the pointer to the appropiate position and read
        fseek( pointer, leafByteSize * position, SEEK_SET );
        fread( readBytes, leafByteSize, 1, pointer );

        // Extract values from raw bytes onto appropiate variables
        offset = readBytes;

        // Extract parent pointer
        memcpy( &parent, offset, sizeof( parent ) );
        offset = (char*) (offset + sizeof( parent ));

        // Extract filling counter
        memcpy( &filling, offset, sizeof( filling ) );
        offset = (char*) (offset + sizeof( filling ));

        // Extract if leaf
        memcpy( &isLeaf, offset, sizeof( isLeaf ) );
        offset = (char*) (offset + sizeof( isLeaf ));

        // Extract pointers
        diskPointers = (disk_pointer*) malloc( sizeof(disk_pointer) * BP_POINTERS );
        memcpy( diskPointers, offset, sizeof(disk_pointer) * BP_POINTERS);
        offset = (char*) (offset + (sizeof(disk_pointer) * BP_POINTERS));

        // Extract keys 
        keys = (char**) malloc( sizeof(char*) * (BP_POINTERS - 1) );
        for (int i = 0; i < BP_POINTERS - 1; i++)
        {

            // If less than filling counter, then add the key value
            if ( i < filling )
            {
                key = (char*) malloc( meta->getPrimaryKeyByteSize() );
                memcpy(key, offset, meta->getPrimaryKeyByteSize());
                offset = (char*) (offset + meta->getPrimaryKeyByteSize());
                keys[i] = key;
            } // End if 

            // Otherwise add a null pointer
            else{
                keys[i] = NULL;
            } // End else
        } // End for 

        // Build new leaf 
        leaf = new BPTree::BPLeaf(meta, filling, diskPointers, keys, parent, isLeaf, position);

        // Release resources
        free(readBytes);
        fclose(pointer); 

    } // End else

    return leaf;

} // End readBPLeafAt


BPTree* DiskManager::readBPTree(Meta* meta)
{

    // Variables
    FILE*               pointer;
    BPTree*             tree;
    char*               readBytes;
    char*               offset;
    disk_pointer        root;
    unsigned long long  byteSize;
    std::string         fileName( meta->getTableName() );

    // Open file
    fileName += ".bpindexconf";
    pointer = fopen( fileName.c_str() ,"rb");

    // Check if the system is able to access the file to read
    if ( !pointer )
    {
        throw UnaccessibleIndexFile();
    } // End if 

    else 
    {
        // Allocate space for storing read values
        fseek( pointer, 0L, SEEK_END );
        byteSize = ftell( pointer );
        readBytes = (char*) malloc( sizeof(disk_pointer) );

        // Move the pointer to the appropiate position and read
        fseek( pointer, 0L, SEEK_SET );
        fread( readBytes, byteSize , 1, pointer );

        // Extract values from raw bytes onto appropiate variables
        offset = readBytes;

        // Extract root pointer
        memcpy( &root, offset, sizeof( root ) );
        offset = (char*) (offset + sizeof( root ));

        // Build new tree    
        tree = new BPTree (meta, root);

        // Release resources
        free(readBytes);
        fclose(pointer); 

    } // End else

    return tree;

} // End readBPLeafAt

bool DiskManager::tableExists( char* table )
{

    // Variables
    FILE* pointer;
    bool exists = true;
    std::string fileName( table );

    // Open file
    fileName += ".meta";
    pointer = fopen( fileName.c_str() ,"rb");

    // Check if the system is able to access the file to store
    if ( !pointer )
    {
        exists = false;
    } // End if 

    // Release resources
    fclose(pointer); 
    return exists;

} // End tableExists


void DiskManager::deleteTable( char* table )
{

    // Error if table does not exist
    if ( ! tableExists(table) )
        throw UnexistentTable( std::string(table) );

    // Delete meta
    std::string metaFilename( table );
    metaFilename += ".meta";
    remove( metaFilename.c_str() );

    // Delete index
    std::string bpindexFilename( table );
    bpindexFilename += ".bpindex";
    remove( bpindexFilename.c_str() );

    // Delete index metadata
    std::string bpindexconfFilename( table );
    bpindexconfFilename += ".bpindexconf";
    remove( bpindexconfFilename.c_str() );

    // Delete registers
    std::string dataFilename( table );
    dataFilename += ".data";
    remove( dataFilename.c_str() );

} // End deleteTable