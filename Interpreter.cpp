#include "Interpreter.h"


void Interpreter::interpret( char* command )
{

    char* token;
    std::vector<char*> tokens;

    // Convert to lowercase 
    for (int i = 0; i < COMMAND_BUFFER_SIZE; i ++)
    {
        if ( command[i] >= 65 && command[i] <= 90 )
        {
            command[i] += 32;
        } // End if

    } // End for

    // Tokenize command
    token = strtok(command, " \n\t,");

    while (token != NULL) 
    { 
        tokens.push_back(token);
        token = strtok(NULL, " \n\t,"); 

    } // End while

    // Exit command
    if ( strcmp(tokens.at(0),"exit") == 0 )
    {
        exit(0);
    } // End if

    // Delete table command
    else if ( strcmp(tokens.at(0),"delete") == 0 && strcmp(tokens.at(1),"table") == 0 )
    {
        auto start = std::chrono::steady_clock::now();
        Interpreter::executeDeleteTableInstruction( tokens );
        auto end = std::chrono::steady_clock::now();

        std::chrono::duration<double> elapsed_seconds = end-start;
        printf("Successfully deleted table '%s' in %lfs\n\n", tokens.at(2), elapsed_seconds.count() );

    } // End else if

    // Create table command
    else if ( strcmp(tokens.at(0),"create") == 0 && strcmp(tokens.at(1),"table") == 0 && strcmp(tokens.at(3),"fields") == 0 )
    {
        auto start = std::chrono::steady_clock::now();
        Interpreter::executeTableCreationInstruction( tokens );
        auto end = std::chrono::steady_clock::now();

        std::chrono::duration<double> elapsed_seconds = end-start;
        printf("Successfully created table '%s' in %lfs\n\n", tokens.at(2), elapsed_seconds.count() );

    } // End else if

    // Insert command
    else if ( strcmp(tokens.at(0),"insert") == 0 && strcmp(tokens.at(1),"into") == 0 && strcmp(tokens.at(3),"values") == 0 )
    {
        auto start = std::chrono::steady_clock::now();
        Interpreter::executeTupleInsertion( tokens );
        auto end = std::chrono::steady_clock::now();

        std::chrono::duration<double> elapsed_seconds = end-start;
        printf("Successfully inserted tuple into '%s' in %lfs\n\n", tokens.at(2), elapsed_seconds.count() );

    } // End if
    
    else
    {
        printf("Error: Unknown command\n");
    } // End else
    

} // End interpret


void Interpreter::executeTableCreationInstruction( std::vector<char*> tokens )
{        
    unsigned short  i, j, k;
    unsigned short  processsedFields;
    unsigned short  notNulls;
    unsigned short  tableNameLength;
    char*           tableName; 
    unsigned short  tupleFieldCount;
    unsigned short* tupleFieldTypes; 
    unsigned int*   tupleFieldSizes; 
    unsigned short* fieldNameSizes;
    char**          fieldNames;
    unsigned short  primaryKeyFieldCount; 
    unsigned short* primaryKeyFields;
    unsigned short* notNullFields;
    Meta*           meta;

    tableName = tokens.at(2);
    tableNameLength = sizeof( tokens.at(2) );

    i = 4;
    notNulls = 0;
    while( strcmp(tokens.at(i),"primary") != 0 && i < tokens.size() )
    {
        
        if ( strcmp(tokens.at(i),"notnull") == 0 )
            notNulls += 1;

        i++;

    } // End while

    notNullFields = (unsigned short*) malloc( sizeof(unsigned short) * notNulls );
    tupleFieldCount = (i - 4 - notNulls) / 2;
    tupleFieldTypes = (unsigned short*) malloc( sizeof(unsigned short) * tupleFieldCount );
    tupleFieldSizes = (unsigned int*) malloc( sizeof(unsigned int) * tupleFieldCount );
    fieldNameSizes = (unsigned short*) malloc( sizeof(unsigned short) * tupleFieldCount );
    fieldNames = (char**) malloc( sizeof(char*) * tupleFieldCount );

    processsedFields = 0;
    i = 4, k =0;
    while( strcmp(tokens.at(i),"primary") != 0 )
    {

        if ( strcmp(tokens.at(i),"miniint") == 0 )
        {
            tupleFieldTypes[processsedFields] = MINITINT;
            tupleFieldSizes[processsedFields] = VARSIZES[MINITINT];
        } // End if

        if ( strcmp(tokens.at(i),"shortint") == 0 )
        {
            tupleFieldTypes[processsedFields] = SHORTINT;
            tupleFieldSizes[processsedFields] = VARSIZES[SHORTINT];
        } // End if

        else if ( strcmp(tokens.at(i),"integer") == 0 )
        {
            tupleFieldTypes[processsedFields] = INTEGER;
            tupleFieldSizes[processsedFields] = VARSIZES[INTEGER];
        } // End if

        else if ( strcmp(tokens.at(i),"largeint") == 0 )
        {
            tupleFieldTypes[processsedFields] = LARGEINT;
            tupleFieldSizes[processsedFields] = VARSIZES[LARGEINT];
        } // End if

        else if ( strcmp(tokens.at(i),"uminiint") == 0 )
        {
            tupleFieldTypes[processsedFields] = UMINITINT;
            tupleFieldSizes[processsedFields] = VARSIZES[UMINITINT];
        } // End if

        else if ( strcmp(tokens.at(i),"ushortint") == 0 )
        {
            tupleFieldTypes[processsedFields] = USHORTINT;
            tupleFieldSizes[processsedFields] = VARSIZES[USHORTINT];
        } // End if

        else if ( strcmp(tokens.at(i),"uniteger") == 0 )
        {
            tupleFieldTypes[processsedFields] = UINTEGER;
            tupleFieldSizes[processsedFields] = VARSIZES[UINTEGER];
        } // End if

        else if ( strcmp(tokens.at(i),"ulargeint") == 0 )
        {
            tupleFieldTypes[processsedFields] = ULARGEINT;
            tupleFieldSizes[processsedFields] = VARSIZES[ULARGEINT];
        } // End if

        else if ( strcmp(tokens.at(i),"float") == 0 )
        {
            tupleFieldTypes[processsedFields] = FLOAT;
            tupleFieldSizes[processsedFields] = VARSIZES[FLOAT];
        } // End if

        else if ( strcmp(tokens.at(i),"double") == 0 )
        {
            tupleFieldTypes[processsedFields] = DOUBLE;
            tupleFieldSizes[processsedFields] = VARSIZES[DOUBLE];
        } // End if

        else if ( strcmp(tokens.at(i),"char") == 0 )
        {
            tupleFieldTypes[processsedFields] = CHAR;
            tupleFieldSizes[processsedFields] = VARSIZES[CHAR];
        } // End if

        else if ( strcmp(tokens.at(i),"boolean") == 0 )
        {
            tupleFieldTypes[processsedFields] = BOOLEAN;
            tupleFieldSizes[processsedFields] = VARSIZES[BOOLEAN];
        } // End iF

        fieldNameSizes[processsedFields] = sizeof( tokens.at(i+1) );
        fieldNames[processsedFields] = tokens.at(i+1);

        // Add not null condition if indicated
        if ( (i + 2 < tokens.size()) && (strcmp( tokens.at(i+2), "notnull") == 0) )
        {
            notNullFields[k++] = processsedFields;
            i++;
        } // End if

        processsedFields++;
        i+= 2;

    } // End for

    // Interpret primary keys
    i = 4 + ( tupleFieldCount * 2 ) + notNulls + 2;
    primaryKeyFieldCount = 0;
    while( i < tokens.size() )
    {
        
        primaryKeyFieldCount ++;
        i++;

    } // End while

    primaryKeyFields = (unsigned short*) malloc( sizeof(unsigned short) * primaryKeyFieldCount );
    i = 4 + ( tupleFieldCount * 2 ) + notNulls + 2;

    while( i < tokens.size() )
    {
        
        for ( j = 0 ; j < tupleFieldCount; j++ )
        {

            if ( strcmp( fieldNames[j], tokens.at(i) ) == 0 )
                break;
            
        } // End for

        primaryKeyFields[i] = j;
        i++;

    } // End while

    // Create metadata of the table
    meta = new Meta(
        tableNameLength,
        tableName,
        tupleFieldCount,
        tupleFieldTypes, 
        tupleFieldSizes, 
        primaryKeyFieldCount,
        primaryKeyFields,
        fieldNameSizes,
        fieldNames,
        notNulls,
        notNullFields
    );

    // Create table
    new Table( meta );

} // End executeTableCreationInstruction

void Interpreter::executeTupleInsertion( std::vector<char*> tokens )
{
    Table* table;
    Tuple* tuple;

    // Variables to store tuple fields
    int     intVal;
    float   floatVal;
    double  doubleVal;

    // Load table for insertion
    table = new Table( tokens.at(2) );
    tuple = new Tuple( table->getMeta() );

    for ( unsigned short i = 4; i < table->getMeta()->getFieldCount(); i++ )
    {

        switch ( table->getMeta()->getFieldType(i) )
        {
        case INTEGER:

            intVal = std::atoi( tokens.at(i) );
            tuple->setValueAt( i, (char*) &intVal, VARSIZES[INTEGER]);
            break;

        case FLOAT:
            floatVal = std::stof( tokens.at(i) );
            tuple->setValueAt( i, (char*) &floatVal, VARSIZES[FLOAT]);
            break;

        case DOUBLE:
            doubleVal = std::stod( tokens.at(i) );
            tuple->setValueAt( i, (char*) &doubleVal, VARSIZES[DOUBLE]);
            break;

        } // End switch

    } // End for

    table->insertTuple( tuple );

} // End executeTupleInsertion


void Interpreter::executeDeleteTableInstruction( std::vector<char*> tokens )
{

    

} // End executeDeleteTableInstruction