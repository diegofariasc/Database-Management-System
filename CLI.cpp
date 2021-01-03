#include "CLI.h"
#include "Constants.h"
#include "Interpreter.h"
#include "Exceptions.h"

void CLI::start()
{
   char command[50];

   while (true)
   {
        printf("dbms > ");
        fgets(command, COMMAND_BUFFER_SIZE, stdin);

        try{
            Interpreter::interpret(command);

        } // End try
        catch( BoundsCheckOverflow& e )
        {
            std::cout << e.what() << std::endl;
        } // End catch
        catch( UnaccessibleDatabaseMetadata& e )
        {
            std::cout << e.what() << std::endl;
        } // End catch
        catch( UnaccessibleDatabaseDataFile& e )
        {
            std::cout << e.what() << std::endl;
        } // End catch
        catch( UnaccessibleIndexFile& e )
        {
            std::cout << e.what() << std::endl;
        } // End catch
        catch( UnexistentTable& e )
        {
            std::cout << e.what() << std::endl;
        } // End catch
        catch( UnrecognizedDatatype& e )
        {
            std::cout << e.what() << std::endl;
        } // End catch
        
        fflush(stdin);


   } // End while 

} // End start
