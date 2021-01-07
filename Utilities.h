#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <iostream>
#include <vector>

class Utilities
{

    public:
    
        static char* concatBlocks(char* first, unsigned int firstSize, char* second, unsigned int secondSize)
        {
            char* newPointer = (char*) realloc( first , firstSize + secondSize);
            memcpy( newPointer + firstSize, second, secondSize );
            free(second);
            return newPointer;
        } // End concatBlocks

        template<typename T>
        static void increaseByValue(T* array, unsigned int size, unsigned int increase)
        {
            for ( unsigned int i = 0; i < size; i ++ )
                array[i] += increase;

        } // End increaseByValue


}; // End class

#endif
