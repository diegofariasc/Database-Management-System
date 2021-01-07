#ifndef __TUPLE_H__
#define __TUPLE_H__

#include "Meta.h"

class Tuple : Serializable
{

    private:
        
        char*           payload;        // Bytes with the tuple content
        disk_pointer    diskLocation;   // Location of the tuple in the disk
        
    public:

        Meta*  meta;     // Metadata associated with the tuple

        // Constructor and destructor
        Tuple( Meta* meta );    
        Tuple( char* content, Meta* meta, disk_pointer diskLocation );
        ~Tuple();

        // Method to retrieve a value from the tuple
        void setValueAt( unsigned short position, char* value, unsigned int length );   

        // Method to get a value from the tuple
        void getValueAt( unsigned short position, char* output );  

        // Method to set a tuple's location in disk
        void setDiskLocation( disk_pointer position );

        // Method to get a tuple's location in disk
        disk_pointer getDiskLocation();

        // Method to get a char array with the tuple's primary key
        char* getPrimary();   

        // Comply with the virtual serialization method
        char* serialize();        
        unsigned int getSerialFormSize();                             

        // Get and set payload
        char* getPayload();
        void setPayload( char* newPayload );

}; // End class


#endif