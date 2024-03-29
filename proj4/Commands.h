//
// Created by coleg on 5/3/21.
//

#ifndef PROJ4_COMMANDS_H
#define PROJ4_COMMANDS_H

#include "Variables.h"
#include "vector"

using namespace std;

class Commands {
private:
    // command functions
    static void setVariable(vector<string> tokens, Variables &variables);
    static void listVariables(vector<string> tokens, Variables &variables);
    static void unsetVariable(vector<string> tokens, Variables &variables);
    static void executeProgram(vector<string> tokens, Variables &variables);
    static void changeDirectory(vector<string> tokens, Variables &variables);

    // utility methods
    static string getCurrentDirectory();
    static bool isNumArgsValid(const vector<string> &tokens, int numArgs);
    static void printSyntaxError(const string& errorEnd);
public:
    Commands() = default;

    // handle commands, redirect in to subroutines. Return true if request quit
    static bool handleCommand(vector<string>& tokens, Variables & variables);
};


#endif //PROJ4_COMMANDS_H
