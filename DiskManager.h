#ifndef __DISKMANAGER_H__
#define __DISKMANAGER_H__

#include "Tuple.h"

#include <stdio.h>
#include <stdlib.h>

class DiskManager
{

    public:
        void insertTuple( Tuple* tuple );
        void updateTuple( Tuple* tuple );
        void storeMetadata( Meta* meta );

        Tuple* readTupleAt( disk_pointer position, Meta* meta );
        Meta* readMetadata( char* tableName );

}; // End class

#endif