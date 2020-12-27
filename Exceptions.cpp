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
    return "Error BC-0 Bounds check error: \nProvided data for the tuple exceeds field bounds";
} // End InvalidType::what

/* The exception occurs when the system requires a database metadata 
file to be read/written but the access isn't conceeded by the OS
Input:  Void
Output: (string) indicating the causing error
*/
const char* UnaccessibleDatabaseMetadata::what() 
{
    return "Error IO-0 Unaccessible database metadata: \nThe system wasn't able to read/write metadata";
} // End InvalidType::what

/* The exception occurs when the system requires a database content 
file to be read/written but the access isn't conceeded by the OS
Input:  Void
Output: (string) indicating the causing error
*/
const char* UnaccessibleDatabaseDataFile::what() 
{
    return "Error IO-1 Unaccessible database data file: \nThe system wasn't able to read/write onto the database content file";
} // End InvalidType::what