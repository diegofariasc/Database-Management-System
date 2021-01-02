#include "Table.h"
#include "DiskManager.h"
#include "Exceptions.h"

Table::Table( char* tableName )
{

    // Manager to read values from disk
    DiskManager* manager;

    // Check if table exists prior to accessing
    if ( ! manager->tableExists( tableName ) )
        throw UnexistentTable();

    // Load metadata and index structure root
    meta = manager->readMetadata( tableName );
    index = manager->readBPTree( meta );

} // End constructor

Table::Table( Meta* meta )
{
    // Manager to read values from disk
    DiskManager*    manager;
    BPTree*         tree;

    // Load metadata and index structure root
    manager->storeMetadata( meta );

    // Create structure
    tree = new BPTree( meta, NULL_DISK_POINTER );
    manager->storeBPTree( tree );
    index = tree;

} // End constructor

void Table::insertTuple( Tuple* tuple )
{
    // Manager to write onto disk
    DiskManager* manager;

    // Insert tuple into permanent storage
    manager->insertTuple( tuple );

    // Using the storage direction of the tuple insert into index
    index->insert( tuple->getDiskLocation(), tuple->getPrimary() );

} // End insertTuple

Meta* Table::getMeta() { return meta; }