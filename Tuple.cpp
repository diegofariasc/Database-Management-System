#include "Tuple.h"
#include "Exceptions.h"

// ------------------------------------------------------------
// CONSTRUCTORS
// ------------------------------------------------------------
Tuple::Tuple( Meta* meta )
{
    
    // Associate metadata
    this->meta = meta;
    
    // From metadata build tuple payload
    this->payload = (char*) calloc( 0, this->meta->getTupleByteSize() );

    // Initialize tuple with null disk pointer
    this->diskLocation = NULL_DISK_POINTER;

} // End constructor

Tuple::Tuple( char* content, Meta* meta, disk_pointer diskLocation )
{
    // Associate metadata
    this->meta = meta;
    
    // From metadata build tuple payload
    this->payload = content;

    // Initialize tuple with null disk pointer
    this->diskLocation = diskLocation;

} // End constructor


// ------------------------------------------------------------
// DESTRUCTOR
// ------------------------------------------------------------
Tuple::~Tuple(){

    // Release the payload 
    free(this->payload);

} // End destructor


// ------------------------------------------------------------
// METHODS
// ------------------------------------------------------------
/* The method allows to set a value from the tuple [Not in disk]
Input:  position with the field number at which the value must be set
        value with the value to be set 
        length with the byte size of the value to set
Output: void
Warning: Method performs a bounds check before setting the value.
In case of error, exception is thrown
*/
void Tuple::setValueAt( unsigned short position, char* value, unsigned int length )
{
    
    // Check if the field is safe to set into the tuple
    if ( length <= meta->getFieldSize( position ) )
    {
        // Restart memory bank
        memset( (char*) (payload + meta->getFieldStart( position ) ), 0, meta->getFieldSize( position ) );

        // Copy given value
        memcpy( (char*) (payload + meta->getFieldStart( position ) ), value, length );

    } // End if 

    // If the length of the value to set does not fit then throw a
    // bound overflow error 
    else
    {
        throw BoundsCheckOverflow();
    } // End else

} // End setValueAt


/* The method allows to get a value from the tuple
Input:  position with the field number at which the value must be set
        value with the value to be set 
Output: void
Warning: The method assumes that output is a properly-sized variable
to store the content of the tuple field
*/
void Tuple::getValueAt( unsigned short position, char* output )
{
    
    // Copy value from tuple to given variable
    memcpy( output, (char*) (payload + meta->getFieldStart( position ) ) , meta->getFieldSize( position ) );

} // End getValueAt


/* The method establishes the position (in disk) of a tuple
Input:  position with the registry # at which the tuple is stored
Output: void
*/
void Tuple::setDiskLocation( disk_pointer position )
{
    diskLocation = position;
} // End setDiskLocation

/* The method allows to extract an array of char that represents the 
primary key of the tuple
Input:  void
Output: char* with the primary key in byte-representation
Note: Method is safe as given char* is in heap already
*/
char* Tuple::getPrimary()
{

    char* primaryKey;
    char* offset;

    primaryKey = (char*) malloc( meta->getPrimaryKeyByteSize() );
    offset = primaryKey;
    
    for ( int i = 0; i < meta->getPrimaryFieldCount(); i++ )
    {
        memcpy( offset, payload + meta->getFieldStart( meta->getPrimaryField(i) ), meta->getFieldSize( meta->getPrimaryField(i) ) );
        offset = (char*) (offset + meta->getFieldSize( meta->getPrimaryField(i) ));
    } // End for

    return primaryKey;

} // End getPrimary

disk_pointer Tuple::getDiskLocation()
{
    return diskLocation;
} // End getDiskLocation


// ------------------------------------------------------------
// MANDATORY METHOD [VIRTUAL] 
// ------------------------------------------------------------

// For information about this method refer to Serializable.h
char* Tuple::serialize()
{
    char* serializedForm;

    serializedForm = (char*) malloc(meta->getTupleByteSize());
    memcpy(serializedForm, payload, meta->getTupleByteSize());

    return serializedForm;
    
} // End serialize

// For information about this method refer to Serializable.h
unsigned int Tuple::getSerialFormSize()
{
    return meta->getTupleByteSize();
} // End getSerialFormSize

