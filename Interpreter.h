#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include "Constants.h"
#include "Table.h"
#include "Exceptions.h"
#include "Meta.h"
#include "DiskManager.h"
#include "View.h"
#include <stdio.h> 
#include <string.h> 
#include <string>
#include <vector>
#include <chrono>

    class Interpreter
    {
        public:
            static void interpret( char* command );

        private:
            static void executeTableCreationInstruction( std::vector<char*> tokens );
            static void executeTupleInsertion( std::vector<char*> tokens );
            static void executeDeleteTableInstruction( std::vector<char*> tokens );
            static View* executeQuery( std::vector<char*> tokens );

    }; // End class

#endif