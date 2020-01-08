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
#include "ex1.cpp"
#include "BoolExpression.h"
#include "Utils.h"

using namespace std;
#ifndef EX3_COMMANDS_H
#define EX3_COMMANDS_H

#endif //EX3_COMMANDS_H

class Command {

public:
    Command() {}

    virtual int execute(vector<string> *, int, map<string, Command *> *) {}

    bool isTrueCondition(vector<string> &conditionVector) {
        string exp1, exp2, oper;
        int found, length, flag = 0;
        string s = conditionVector.front();
        if (s == "true") {
            return true;
        } else if (s == "false") {
            return false;
        }
        double exp1Value, exp2Value;
        string s1 = "", s2 = "", s3 = "";
        vector<string> delims1 = {"==", "!=", "<=", ">="};
        for (size_t i = 0; i < delims1.size(); i++) {
            if (s.find(delims1[i]) != s.npos) {
                found = s.find(delims1[i]);
                length = delims1[i].length();
                exp1 = s.substr(0, found);
                oper = s.substr(found, length);
                exp2 = s.substr(found + length);
                flag = 1;
            }
        }
        if (flag == 0) {
            vector<string> delims2 = {"<", ">"};
            for (size_t i = 0; i < delims2.size(); i++) {
                if (s.find(delims2[i]) != s.npos) {
                    found = s.find(delims2[i]);
                    length = delims2[i].length();
                    exp1 = s.substr(0, found);
                    oper = s.substr(found, length);
                    exp2 = s.substr(found + length);
                }
            }
        }
        if (symbolTable.find(exp1) != symbolTable.end()) {
            string sim = symbolTable.at(exp1);
            //left contain the value of this sim
            if (leftDirection.find(sim) != leftDirection.end()) {
                exp1Value = leftDirection.at(sim);
            }
                //right contain the value of this sim
            else if (rightDirection.find(exp1) != rightDirection.end()) {
                exp1Value = rightDirection.at(exp1)->getValue();
            }
        }
        if (symbolTable.find(exp2) != symbolTable.end()) {
            // exp2Value = symbolTable.at(exp2)->getValue();
            string sim = symbolTable.at(exp2);
            //left contain the value of this sim
            if (leftDirection.find(sim) != leftDirection.end()) {
                exp2Value = leftDirection.at(sim);
            }
            //right contain the value of this sim
            if (rightDirection.find(exp2) != rightDirection.end()) {
                exp2Value = rightDirection.at(exp2)->getValue();
            }
        }
        if (oper == "==") {
            if (exp1Value == exp2Value)
                return true;

        } else if (oper == "<=") {
            if (exp1Value <= exp2Value) {
                return true;
            } else {
                return false;
            }
        } else if (oper == ">=") {
            if (exp1Value >= exp2Value) {
                return true;
            } else {
                return false;
            }
        } else if (oper == "!=") {
            if (exp1Value != exp2Value) {
                return true;
            } else {
                return false;
            }
        } else if (oper == "<") {
            if (exp1Value < exp2Value) {
                return true;
            } else {
                return false;
            }
        } else if (oper == ">") {
            if (exp1Value > exp2Value) {
                return true;
            } else {
                return false;
            }
        }
    }
//check if need to do complexity while
protected:
    static map<string, VarInfo *> rightDirection;
    static map<string, double> leftDirection;
    static map<string, string> symbolTable;
    static queue<string> queueToConnectCommand;
};

map<string, VarInfo *>Command::rightDirection;
map<string, string> Command::symbolTable;
map<string, double> Command::leftDirection;
queue<string> Command::queueToConnectCommand;


class OpenServerCommand : public Command {
public:
    OpenServerCommand() {}

    int execute(vector<string> *arr, int index, map<string, Command *> *nameCommandMap) {
        vector<string> chronologicalSimAdd;
        createChronoSimvector(&chronologicalSimAdd);
        Interpreter *i = new Interpreter();
        string s = (*arr)[index + 1];
        Expression *e = i->interpret(s);
        double port = e->calculate();
        delete e;
        int client_socket, valread, socketfd;
        //create socket
        socketfd = socket(AF_INET, SOCK_STREAM, 0);
        if (socketfd == -1) {
            //error
            std::cerr << "Could not create a socket" << std::endl;
            return -1;
        }
        //bind the socket to a port
        sockaddr_in address; //in means IP4
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY; //give me any IP allocated for my machine
        address.sin_port = htons((int) port);
        if (bind(socketfd, (struct sockaddr *) &address, sizeof(address)) == -1) {
            std::cerr << "Could not bind the socket to an IP" << std::endl;
            return -2;
        }
        //making socket listen to the port
        if (listen(socketfd, 5) == -1) {
            std::cerr << "Error during listening command" << std::endl;
            return -3;
        } else {
            std::cout << "Server is now listening ..." << std::endl;
        }
        // accepting a client
        sockaddr_in client;
        socklen_t clientSize = sizeof(client);
        client_socket = accept(socketfd, (struct sockaddr *) &address, (socklen_t *) &address);
        if (client_socket == -1) {
            std::cerr << "Error accepting client" << std::endl;
            return -4;
        }
        close(socketfd);
        //reading from client
        thread thread1(&OpenServerCommand::readFromClient, this, client_socket, chronologicalSimAdd);
        thread1.detach();
        //readFromClient(client_socket, &chronologicalSimAdd,leftDirection);
        return 2;
    };

    void readFromClient(int client_socket, vector<string> chronologicalSimAdd) {
        while (true) {
            char buffer[1048] = {0};
            int valread = read(client_socket, buffer, 1024);
            updateLeftDirectionMap(buffer, chronologicalSimAdd);
            //cout << buffer << endl;
        }
    }

    void updateLeftDirectionMap(char *buffer, vector<string> csa) {
        map<string,double>::iterator it;
        char delim[] = ",";
        char *token = strtok(buffer, delim);
        //insert to arr od double
        for (int i = 0; i < csa.size(); i++){
            string s = csa.at(i);
            if(leftDirection.find(s) != leftDirection.end()){
                
            }
        }
}
/*
for (int count  // cout<<"hiiii"<<endl;er = 0; counter < 36; counter++) {
    char *token = strtok(buffer, delim);
    //found chronological[i] at leftDirection map
    if (!leftDirection.empty()) {
        for (auto elem:leftDirection) {
            cout << elem.first << " " << elem.second << endl;
        }
    }
    if (leftDirection.find(chronologicalSimAdd[counter]) != leftDirection.end()) {
        cout << "here" << endl;
        leftDirection.at(chronologicalSimAdd[counter]) = atof(token);
        cout << leftDirection.at(chronologicalSimAdd[counter]) << endl;
    }
    token = (NULL, delim);*/

//  updateBySimulator(buffer, &chronoVec);
/* char delim[] = ",";
 char *token = strtok(buffer, delim);
 while (token) {
     if (leftDirection.find((*chronologicalSimAdd)[counter]) != leftDirection.end()) {
         double doubleToken = atof(token);
         leftDirection[(*chronologicalSimAdd)[counter]] = doubleToken;
         cout<< leftDirection.at((*chronologicalSimAdd)[counter])<<"   ";
         counter++;
         token = (NULL, delim);
         cout<<endl;
     }
 }

void updateBySimulator(char *arr, vector<string> *chronoSimAdd) {
    int size = chronoSimAdd->size();
}
*/
void createChronoSimvector(vector<string> *chronologicalSinAdd) {
    chronologicalSinAdd->push_back("/instrumentation/airspeed-indicator/indicated-speed-kt");
    chronologicalSinAdd->push_back("/sim/time/warp");
    chronologicalSinAdd->push_back("/controls/switches/magnetos");
    chronologicalSinAdd->push_back("/instrumentation/heading-indicator/offset-deg");
    chronologicalSinAdd->push_back("/instrumentation/altimeter/indicated-altitude-ft");
    chronologicalSinAdd->push_back("/instrumentation/altimeter/pressure-alt-ft");
    chronologicalSinAdd->push_back("/instrumentation/attitude-indicator/indicated-pitch-deg");
    chronologicalSinAdd->push_back("/instrumentation/attitude-indicator/indicated-roll-deg");
    chronologicalSinAdd->push_back("/instrumentation/attitude-indicator/internal-pitch-deg");
    chronologicalSinAdd->push_back("/instrumentation/attitude-indicator/internal-roll-deg");
    chronologicalSinAdd->push_back("/instrumentation/encoder/indicated-altitude-ft");
    chronologicalSinAdd->push_back("/instrumentation/encoder/pressure-alt-ft");
    chronologicalSinAdd->push_back("/instrumentation/gps/indicated-altitude-ft");
    chronologicalSinAdd->push_back("/instrumentation/gps/indicated-ground-speed-kt");
    chronologicalSinAdd->push_back("/instrumentation/gps/indicated-vertical-speed");
    chronologicalSinAdd->push_back("/instrumentation/heading-indicator/indicated-heading-deg");
    chronologicalSinAdd->push_back("/instrumentation/magnetic-compass/indicated-heading-deg");
    chronologicalSinAdd->push_back("/instrumentation/slip-skid-ball/indicated-slip-skid");
    chronologicalSinAdd->push_back("/instrumentation/turn-indicator/indicated-turn-rate");
    chronologicalSinAdd->push_back("/instrumentation/vertical-speed-indicator/indicated-speed-fpm");
    chronologicalSinAdd->push_back("/controls/flight/aileron");
    chronologicalSinAdd->push_back("/controls/flight/elevator");
    chronologicalSinAdd->push_back("/controls/flight/rudder");
    chronologicalSinAdd->push_back("/controls/flight/flaps");
    chronologicalSinAdd->push_back("/controls/engines/engine/throttle");
    chronologicalSinAdd->push_back("/controls/engines/current-engine/throttle");
    chronologicalSinAdd->push_back("/controls/switches/master-avionics");
    chronologicalSinAdd->push_back("/controls/switches/starter");
    chronologicalSinAdd->push_back("/engines/active-engine/auto-start");
    chronologicalSinAdd->push_back("/controls/flight/speedbrake");
    chronologicalSinAdd->push_back("/sim/model/c172p/brake-parking");
    chronologicalSinAdd->push_back("/controls/engines/engine/primer");
    chronologicalSinAdd->push_back("/controls/engines/current-engine/mixture");
    chronologicalSinAdd->push_back("/controls/switches/master-bat");
    chronologicalSinAdd->push_back("/controls/switches/master-alt");
    chronologicalSinAdd->push_back("/engines/engine/rpm");
}

};

/*
class ConnectCommand : public Command {
public:
    int execute(vector<string> **arr, int index, map<string, Command *> *nameCommandMap) {
        string locS = (**arr)[index + 1];
        const char *c_locS = locS.c_str();
        Interpreter *i = new Interpreter();
        string s = (**arr)[index + 1];
        Expression *e = i->interpret(s);
        double port = e->calculate();
        delete e;
        //create socket
        int client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket == -1) {
            //error
            std::cerr << "Could not create a socket" << std::endl;
            return -1;
        }
        //We need to create a sockaddr obj to hold address of server
        sockaddr_in address; //in means IP4
        address.sin_family = AF_INET;//IP4
        address.sin_addr.s_addr = inet_addr(c_locS);  //the localhost address
        address.sin_port = htons(port);
        //we need to convert our number (both port & localhost)
        // to a number that the network understands.
        int is_connect = connect(client_socket, (struct sockaddr *) &address, sizeof(address));
        if (is_connect == -1) {
            std::cerr << "Could not connect to host server" << std::endl;
            return -2;
        } else {
            std::cout << "Client is now connected to server" << std::endl;
        }
        char hello[] = "Hi from client";
        int is_sent = send(client_socket, hello, strlen(hello), 0);
        if (is_sent == -1) {
            std::cout << "Error sending message" << std::endl;
        } else {
            std::cout << "Hello message sent to server" << std::endl;
        }
        char buffer[1024] = {0};
        int valread = read(client_socket, buffer, 1024);
        std::cout << buffer << std::endl;

        close(client_socket);

        //if here we made a connection
           thread thread2(&ConnectCommand::writeToServer,this,&queueToConnectCommand);
           thread2.join();
           char buffer[1024] = {0};
           int valread = read(client_socket, buffer, 1024);
           std::cout << buffer << std::endl;
           close(client_socket);
        return 3;
    }
};*/
void writeToServer(queue<string> *que) {
    while (true) {
        if (!que->empty()) {
            while (!que->empty()) {
                char buffer[1024] = {0};
                string s = que->front();
                strcpy(buffer, s.c_str());
            }
        }
    }
}


class DefineVarCommand : public Command {
    //need to build destructor.
public:
    int execute(vector<string> *arr, int index, map<string, Command *> *nameCommandMap) {
        string s = (*arr)[index];
        if (s == "var") {
            string varName = (*arr)[index + 1];
            if ((*arr)[index + 2] == "->" || (*arr)[index + 2] == "<-") {
                string simAdress = (*arr)[index + 4];
                //check if the the varName exist in the map,if yes,update all the maps by the pointer
                if ((*arr)[index + 2] == "->") {
                    VarInfo *vI = new VarInfo(varName, simAdress, (*arr)[index + 2]);
                    rightDirection.insert(pair<string, VarInfo *>(varName, vI));
                    symbolTable.insert(pair<string, string>(varName, simAdress));
                } else if ((*arr)[index + 2] == "<-") {
                    //  VarInfo *vI = new VarInfo(varName, (*arr)[index + 4], (*arr)[index + 2]);
                    leftDirection.insert(pair<string, double>((*arr)[index + 4], 0));
                    symbolTable.insert(pair<string, string>(varName, simAdress));
                }
                return 5;
            } else if ((*arr)[index + 2] == "=") {
                string varValueString = (*arr)[index + 3];
                double varValue = atof(varValueString.c_str());
                //not found
                if (rightDirection.find(varName) == rightDirection.end()) {
                    VarInfo *vI = new VarInfo(varName, varValue);
                    rightDirection.insert(pair<string, VarInfo *>(varName, vI));
                } else {
                    VarInfo *varInfoPtr = rightDirection[varName];
                    varInfoPtr->SetValue(varValue);
                }
                return 4;
            }
        }
    }
};

class SetVar : public Command {
    int execute(vector<string> *arr, int index, map<string, Command *> *nameCommandMap) {
        double value;
        string varName = ((*arr)[index]);
        string sim = symbolTable.at(varName);
        if (leftDirection.find(sim) != leftDirection.end()) {
            value = leftDirection.at(sim);
        } else if (rightDirection.find(sim) != rightDirection.end()) {
            value = rightDirection.at(varName)->getValue();
        }
        string s = "set" + sim + to_string(value);
        queueToConnectCommand.push(s);
        return 3;
    }
};

class ConditionParser : public Command {
public:
    double exp1Value, exp2Value;

    //return if the condition is true or false
    int execute(vector<string> **arr, int index, map<string, Command *> *nameCommandMap) {
        return 0;//    nameCommandsMap
    }
};

class WhileCommand : public Command {
public:
    int execute(vector<string> *arr, int index, map<string, Command *> *nameCommandMap) {
        vector<string> vs;
        int helperIndex = index + 3;
        while ((*arr)[helperIndex] != "}") {
            vs.push_back((*arr)[helperIndex]);
            helperIndex++;
        }
        int helper = index;
        int indexToReturn = 0;
        while ((*arr)[helper] != "}") {
            helper++;
            indexToReturn++;
        }
        indexToReturn++;
        int tempIndex = index + 2;
        vector<string> conditionPartsVector;
        int tempIndex2 = tempIndex;
        conditionPartsVector.push_back((*arr)[index + 1]);
        //while the condition is true execute the commands
        while (isTrueCondition(conditionPartsVector) == true) {
            cout << "here" << endl;
            int indexForExecuteCommand = 0;
            int size = vs.size();
            while (indexForExecuteCommand < size) {
                string s = vs.at(indexForExecuteCommand);
                if (nameCommandMap->find(s) != nameCommandMap->end()) {
                    Command *c = nameCommandMap->at(s);
                    indexForExecuteCommand += c->execute(&vs, indexForExecuteCommand, nameCommandMap);
                } else if (vs.at(index + 1) == "=") {
                    //command setVariable with queue.
                    Command *c = new SetVar;
                    indexForExecuteCommand += c->execute(&vs, indexForExecuteCommand, nameCommandMap);
                }
            }
            //go to first condition
            tempIndex = tempIndex2;
        }
        return indexToReturn;
    }
};

class IfCommand : public ConditionParser {
public:
    int execute(vector<string> *arr, int index, map<string, Command *> *nameCommandMap) {
        int tempIndex = index + 1;
        vector<string> conditionPartsVector;
        while ((*arr)[tempIndex] != "{") {
            conditionPartsVector.push_back((*arr)[tempIndex]);
            tempIndex++;
        }
        if (isTrueCondition(conditionPartsVector)) {
            while (!((*arr)[tempIndex] == "}")) {
                string s = (*arr)[tempIndex + 1];
                if ((*nameCommandMap).find(s) != (*nameCommandMap).end()) {
                    Command *c = (*nameCommandMap)[s];
                    tempIndex += c->execute(arr, tempIndex, nameCommandMap);
                } else if (symbolTable.find(s) != symbolTable.end()) {
                    Command *c = new DefineVarCommand;
                    tempIndex += c->execute(arr, tempIndex, nameCommandMap);
                }
            }
        }
        return tempIndex;
    }
};

class PrintCommand : public Command {
public:
    int execute(vector<string> *arr, int index, map<string, Command *> *nameCommandMap) {
        string s = (*arr)[index + 1];
        double toStringValue;
        int sizeOfInputForPrint = s.size();
        string toPrint;
        //if (sizeOfInputForPrint <= 2) {
        //   cout << "no input to print" << endl;
        //}
        if ((s[0] == '"' && (s[sizeOfInputForPrint - 1] == '"'))) {
            toPrint = s.substr(1, sizeOfInputForPrint - 2);
            cout << toPrint << endl;
        } else if ((s[0] != '"' && s[sizeOfInputForPrint - 1] != '"')) {
            //s not found
            if (symbolTable.find(s) == symbolTable.end()) {
                cout << "no valid input" << endl;
                return 0;
            } else if (symbolTable.find(s) != symbolTable.end()) {
                string simAdress = symbolTable.at(s);
                if (leftDirection.find(simAdress) != leftDirection.end()) {
                    toStringValue = leftDirection.at(simAdress);
                } else if (rightDirection.find(s) != rightDirection.end()) {
                    toStringValue = rightDirection.at(s)->getValue();
                }
                //convert from double to string
                toPrint = to_string(toStringValue);
                cout << toPrint << endl;
            }
        }
        return 2;
    }
};

class SleepCommand : public Command {
public:
    int execute(vector<string> *arr, int index, map<string, Command *> *nameCommandMap) {
        Interpreter *i = new Interpreter();
        string s = (*arr)[index + 1];
        Expression *e = i->interpret(s);
        if (e == nullptr) {
            cout << "e is a null pointer" << endl;
            return 0;
        }
        double timeForSleep = e->calculate();
        delete e;
        usleep(timeForSleep);
        return 2;
    }
};
