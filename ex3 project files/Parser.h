//
// Created by linor on 24/12/2019.
//

#ifndef EX3_PARSER_H
#define EX3_PARSER_H

#include <map>
#include <string>
#include "commands.h"
using namespace std;

class Parser {
public:

    //constructor
    Parser() { buildMap(); }

    //create map of nama's words in lexed vector and the matching commands
    void buildMap() {
        nameCommandsMap.insert(pair<string, Command *>("openDataServer", new OpenServerCommand));
        nameCommandsMap.insert(pair<string, Command *>("connectControlClient", new ConnectCommand()));
        nameCommandsMap.insert(pair<string, Command *>("var", new DefineVarCommand()));
        nameCommandsMap.insert(pair<string, Command *>("while", new WhileCommand()));
        nameCommandsMap.insert(pair<string, Command *>("if", new IfCommand()));
        nameCommandsMap.insert(pair<string, Command *>("Print", new PrintCommand()));
        nameCommandsMap.insert(pair<string, Command *>("Sleep", new SleepCommand()));
    }

    /*
     in while loop pass on the lexed vector and call execute function for all command by order.
     if the word is name of var,the function call the execute of set var
    */
    void parse(vector<string> *commandsParametersVector) {
        int index = 0;
        int size = commandsParametersVector->size();
        while (index < size) {
            string s = commandsParametersVector->at(index);
            if (nameCommandsMap.find(s) != nameCommandsMap.end()) {
                Command *c = this->nameCommandsMap[s];
                index += c->execute(commandsParametersVector, index, &nameCommandsMap);
            } else if (commandsParametersVector->at(index + 1) == "=") {
                //command setVariable with queue.
                Command *c = new SetVar;
                index += c->execute(commandsParametersVector, index, &nameCommandsMap);
            }
        }
    }
private:
    map<string, Command *> nameCommandsMap;
};

#endif //EX3_PARSER_H
