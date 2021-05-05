//
// Created by coleg on 5/3/21.
//

#include <unistd.h>
#include <climits>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "Commands.h"

void Commands::setVariable(vector<string> tokens, Variables &variables) {
    if (isNumArgsValid(tokens, 3)) {
        string varName = tokens[0];
        string varValue = tokens[2];

        variables.set(varName, varValue);
    } else {
        printSyntaxError("set a variable? (FOO = bar)");
    }
}

void Commands::listVariables(vector<string> tokens, Variables &variables) {
    if (isNumArgsValid(tokens, 1)) {
        variables.list();
    } else {
        printSyntaxError("list variables? (lv)");
    }
}

void Commands::unsetVariable(vector<string> tokens, Variables &variables) {
    if (isNumArgsValid(tokens, 2)) {
        string varName = tokens[1];
        variables.unset(varName);
    } else {
        printSyntaxError("unset a variable? (unset FOO)");
    }
}

void Commands::executeProgram(vector<string> tokens, Variables &variables) {
    if (tokens.size() >= 2) {
        string program = tokens[1];
        vector<string> params;
        params.push_back(tokens[1]);
        string infrom;
        string outto;
        for (auto it = tokens.begin() + 2; it != tokens.end(); it++) {
            // check if infrom or outto keyword
            if (*it == "infrom:") {
                it++;
                infrom = *(it);
            } else if (*it == "outto:") {
                it++;
                outto = *(it);
            } else if (infrom.empty() && outto.empty()) {
                // add to parameters vector
                params.push_back((*it));
            } else if (!infrom.empty() && !outto.empty()) {
                printSyntaxError("execute a program? (! cmd param* [infrom: file0] [outto: file1])");
                return;
            }
        }

        auto envVars = variables.getEnv();

        // all the good stuff should be parsed by now
        auto pid = fork();
        if (pid == 0) {
            // child code, run process
            int inFile, outFile;
            if (!infrom.empty()) {
                inFile = open(infrom.c_str(), O_RDONLY);
                dup2(inFile, STDIN_FILENO);
                close(inFile);
            }
            if (!outto.empty()) {
                outFile = open(outto.c_str(), O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
                dup2(outFile, STDOUT_FILENO);
                close(outFile);
            }

            //auto envVars = variables.getEnv();
            vector<char *> argEnv(envVars.size() + 1);  // one extra for the null
            vector<char *> argParam(params.size() + 1);
            for (size_t i = 0; i != envVars.size(); ++i) {
                argEnv[i] = &envVars[i][0];
            }

            for (size_t i = 0; i != params.size(); ++i) {
                argParam[i] = &params[i][0];
            }

            if (program[0] != '/' && !(program[0] == '.' && program[1] == '/')) {
                // program string not an absolute or relative path, look in PATH var
                // execvpe will search a provided PATH var
                execvpe(program.c_str(), argParam.data(), argEnv.data());
                cerr << "Execv failed" << endl;
            } else {
                execve(program.c_str(), argParam.data(), argEnv.data());
                cerr << "Execv failed" << endl;
            }

        } else {
            // wait for child to exit
            waitpid(pid, nullptr, 0);
        }
    } else {
        printSyntaxError("execute a program? (! ls /etc/passwd)");
    }
}

void Commands::changeDirectory(vector<string> tokens, Variables &variables) {
    if (isNumArgsValid(tokens, 2)) {
        errno = 0;
        // check if error occurred
        if (chdir(tokens[1].c_str()) != 0) {
            // error occurred
            cout << "cd: " << tokens[1] << ": " << strerror(errno) << endl;
        } else {
            // no errors occurred
            variables.set("", getCurrentDirectory(), true);
        }
    } else {
        printSyntaxError("change directory? (cd /home/coleg/Documents)");
    }
}

string Commands::getCurrentDirectory() {
    char temp[PATH_MAX];
    return getcwd(temp, sizeof(temp));
}

bool Commands::isNumArgsValid(const vector<string> &tokens, int numArgs) {
    return tokens.size() == numArgs;
}

void Commands::printSyntaxError(const string &errorEnd) {
    cout << "Invalid command syntax, did you mean to " << errorEnd << endl;
}

bool Commands::handleCommand(vector<string> &tokens, Variables &variables) {
    string command = tokens[0];
    void (*commandFunc)(vector<string> tokens, Variables &variables);
    if (command == "quit") {
        // quit
        return true;
    } else if (command == "cd") {
        // change directory
        commandFunc = &Commands::changeDirectory;
    } else if (command == "!") {
        // execute program
        commandFunc = &Commands::executeProgram;
    } else if (command == "lv") {
        // list vars
        commandFunc = &Commands::listVariables;
    } else if (command == "unset") {
        // unset var
        commandFunc = &Commands::unsetVariable;
    } else if (tokens.size() > 1 && tokens[1] == "=") {
        // set var
        commandFunc = &Commands::setVariable;
    } else {
        // invalid command
        cout << command << ": command not found" << endl;
        return false;
    }
    (*commandFunc)(tokens, variables);
    return false;
}

//Commands::Commands(Variables &variables) : variables(variables) {}



