#include "View.h"
#include <iostream>
#include <limits>
#include<cmath>

View::View( Meta* meta, unsigned short* selectedFields, unsigned short selectedFieldsCount )
{

    // Variables to carry out view initialization
    DECLARE_INTERPRETATION_VARS(values);

    // Definition of the operation to apply to all types
    #define INITIALIZE_MIN_MAX(FOR_TYPE)                                                                                            \
    FOR_TYPE ##Var_values = std::numeric_limits<FOR_TYPE>::min();                                                                   \
    maximums->setValueAt(selectedFields[i], (char*) & FOR_TYPE##Var_values, maximums->meta->getFieldSize( selectedFields[i] ) );    \
    FOR_TYPE##Var_values = std::numeric_limits<FOR_TYPE>::max();                                                                    \
    minimums->setValueAt(selectedFields[i], (char*) & FOR_TYPE##Var_values, maximums->meta->getFieldSize( selectedFields[i] ) );  

    // Initialize tuples
    minimums = new Tuple( meta );
    maximums = new Tuple( meta );
    summations = new Tuple( meta );
    this->meta = meta;
    this->selectedFields = selectedFields;
    this->selectedFieldsCount = selectedFieldsCount;

    // Iterate over each field and initialize min and max
    for (unsigned short i = 0; i < selectedFieldsCount; i++ )
    {
        switch ( maximums->meta->getFieldType( selectedFields[i] ) )
        {
            EXECUTE_OPERATION_ON_ALL_TYPES(INITIALIZE_MIN_MAX)
        } // End switch
    } // End for

} // End constructor


void View::addTuple( Tuple* tuple )
{

    DECLARE_INTERPRETATION_VARS(added);
    DECLARE_INTERPRETATION_VARS(recorded);

    // Define the operation to compute max, mins and summations for all types
    #define COMPUTE_MAX_MINS_SUMMATION(FOR_TYPE)                                                                                    \
                                                                                                                                    \
    tuple->getValueAt( selectedFields[i],(char*) & FOR_TYPE##Var_added);                                                            \
                                                                                                                                    \
    /* Minimums*/                                                                                                                   \
    minimums->getValueAt( selectedFields[i],(char*) & FOR_TYPE##Var_recorded);                                                      \
                                                                                                                                    \
    if ( FOR_TYPE##Var_added <= FOR_TYPE##Var_recorded )                                                                            \
        minimums->setValueAt( selectedFields[i], (char*) & FOR_TYPE##Var_added, tuple->meta->getFieldSize( selectedFields[i] ) );   \
                                                                                                                                    \
    /* Maximums */                                                                                                                  \
    maximums->getValueAt( selectedFields[i],(char*) & FOR_TYPE##Var_recorded);                                                      \
                                                                                                                                    \
    if ( FOR_TYPE##Var_added >= FOR_TYPE##Var_recorded )                                                                            \
        maximums->setValueAt(selectedFields[i], (char*) & FOR_TYPE##Var_added, tuple->meta->getFieldSize(selectedFields[i]) );      \
                                                                                                                                    \
    /* Summation */                                                                                                                 \
    summations->getValueAt(selectedFields[i],(char*) & FOR_TYPE##Var_recorded);                                                     \
                                                                                                                                    \
    FOR_TYPE##Var_recorded += FOR_TYPE##Var_added;                                                                                  \
    summations->setValueAt(selectedFields[i], (char*) & FOR_TYPE##Var_recorded, tuple->meta->getFieldSize(selectedFields[i]) );  
                    
    for (unsigned short i = 0; i < selectedFieldsCount; i++ )
    {
        switch ( tuple->meta->getFieldType( selectedFields[i] ) )
        {
            // Call for calc of execution of max, min and sum operation on all types
            EXECUTE_OPERATION_ON_ALL_TYPES( COMPUTE_MAX_MINS_SUMMATION )
        } // End switch 

    } // End for
    
    tuples.push_back( tuple );

} // End addTuple

template <typename T> 
unsigned int View::getLength( T value )
{
    unsigned int length = 1;

    if ( value < 0 )
        length = trunc( log10( fabs(value) ) ) + 2;
    else if (value > 0)
        length = trunc( log10(value) ) + 1;

    if ( typeid(value) == typeid(float) )
        length += 5;

    else if ( typeid(value) == typeid(double) )
        length += 8;
    
    return length;

} // End getLength


void View::print()
{

    // Declare variables to interpret field contents
    DECLARE_INTERPRETATION_VARS(tuple);

    // Local variables
    unsigned int* fieldLengths;
    unsigned int maxLength;
    unsigned int lengthMinimum;
    unsigned int lengthMaximum;
    unsigned short selectedFieldIndex;
    unsigned short datatype;
    std::string maximumStringForm;
    std::string minimumStringForm;
    std::string token;

    // Allocate space for field lengths
    fieldLengths = (unsigned int*) malloc( sizeof(unsigned int) * selectedFieldsCount );

    // Define operation for calculating length of each field

    #define CALCULATE_FIELD_LENGTH(FOR_TYPE)                                    \
                                                                                \
    /* Get length of minimum value */                                           \
    minimums->getValueAt(selectedFields[i], (char*) & FOR_TYPE##Var_tuple);     \
    lengthMinimum = getLength(FOR_TYPE##Var_tuple);                             \
                                                                                \
    /* Get length of maximum value */                                           \
    maximums->getValueAt(selectedFields[i], (char*) & FOR_TYPE##Var_tuple);     \
    lengthMaximum = getLength(FOR_TYPE##Var_tuple);                             \
                                                                                \
    /* Check which is maximum */                                                \
    if ( lengthMinimum > maxLength )                                            \
        maxLength = lengthMinimum;                                              \
                                                                                \
    if ( lengthMaximum > maxLength )                                            \
        maxLength = lengthMaximum;                                              \
                                                                                \
    fieldLengths[i] = maxLength;    


    // Calculate printing length of all fields
    for (unsigned short i = 0; i < selectedFieldsCount; i++ )
    {
        maxLength = meta->getFieldNameSize( selectedFields[i] );

        switch( meta->getFieldType( selectedFields[i] ) )
        {
            EXECUTE_OPERATION_ON_ALL_TYPES(CALCULATE_FIELD_LENGTH)
        } // End switch 

    } // End for

    printLines( fieldLengths );

    // Print titles
    printf("|");
    for (unsigned short i = 0; i < selectedFieldsCount; i++ )
    {
        token = " %-" + std::to_string(fieldLengths[i]) + "s |";
        printf(token.c_str(), meta->getFieldName( selectedFields[i] ) );
    } // End for
    printf("\n");

    printLines( fieldLengths );

    selectedFieldIndex = 0;
    for (unsigned long long i = 0; i < getTupleCount(); i ++ )
    {
        printf("|");
        for (unsigned short j = 0; j < selectedFieldsCount; j++ )
        {

            switch ( meta->getFieldType( selectedFields[j] ) )
            {
                case INTEGER:
                    token = " %-" + std::to_string(fieldLengths[j]) + "i |";
                    getTupleAt(i)->getValueAt(selectedFields[j], (char*) &integerVar_tuple);
                    printf(token.c_str(), integerVar_tuple);
                    break;
                case FLOAT:
                    token = " %.4-" + std::to_string(fieldLengths[j]) + "f |";
                    getTupleAt(i)->getValueAt(selectedFields[j], (char*) &floatVar_tuple);
                    printf(token.c_str(), floatVar_tuple);
                    break;
                case DOUBLE:
                    token = " %.8-" + std::to_string(fieldLengths[j]) + "lf |";
                    getTupleAt(i)->getValueAt(selectedFields[j], (char*) &doubleVar_tuple);
                    printf(token.c_str(), doubleVar_tuple);
                    break;
            } // End for
            
        } // End for
        printf("\n");

    } // End for

    printLines( fieldLengths );
    printf("\n");

} // End print

void View::printLines( unsigned int* fieldLengths )
{
    printf("+");
    for (unsigned short i = 0; i < selectedFieldsCount; i++ )
    {
        std::string lines( fieldLengths[i] + 2, '-');
        printf("%s+", lines.c_str() );
    } // End for
    printf("\n");

} // End printLines

Tuple* View::getTupleAt( unsigned long long i ) { return tuples.at(i); }
unsigned long long View::getTupleCount() { return tuples.size(); }