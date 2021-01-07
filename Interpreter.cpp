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

    // Adjust the B+ tree index 
    else if ( strcmp(tokens.at(0),"set") == 0 && strcmp(tokens.at(1),"config") == 0 &&
              strcmp(tokens.at(2),"index") == 0 && strcmp(tokens.at(3),"type") == 0 &&
              strcmp(tokens.at(4),"tree") == 0 && strcmp(tokens.at(5),"pointers") == 0)
    {

        printf("Warning: Adjusting the page size of the B+ tree index structure will override all existing indexes\nand a full system reset will be required. Continue? (Y/N)\n");
        auto start = std::chrono::steady_clock::now();
        auto end = std::chrono::steady_clock::now();

        std::chrono::duration<double> elapsed_seconds = end-start;
        printf("Successfully adjusted B+ tree indexation structure in %.4lfs\n", elapsed_seconds.count() );
        printf("Full system reset initiated\n");
        printf("Full system reset successfully performed in %.4lfs\n\n", elapsed_seconds.count());

    } // End else if

    // Delete table command
    else if ( strcmp(tokens.at(0),"select") == 0 )
    {
        View* result;

        auto start = std::chrono::steady_clock::now();
        result = Interpreter::executeQuery( tokens );
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;

        if (result->getTupleCount() == 0)
        {
            result->print();
            printf("Query result: Empty set in in %.4lfs\n\n",elapsed_seconds.count());
        }
        else
        {
            result->print();
            printf("Query result: %llu entries in %.4lfs\n\n", result->getTupleCount(), elapsed_seconds.count() );
        } // End else

    } // End else if

    // Delete table command
    else if ( strcmp(tokens.at(0),"drop") == 0 && strcmp(tokens.at(1),"table") == 0 )
    {
        auto start = std::chrono::steady_clock::now();
        Interpreter::executeDeleteTableInstruction( tokens );
        auto end = std::chrono::steady_clock::now();

        std::chrono::duration<double> elapsed_seconds = end-start;
        printf("Successfully deleted table '%s' in %.4lfs\n\n", tokens.at(2), elapsed_seconds.count() );

    } // End else if

    // Create table command
    else if ( strcmp(tokens.at(0),"create") == 0 && strcmp(tokens.at(1),"table") == 0 && strcmp(tokens.at(3),"fields") == 0 )
    {
        auto start = std::chrono::steady_clock::now();
        Interpreter::executeTableCreationInstruction( tokens );
        auto end = std::chrono::steady_clock::now();

        std::chrono::duration<double> elapsed_seconds = end-start;
        printf("Successfully created table '%s' in %.4lfs\n\n", tokens.at(2), elapsed_seconds.count() );

    } // End else if

    // Insert command
    else if ( strcmp(tokens.at(0),"insert") == 0 && strcmp(tokens.at(1),"into") == 0 && strcmp(tokens.at(3),"values") == 0 )
    {
        auto start = std::chrono::steady_clock::now();
        Interpreter::executeTupleInsertion( tokens );
        auto end = std::chrono::steady_clock::now();

        std::chrono::duration<double> elapsed_seconds = end-start;
        printf("Successfully inserted tuple into '%s' in %.4lfs\n\n", tokens.at(2), elapsed_seconds.count() );

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
    tableNameLength = strlen( tokens.at(2) );

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
            tupleFieldTypes[processsedFields] = MINIINT;
            tupleFieldSizes[processsedFields] = VARSIZES[MINIINT];
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
            tupleFieldTypes[processsedFields] = UMINIINT;
            tupleFieldSizes[processsedFields] = VARSIZES[UMINIINT];
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
        } // End if
        
        else
        {
            throw UnrecognizedDatatype( tokens.at(i) );
        } // End if
        

        fieldNameSizes[processsedFields] = strlen(tokens.at(i+1));
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
    unsigned short field, tokenIndex;
    int     intVal;
    float   floatVal;
    double  doubleVal;

    // Load table for insertion
    table = new Table( tokens.at(2) );
    tuple = new Tuple( table->getMeta() );

    tokenIndex = 4;
    for ( field = 0; field < table->getMeta()->getFieldCount(); field++ )
    {

        switch ( table->getMeta()->getFieldType(field) )
        {
        case INTEGER:

            intVal = std::atoi( tokens.at(tokenIndex) );
            tuple->setValueAt( field, (char*) &intVal, VARSIZES[INTEGER]);
            break;

        case FLOAT:
            floatVal = std::stof( tokens.at(tokenIndex) );
            tuple->setValueAt( field, (char*) &floatVal, VARSIZES[FLOAT]);
            break;

        case DOUBLE:
            doubleVal = std::stod( tokens.at(tokenIndex) );
            tuple->setValueAt( field, (char*) &doubleVal, VARSIZES[DOUBLE]);
            break;

        } // End switch

        tokenIndex++;

    } // End for

    table->insertTuple( tuple );

} // End executeTupleInsertion


void Interpreter::executeDeleteTableInstruction( std::vector<char*> tokens )
{

    DiskManager* manager;
    manager->deleteTable( tokens.at(2) );

} // End executeDeleteTableInstruction


View* Interpreter::executeQuery( std::vector<char*> tokens )
{
    // Variables
    View*                           view;
    DiskManager*                    manager;
    Meta*                           resultMeta;
    Meta*                           auxiliarMeta;
    Tuple*                          tuple;
    unsigned short*                 selectedFields;
    std::vector<char*>              fields;
    std::vector<unsigned long long> tupleCounts;
    std::vector<Meta*>              involvedMetas;
    unsigned long long              tupleCount;
    unsigned long long              returningTuples;
    bool                            passedFrom;

    passedFrom = false;
    resultMeta = NULL;
    returningTuples = 0;

    // Analze query in reverse <---
    for ( unsigned short i = tokens.size() - 1; i > 0; i--  )
    {

        // Check if "FROM" clause was reached. If so notify it
        if ( strcmp(tokens.at(i), "from") == 0 )
        {
            passedFrom = true;
            continue;
        } // End if 

        // Check if i-th token is prior or posterior to FROM clause
        // If prior -> table, if posterior -> field
        if ( !passedFrom )
        {
            // Add metadatas
            involvedMetas.insert( involvedMetas.begin(), manager->readMetadata( tokens.at(i) ) );
            
            // Add to tuple counts
            tupleCount = manager->getTupleCount( involvedMetas.at(0) );
            tupleCounts.insert( tupleCounts.begin(), tupleCount );

            // Compute returning tuples
            if ( returningTuples == 0 )
                returningTuples = tupleCount;
            else
                returningTuples *= tupleCount;
            
            // Merge metas if necessary
            if ( resultMeta == NULL )
                resultMeta = manager->readMetadata( tokens.at(i) );
            else
            {
                auxiliarMeta = manager->readMetadata( tokens.at(i) );
                auxiliarMeta->mergeWith ( resultMeta );
                resultMeta = auxiliarMeta;
            } // End else

        } // End if
        else
        {
            // Executing renaming operation when required
            if ( i - 1 > 0 && strcmp( tokens.at(i-1), "as") == 0 )
            {
                if ( i - 2 > 0 )
                {
                    // Execute renaming
                    resultMeta->setFieldName( tokens.at(i-2), tokens.at(i) );

                    // Add to list of projection
                    fields.insert(fields.begin(), tokens.at(i));
                    i -= 2;
                } // End if

            } // End if
            else
            {
                fields.insert(fields.begin(), tokens.at(i));
            } // End else


        } // End else
            
    } // End for

    // Build the returning view

    // Check if projection choosed all field
    if ( strcmp(tokens.at(1), "*") == 0 )
        view = new View( resultMeta, ALL_FIELDS, 0 );

    // If not all fields where chosen in the projection
    else
    {
        // Build the array with positions
        selectedFields = (unsigned short*) malloc( fields.size() * sizeof(unsigned short) );

        // Initialize it
        for ( unsigned short i = 0; i < fields.size(); i++ )
            selectedFields[i] = resultMeta->getFieldPositionOfName( fields.at(i) );

        // Build view using above parameters
        view = new View( resultMeta, selectedFields, fields.size() );

    } // End else
        /*


    // Load and add tuples
    for ( disk_pointer i = 0; i < manager->getTupleCount( resultMeta ); i++ )
    {
        tuple = manager->readTupleAt( i, resultMeta );
        view->addTuple(tuple);
    } // End for*/

    for (unsigned long long i = 0; i < returningTuples; i ++ )
    {
        tuple = new Tuple( (char*) calloc(1,resultMeta->getTupleByteSize()), resultMeta, NULL_DISK_POINTER );
        view->addTuple(tuple);
    } // End for



        unsigned long long filling;
        unsigned long long tupleIndex;
        unsigned long long accumulated ;
        unsigned long long offset;
        unsigned long long repetitions;
        unsigned long long table;
        char* readBytes;

        offset = 0;
        repetitions = returningTuples;

        for ( table = 0; table < involvedMetas.size(); table++ )
        {

        accumulated = 0;
        tupleIndex = 0;
        repetitions /= tupleCounts.at(table);

        while ( accumulated < returningTuples )
        {

            filling = 0;
            readBytes = manager->readTupleAt( tupleIndex, involvedMetas.at(table) )->getPayload(); 
            for ( ; filling < repetitions; filling ++ )
            {
                memcpy( (char*) (view->getTupleAt(accumulated)->getPayload() + offset), 
                        readBytes, involvedMetas.at(table)->getTupleByteSize() );
                accumulated++;
            } // End for

            tupleIndex++;
            tupleIndex = tupleIndex >= tupleCounts.at(table) ? tupleIndex = 0 : tupleIndex ;

        } // End while
        
        offset += involvedMetas.at(table)->getTupleByteSize();

        }



    return view;

} // End executeQuery