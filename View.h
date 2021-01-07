#ifndef __VIEW_H__
#define __VIEW_H__

#include "Tuple.h"
#include "BPTree.h"
#include "Table.h"
#include <vector>

class View
{

    private:
             
        Meta*               meta;
        unsigned short      selectedFieldsCount;
        unsigned short*     selectedFields;
        std::vector<Tuple*> tuples;

        Tuple*              minimums;
        Tuple*              maximums;
        Tuple*              summations;

        template <typename T> 
        unsigned int getLength( T value );
        void printLines( unsigned int* fieldLengths );
        
    public:

        View( Meta* meta, unsigned short* selectedFields, unsigned short selectedFieldsCount );

        void getMinimum( unsigned short field );
        void getMaximum( unsigned short field );
        void getAverage( unsigned short field );
        void addTuple( Tuple* tuple );
        void print();

        Tuple* getTupleAt( unsigned long long i );
        unsigned long long getTupleCount();

        //void filterLarger( unsigned short field, T threshold );
        //void filterShorter( unsigned short field, T threshold );
        //void filterBetween( unsigned short field, T minThreshold, T maxThreshold );

}; // End class

#endif
        
        
