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
            //string varName = token.substr(1);

            // TODO this code kind of sucks we should rewrite it
            vector<string> varNames;
            string varName;
            // for character in token string
            for (auto &ch : token){
                if (!varName.empty() && isalnum(ch)){
                    varName.push_back(ch);
                } else if (ch == '$'){
                    if (varName.empty()) {
                        varName.push_back(ch);
                    } else {
                        // TODO test and remove
                        cout << "varName string not empty (impossible)" << endl;
                        return -1;
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

//            cout << "varNames" << endl;
//            for (auto varNameT : varNames){
//                cout << varNameT << endl;
//            }

            // TODO at least rename this
            for (const auto& varNameE : varNames){
                if (variables.has(varNameE.substr(1))) {
                    auto iter = token.find(varNameE);
                    token.replace(iter, varNameE.length(), variables.get(varNameE.substr(1)));
                } else {
                    cout << "Syntax error, variable " << varNameE << " does not exist!" << endl;
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

int main() {
    Variables shellVars = Variables(generateSysVarMap(), "CWD");
    Commands shellCmds = Commands();
    //testTokenizer();


    bool quit = false;
    do {
        cout << shellVars.get("PS");
        string input;
        getline(cin, input);
        vector<string> tokens = tokenize(input);

        // TODO remove for prod
//        for (const auto &token : tokens) {
//            cout << token << endl;
//        }

        // only try to run a command if we can parse token vars
        if (parseTokenVars(tokens, shellVars)) {
            quit = shellCmds.handleCommand(tokens, shellVars);
            //quit = handleCommand(tokens, shellVars);
        }

    } while (!quit);


    //std::cout << "Hello, World!" << std::endl;
    return 0;
}
