#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <unistd.h>
#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <climits>
#include "Variables.h"
#include "Commands.h"

using namespace std;


// takes an entire line as input string, turns it in to a vector of tokens
vector<string> tokenize(string input) {
    vector<string> tokens;
    istringstream iss(input);
    if (input[0] != '#'){
        string token;
        while (iss >> quoted(token)) {
            tokens.push_back(token);
        }
    }
    return tokens;
}


// takes a vector of tokens and replaces Variables with variable values
// returns false if a token has an invalid variable
bool parseTokenVars(vector<string> &tokens, Variables &variables) {
    for (auto &token : tokens) {
        if (token[0] == '$') {

            // TODO this code is a little convoluted and could use a rewrite
            vector<string> varNames;
            string varName;
            // for character in token string
            for (auto &ch : token){
                if (!varName.empty() && isalnum(ch)){
                    varName.push_back(ch);
                } else if (ch == '$'){
                    if (varName.empty()) {
                        varName.push_back(ch);
                    }
                } else {
                    if (!varName.empty()){
                        varNames.push_back(varName);
                        varName.clear();
                    }
                }
            }
            if (!varName.empty()) {
                varNames.push_back(varName);
            }


            for (const auto& finalVarName : varNames){
                if (variables.has(finalVarName.substr(1))) {
                    auto iter = token.find(finalVarName);
                    token.replace(iter, finalVarName.length(), variables.get(finalVarName.substr(1)));
                } else {
                    cout << "Syntax error, variable " << finalVarName << " does not exist!" << endl;
                    return false;
                }
            }
        }
    }
    return true;
}


string getCurrentDirectory() {
    char temp[PATH_MAX];
    return getcwd(temp, sizeof(temp));
}

map<string, string> generateSysVarMap(){
    map<string, string> sysVariables;
    sysVariables["PS"] = "simpsh>";
    sysVariables["PATH"] = "/bin:/usr/bin";
    sysVariables["CWD"] = getCurrentDirectory();
    return sysVariables;
}

void handleSigInt(int sigNum){
    //signal(SIGINT, handleSigInt); // reset signal handler
}

sighandler_t setSignalHandler(int signum, sighandler_t sighandler) {
    struct sigaction action, old_action;
    action.sa_handler = sighandler;
    sigemptyset(&action.sa_mask); /* Block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* Restart syscallsif possible */
    if (sigaction(signum, &action, &old_action) < 0) {
        cout << "Signal error" << endl;
        exit(-1);
    }
    return (old_action.sa_handler);
}

int main() {
    Variables shellVars = Variables(generateSysVarMap(), "CWD");
    Commands shellCmds = Commands();
    //testTokenizer();

    bool quit = false;
    string input;

    setSignalHandler(SIGINT, handleSigInt);

    cout << shellVars.get("PS");
    while (!quit && getline(cin, input)) {


        vector<string> tokens = tokenize(input);


        // only try to run a command if we can parse token vars
        if (parseTokenVars(tokens, shellVars) && !tokens.empty()) {
            quit = Commands::handleCommand(tokens, shellVars);
            //quit = handleCommand(tokens, shellVars);
        }

        if (!quit) {
            cout << shellVars.get("PS");
        }
    }

    cout << endl;


    //std::cout << "Hello, World!" << std::endl;
    return 0;
}
