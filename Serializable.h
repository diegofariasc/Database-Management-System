#ifndef __SERIALIZABLE_H__
#define __SERIALIZABLE_H__

/*
The class represents an interface whose completion requires the
implementation of the serialization method
*/
class Serializable 
{
    public:
        virtual char* serialize() = 0;                      // The method calculates the size in bytes of the serialized buffer
        virtual unsigned int getSerialFormSize() = 0;       // The method creates a char* buffer, serializes the object and returns it

}; // End class

#endif