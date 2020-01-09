//
// Created by linor on 20/12/2019.
//
#include <string>
#include <map>
#include <thread>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include "vector"
#include <thread>
#include <arpa/inet.h>
#include "VarInfo.h"
#include "Expression.h"
#include "ex1.h"
#include "BoolExpression.h"
#include "Utils.h"
#include "SinletonMutex.h"

using namespace std;
#ifndef EX3_COMMANDS_H
#define EX3_COMMANDS_H

#endif //EX3_COMMANDS_H
static map<string, VarInfo *> rightDirection;
static map<string, double> leftDirection;
static map<string, string> symbolTable;
static queue<string> queueToConnectCommand;
static Interpreter i1;

class Command {
public:
virtual int execute(vector<string> *, int, map<string, Command *> *) {}
bool isTrueCondition(vector<string> &conditionVector);
protected:
/*static map<string, VarInfo *> rightDirection;
static map<string, double> leftDirection;
static map<string, string> symbolTable;
static queue<string> queueToConnectCommand;
static Interpreter i1;*/
};

class OpenServerCommand : public Command {
public:
    OpenServerCommand() {};
    int execute(vector<string> *arr, int index, map<string, Command *> *nameCommandMap);
    void createChronoSimvector(vector<string> *chronologicalSinAdd);
    void readFromClient(int client_socket, vector<string> chronologicalSimAdd);
};

class ConnectCommand : public Command {
public:
    void writeToClient(int client_socket, queue<string> queueToConnectCommand);
    int execute(vector<string> *arr, int index, map<string, Command *> *nameCommandMap);

};

class DefineVarCommand : public Command {
    //need to build destructor.
public:
    int execute(vector<string> *arr, int index, map<string, Command *> *nameCommandMap);

};

// put the new value as the value of the updated variable and push the message about the update
// to the queueToConnectCommand.
class SetVar : public Command {
    int execute(vector<string> *arr, int index, map<string, Command *> *nameCommandMap);

};

class ConditionParser : public Command {
public:
    double exp1Value, exp2Value;
    int execute(vector<string> **arr, int index, map<string, Command *> *nameCommandMap);
    //return if the condition is true or false

};

//while the condition to be recognized in the while loop is true, this function call the execute function
//of all the commands in the while loop.
class WhileCommand : public Command {
public:
    int execute(vector<string> *arr, int index, map<string, Command *> *nameCommandMap);

};

//if the condition to be recognized in the if loop is true, this function call the execute functions
//of all the commands in the body of loop.
class IfCommand : public ConditionParser {
public:
    int execute(vector<string> *arr, int index, map<string, Command *> *nameCommandMap);

};

//this command print the data the user give.
//the function recognize if the input is string or name of variable

class PrintCommand : public Command {
public:
    int execute(vector<string> *arr, int index, map<string, Command *> *nameCommandMap);

};

class SleepCommand : public Command {
public:
    int execute(vector<string> *arr, int index, map<string, Command *> *nameCommandMap);

};
