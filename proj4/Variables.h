//
// Created by coleg on 5/3/21.
//

#ifndef PROJ4_VARIABLES_H
#define PROJ4_VARIABLES_H

#include <string>
#include <vector>
#include <map>
#include <iostream>

using namespace std;

class Variables {
private:
    vector<string> sysVarNames;
    map<string, string> variables;
    string dirVar;

    static bool isVarNameValid(string varName);
    bool isSysVar(const string& varName);

public:
    Variables(const map<string, string>& sysVars, const string& dirVar);

    void set(const string& varName, const string& varValue, bool dir = false);

    void unset(const string& varName);

    void list();

    bool has(const string& varName);

    string get(const string& varName);

    vector<string> getEnv();
};


#endif //PROJ4_VARIABLES_H
