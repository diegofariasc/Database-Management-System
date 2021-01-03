#include "Comparator.h"


short Comparator::compareKeys( char* firstKey, char* secondKey, Meta* meta )
{

    // Declaration of available types
    DECLARE_INTERPRETATION_VARS(first);
    DECLARE_INTERPRETATION_VARS(second);

    // Go through the primary key fields and compare them
    for ( unsigned short i = 0; i < meta->getPrimaryFieldCount(); i++  )
    {
        switch( meta->getFieldType( meta->getPrimaryField(i) ) )
        {

        // Compare a MINIINT
        case  MINITINT: 

            memcpy( &miniintVar_first, firstKey, VARSIZES[MINITINT] );
            memcpy( &miniintVar_second, secondKey, VARSIZES[MINITINT] );
            
            if ( miniintVar_first < miniintVar_second )
                return -1;
            else if ( miniintVar_first > miniintVar_second )
                return 1;

            firstKey = (char*) (firstKey + VARSIZES[MINITINT]);
            secondKey = (char*) (secondKey + VARSIZES[MINITINT]);
            break;

        // Compare a SHORTINT
        case  SHORTINT: 
            
            memcpy( &shortintVar_first, firstKey, VARSIZES[SHORTINT] );
            memcpy( &shortintVar_second, secondKey, VARSIZES[SHORTINT] );
            
            if ( shortintVar_first < shortintVar_second )
                return -1;
            else if ( shortintVar_first > shortintVar_second )
                return 1;

            firstKey = (char*) (firstKey + VARSIZES[SHORTINT]);
            secondKey = (char*) (secondKey + VARSIZES[SHORTINT]);
            break;

        // Compare an INTEGER
        case  INTEGER: 
            
            memcpy( &integerVar_first, firstKey, VARSIZES[INTEGER] );
            memcpy( &integerVar_second, secondKey, VARSIZES[INTEGER] );
            
            if ( integerVar_first < integerVar_second )
                return -1;
            else if ( integerVar_first > integerVar_second )
                return 1;

            firstKey = (char*) (firstKey + VARSIZES[INTEGER]);
            secondKey = (char*) (secondKey + VARSIZES[INTEGER]);
            break;

        // Compare a LARGEINT
        case  LARGEINT: 

            memcpy( &largeintVar_first, firstKey, VARSIZES[LARGEINT] );
            memcpy( &largeintVar_second, secondKey, VARSIZES[LARGEINT] );
            
            if ( largeintVar_first < largeintVar_second )
                return -1;
            else if ( largeintVar_first > largeintVar_second )
                return 1;

            firstKey = (char*) (firstKey + VARSIZES[LARGEINT]);
            secondKey = (char*) (secondKey + VARSIZES[LARGEINT]);
            break;

        // Compare an UMINIINT
        case  UMINITINT: 

            memcpy( &uminiintVar_first, firstKey, VARSIZES[UMINITINT] );
            memcpy( &uminiintVar_second, secondKey, VARSIZES[UMINITINT] );
            
            if ( uminiintVar_first < uminiintVar_second )
                return -1;
            else if ( uminiintVar_first > uminiintVar_second )
                return 1;

            firstKey = (char*) (firstKey + VARSIZES[UMINITINT]);
            secondKey = (char*) (secondKey + VARSIZES[UMINITINT]);
            break;

        // Compare an USHORTINT
        case  USHORTINT: 

            memcpy( &ushortintVar_first, firstKey, VARSIZES[USHORTINT] );
            memcpy( &ushortintVar_second, secondKey, VARSIZES[USHORTINT] );
            
            if ( ushortintVar_first < ushortintVar_second )
                return -1;
            else if ( ushortintVar_first > ushortintVar_second )
                return 1;

            firstKey = (char*) (firstKey + VARSIZES[USHORTINT]);
            secondKey = (char*) (secondKey + VARSIZES[USHORTINT]);
            break;

        // Compare an UINTEGER
        case  UINTEGER:

            memcpy( &uintegerVar_first, firstKey, VARSIZES[UINTEGER] );
            memcpy( &uintegerVar_second, secondKey, VARSIZES[UINTEGER] );
            
            if ( uintegerVar_first < uintegerVar_second )
                return -1;
            else if ( uintegerVar_first > uintegerVar_second )
                return 1;

            firstKey = (char*) (firstKey + VARSIZES[UINTEGER]);
            secondKey = (char*) (secondKey + VARSIZES[UINTEGER]);
            break;

        // Compare an ULARGEINT
        case  ULARGEINT:

            memcpy( &ulargeintVar_first, firstKey, VARSIZES[ULARGEINT] );
            memcpy( &ulargeintVar_second, secondKey, VARSIZES[ULARGEINT] );
            
            if ( ulargeintVar_first < ulargeintVar_second )
                return -1;
            else if ( ulargeintVar_first > ulargeintVar_second )
                return 1;

            firstKey = (char*) (firstKey + VARSIZES[ULARGEINT]);
            secondKey = (char*) (secondKey + VARSIZES[ULARGEINT]);
            break;

        // Compare a FLOAT
        case  FLOAT:

            memcpy( &floatVar_first, firstKey, VARSIZES[FLOAT] );
            memcpy( &floatVar_second, secondKey, VARSIZES[FLOAT] );
            
            if ( floatVar_first < floatVar_second )
                return -1;
            else if ( floatVar_first > floatVar_second )
                return 1;

            firstKey = (char*) (firstKey + VARSIZES[FLOAT]);
            secondKey = (char*) (secondKey + VARSIZES[FLOAT]);
            break;

        // Compare a DOUBLE
        case  DOUBLE:
        
            memcpy( &doubleVar_first, firstKey, VARSIZES[DOUBLE] );
            memcpy( &doubleVar_second, secondKey, VARSIZES[DOUBLE] );
            
            if ( doubleVar_first < doubleVar_second )
                return -1;
            else if ( doubleVar_first > doubleVar_second )
                return 1;

            firstKey = (char*) (firstKey + VARSIZES[DOUBLE]);
            secondKey = (char*) (secondKey + VARSIZES[DOUBLE]);
            break;

        // Compare a CHAR
        case  CHAR:

            memcpy( &charVar_first, firstKey, VARSIZES[CHAR] );
            memcpy( &charVar_second, secondKey, VARSIZES[CHAR] );
            
            if ( charVar_first < charVar_second )
                return -1;
            else if ( charVar_first > charVar_second )
                return 1;

            firstKey = (char*) (firstKey + VARSIZES[CHAR]);
            secondKey = (char*) (secondKey + VARSIZES[CHAR]);
            break;

        // Compare a BOOLEAN
        case  BOOLEAN:

            memcpy( &booleanVar_first, firstKey, VARSIZES[BOOLEAN] );
            memcpy( &booleanVar_second, secondKey, VARSIZES[BOOLEAN] );
            
            if ( booleanVar_first < booleanVar_second )
                return -1;
            else if ( booleanVar_first > booleanVar_second )
                return 1;

            firstKey = (char*) (firstKey + VARSIZES[BOOLEAN]);
            secondKey = (char*) (secondKey + VARSIZES[BOOLEAN]);
            break;

        } // End switch

    } // End for

    // There was no difference between the values
    // hence both keys are equal
    return 0;

} // End compareKeys