//
// Created by coleg on 5/3/21.
//

#include "Variables.h"

bool Variables::isVarNameValid(string varName) {
    // check if first character of variable name is valid
    if (isalpha(varName[0])) {
        for (auto &ch : varName) {
            // check if rest of var name is valid
            if (!isalnum(ch)) {
                cout << "Invalid variable name; must be alpha numeric" << endl;
                return false;
            }
        }
    } else {
        cout << "Invalid variable name; first character must be a letter" << endl;
        return false;
    }
    return true;
}

bool Variables::isSysVar(const string &varName) {
    for (const auto &sysVarName : sysVarNames) {
        if (sysVarName == varName) {
            cout << "Invalid variable name; $" << sysVarName << " is a system variable" << endl;
            return true;
        }
    }
    return false;
}

string Variables::get(const string &varName) {
    return variables.at(varName);
}

Variables::Variables(const map<string, string> &sysVars, const string &dirVar) {
    for (const auto &sysVar : sysVars) {
        sysVarNames.push_back(sysVar.first);
        variables[sysVar.first] = sysVar.second;
    }
    this->dirVar = dirVar;
}

void Variables::set(const string &varName, const string &varValue, bool dir) {
    if (dir) {
        variables[dirVar] = varValue;
    } else if (isVarNameValid(varName)) {
        if (varName != dirVar) {
            variables[varName] = varValue;
        } else {
            cout << "Invalid variable name; can't edit directory variable $" << dirVar << endl;
        }
    }
}

void Variables::unset(const string &varName) {
    if (isVarNameValid(varName) && !isSysVar(varName)) {
        if (this->has(varName)) {
            variables.erase(varName);
        } else {
            cout << "Invalid variable name $" << varName << "; can't unset a variable that does not exist" << endl;
        }
    }
}

void Variables::list() {
    for (const auto &variable : variables) {
        cout << variable.first << "=" << variable.second << endl;
    }
}

bool Variables::has(const string &varName) {
    return variables.find(varName) != variables.end();
}

vector<string> Variables::getEnv() {
    vector<string> env;
    for (const auto &var : variables) {
        string envVar = var.first;
        string varValue = var.second;
        if (varValue.find(' ') != string::npos) {
            // spaces in varVale, we will need to wrap in quotes
            varValue.insert(varValue.begin(), '"');
            varValue.insert(varValue.end(), '"');
            cout << "juice" << endl;
        }
        envVar.append("=");
        envVar.append(varValue);
        env.push_back(envVar);
    }
    return env;

}
