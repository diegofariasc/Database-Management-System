#ifndef __BPTREE_H__
#define __BPTREE_H__

#include "Tuple.h"
#include "Constants.h"

class DiskManager;

class BPTree : Serializable
{

    private:

    public:

        Meta* meta;
        disk_pointer    root;

        class BPLeaf : Serializable {

            public: 
                
                Meta*           meta;
                disk_pointer*   diskPointers;
                disk_pointer    diskLocation;
                disk_pointer    parent;
                char**          keys;
                unsigned short  filling;

                char* serialize();
                unsigned int getSerialFormSize();

                BPLeaf(Meta* meta);
                BPLeaf(Meta* meta, unsigned short filling, disk_pointer* diskPointers, char** keys, disk_pointer parent);
                
                
                void insert( disk_pointer pointer, char* key );
                void shiftRight( unsigned short position );
                BPTree::BPLeaf* split();
                void print();

        }; // End class

        BPTree (Meta* meta, disk_pointer root);

        void insert( disk_pointer location, char* primary );
        
        char* serialize();
        unsigned int getSerialFormSize();

}; // End class

#endif