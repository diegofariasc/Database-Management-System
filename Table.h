#ifndef __TABLE_H__
#define __TABLE_H__

#include "Tuple.h"
#include "BPTree.h"

class Table
{

    private:
        BPTree*             index;
        Meta*               meta;
        
    public:
        
        Table( char* tableName );           // When table exists
        Table( Meta* meta );                // When table does not exist i.e. create

        void insertTuple( Tuple* tuple );
        void deleteTuple( Tuple* tuple );
        void updateTuple( Tuple* oldTuple, Tuple* tuple );

        Meta* getMeta();


}; // End class

#endif