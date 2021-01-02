#include "Exceptions.h"

/* The exception occurs when a user demands a value to be written in a 
shorter memory bank
e.g. the user attempts to write on 2 bytes the content of an int (4 bytes)
The method overrides what and returns a string indicating the problem
Input:  Void
Output: (string) indicating the causing error
*/
const char* BoundsCheckOverflow::what() 
{
    return "Error BC-0 Bounds check error: Provided data for the tuple exceeds field bounds\n";
} // End InvalidType::what

/* The exception occurs when the system requires a database metadata 
file to be read/written but the access isn't conceeded by the OS
Input:  Void
Output: (string) indicating the causing error
*/
const char* UnaccessibleDatabaseMetadata::what() 
{
    return "Error IO-0 Unaccessible database metadata: The system wasn't able to read/write metadata\n";
} // End InvalidType::what

/* The exception occurs when the system requires a database content 
file to be read/written but the access isn't conceeded by the OS
Input:  Void
Output: (string) indicating the causing error
*/
const char* UnaccessibleDatabaseDataFile::what() 
{
    return "Error IO-1 Unaccessible database data file: The system wasn't able to read/write onto the database content file\n";
} // End InvalidType::what

/* The exception occurs when the system requires an index
file to be read/written but the access isn't conceeded by the OS
Input:  Void
Output: (string) indicating the causing error
*/
const char* UnaccessibleIndexFile::what() 
{
    return "Error IO-2 Unaccessible index file: The system wasn't able to read/write onto the index file\n";
} // End InvalidType::what


/* The exception occurs when the user requires a table
but it does not exist
Output: (string) indicating the causing error
*/
const char* UnexistentTable::what() 
{
    return "Error REF-0 Unexistent table: The specified table does not exist in the current database\n";
} // End InvalidType::what