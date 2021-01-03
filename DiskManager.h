#ifndef __DISKMANAGER_H__
#define __DISKMANAGER_H__

#include "Tuple.h"
#include "BPTree.h"
#include "Exceptions.h"
#include "Types.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

class DiskManager
{

    public:

        // Tuple and metadata storage
        void insertTuple(Tuple* tuple);
        void storeMetadata(Meta* meta );

        // Tuple and metadata reading
        Tuple*  readTupleAt(disk_pointer position, Meta* meta);
        Meta*   readMetadata(char* tableName);
        
        // BPTree and BPLeaf storage
        void storeBPTree(BPTree* tree);
        void updateBPLeaf(BPTree::BPLeaf* leaf);
        void insertBPLeaf(BPTree::BPLeaf* leaf);

        // BPTree and BPLeaf reading
        BPTree::BPLeaf* readBPLeafAt(disk_pointer position, Meta* meta);
        BPTree *        readBPTree(Meta* meta);

        bool tableExists( char* table );
        void deleteTable( char* table );

}; // End class

#endif