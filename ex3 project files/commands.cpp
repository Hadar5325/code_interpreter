//
// Created by linor on 09/01/2020.
//

//#include "commands.h"
#include<string>
#include <vector>
#include "commands.h"

bool Command::isTrueCondition(vector<string> &conditionVector) {
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
};
void OpenServerCommand:: createChronoSimvector(vector<string> *chronologicalSinAdd) {
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
int OpenServerCommand::execute(vector<string> *arr, int index, map<string, Command *> *nameCommandMap)
{
    vector<string> chronologicalSimAdd;
    createChronoSimvector(&chronologicalSimAdd);
    string s = (*arr)[index + 1];
    Interpreter *i = new Interpreter();
    Expression *e = i->interpret(s);
    double port = e->calculate();
    delete e;
    delete i;
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
    address.sin_port = htons(port);
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
    return 2;
};

bool should_stop = false;

void OpenServerCommand::readFromClient(int client_socket, vector<string> chronologicalSimAdd) {
    while (!should_stop) {
        char buffer[1024] = {0};
        int valread = read(client_socket, buffer, 1024);
        char delim = ',';
        char *ptr = buffer;
        vector<string> vec;
        string word;
        while (*ptr != '\n' && *ptr != '\r') {
            while ((*ptr != delim) && (*ptr != '\n') && (*ptr != '\r')) {
                word += *ptr;
                ptr++;
            }
            vec.push_back(word);
            word = "";
            if (*ptr != '\n' && *ptr != '\r') {
                ptr++;
            }
        }
        string s, s1;
        // SinletonMutex *sm = SinletonMutex::getInstance();
        //sm->LockMutex();
        //MutexSingleton* ms;
        // ms->GetInstance()->getMutex();
        // ( ms->GetInstance()->lockMutex());
        for (int i = 0; i < 36; i++) {
            s = chronologicalSimAdd[i];
            s1 = vec[i];
            if (leftDirection.find(s) != leftDirection.end()) {
                leftDirection[s] = stod(s1);
                int key = 0;
                for (auto &pair : symbolTable) {
                    string sym = pair.second;
                    if (sym == s) {
                        string key = pair.first;
                        string forSet = key + "=" + s1;
                        i1.setVariables(forSet);
                    }
                }
            }
        }

        //  sm->UnlockMutex();
        // ms->GetInstance()->unlockMutex();
        //  pthread_mutex_unlock(ms->getMutex());
        /*for (auto &pair : leftDirection)
            cout << "key: " << pair.first << " value: " << pair.second << endl; */
    }
}
  /*  void writeToClient(int client_socket, queue<string> queueToConnectCommand)
    {
        //if message exist in queue, send it to simulator
        while (!queueToConnectCommand.empty()) {
            int size = queueToConnectCommand.size();
            for (int i = 0; i < size; i++) {
                string message = queueToConnectCommand.front();
                queueToConnectCommand.pop();
                ssize_t return_val;
                return_val = write(client_socket, message.c_str(), message.length());
                if (return_val == -1) {
                    cout << "error" << endl;
                } else {
                    cout << "it is sent" << endl;
                }
            }
        }
    }
*/

    void ConnectCommand :: writeToClient(int client_socket, queue<string> queueToConnectCommand)
    {
        //if message exist in queue, send it to simulator
        while (!queueToConnectCommand.empty()) {
            int size = queueToConnectCommand.size();
            for (int i = 0; i < size; i++) {
                string message = queueToConnectCommand.front();
                queueToConnectCommand.pop();
                ssize_t return_val;
                return_val = write(client_socket, message.c_str(), message.length());
                if (return_val == -1) {
                    cout << "error" << endl;
                } else {
                    cout << "it is sent" << endl;
                }
            }
        }
    }
int  SleepCommand ::execute(vector<string> *arr, int index, map<string, Command *> *nameCommandMap) {
    Interpreter *i = new Interpreter();
    string s = (*arr)[index + 1];
    Expression *e = i->interpret(s);
    if (e == nullptr) {
        cout << "e is a null pointer" << endl;
        return 0;
    }
    long timeForSleep = e->calculate();
    delete e;
    delete i;
    this_thread::sleep_for(chrono::milliseconds(timeForSleep));
    return 2;
}
int PrintCommand:: execute(vector<string> *arr, int index, map<string, Command *> *nameCommandMap) {
    string s = (*arr)[index + 1];
    double toStringValue;
    int sizeOfInputForPrint = s.size();
    string toPrint;
    if ((s[0] == '"' && (s[sizeOfInputForPrint - 1] == '"'))) {
        if (sizeOfInputForPrint <= 2) {
            cout << "no input to print" << endl;
        }
        toPrint = s.substr(1, sizeOfInputForPrint - 2);
        cout << toPrint << endl;
    } else if ((s[0] != '"' && s[sizeOfInputForPrint - 1] != '"')) {
        //s not found
        if (symbolTable.find(s) == symbolTable.end()) {
            Expression *e4 = nullptr;
            e4 = i1.interpret(s);
            toStringValue = e4->calculate();
            delete e4;
        } else if (symbolTable.find(s) != symbolTable.end()) {
            string simAdress = symbolTable.at(s);
            if (leftDirection.find(simAdress) != leftDirection.end()) {
                toStringValue = leftDirection.at(simAdress);
            } else if (rightDirection.find(s) != rightDirection.end()) {
                toStringValue = rightDirection.at(s)->getValue();
            }
        }
        //convert from double to string
        toPrint = to_string(toStringValue);
        cout << toPrint << endl;
    }
    return 2;
}
int IfCommand:: execute(vector<string> *arr, int index, map<string, Command *> *nameCommandMap) {
    int indexToReturn = 0, tempIndex;
    vector<string> conditionPartsVector, vs;
    int helper = index;
    while ((*arr)[helper] != "}") {
        helper++;
        indexToReturn++;
    }
    indexToReturn++;
    int helperIndex = index + 3;
    while ((*arr)[helperIndex] != "}") {
        vs.push_back((*arr)[helperIndex]);
        helperIndex++;
    }
    conditionPartsVector.push_back((*arr)[index + 1]);
    if (isTrueCondition(conditionPartsVector)) {
        int indexForExecuteCommand = 0;
        int size = vs.size();
        while (indexForExecuteCommand < size) {
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

    return indexToReturn;
}
int WhileCommand :: execute(vector<string> *arr, int index, map<string, Command *> *nameCommandMap) {
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
int ConditionParser:: execute(vector<string> **arr, int index, map<string, Command *> *nameCommandMap) {
    return 0;//    nameCommandsMap
}
int SetVar:: execute(vector<string> *arr, int index, map<string, Command *> *nameCommandMap) {
    string varName = ((*arr)[index]), sim;
    double newVal;
    Expression *e2 = nullptr;
    e2 = i1.interpret((*arr)[index + 2]);
    newVal = e2->calculate();
    delete e2;
    if (symbolTable.find(varName) != symbolTable.end()) {
        sim = symbolTable.at(varName);
    }
    if (rightDirection.find(varName) != rightDirection.end()) {
        rightDirection[varName]->SetValue(newVal);
        string message = "set" + sim + " " + to_string(newVal) + "\r\n";
        queueToConnectCommand.push(message);
    }
    i1.setVariables(varName + "=" + to_string(newVal));
    return 3;
}
int DefineVarCommand:: execute(vector<string> *arr, int index, map<string, Command *> *nameCommandMap) {
    //interpreter
    string varName = (*arr)[index + 1];
    if ((*arr)[index + 2] == "->" || (*arr)[index + 2] == "<-") {
        string simAdress = (*arr)[index + 4];
        string simsub = simAdress.substr(1, simAdress.length() - 2);
        //check if the the varName exist in the map,if yes,update all the maps by the pointer
        if ((*arr)[index + 2] == "->") {
            VarInfo *vI = new VarInfo(varName, simsub, (*arr)[index + 2], 0);
            rightDirection.insert(pair<string, VarInfo *>(varName, vI));
            symbolTable.insert(pair<string, string>(varName, simsub));
            i1.setVariables(varName + "=" + to_string(0));
        } else if ((*arr)[index + 2] == "<-") {
            leftDirection.insert(pair<string, double>(simsub, 0));
            symbolTable.insert(pair<string, string>(varName, simsub));
            i1.setVariables(varName + "=" + to_string(0));
        }
        return 5;
    } else if ((*arr)[index + 2] == "=") {
        string sim;
        double newValue;
        if (symbolTable.find(varName) != symbolTable.end()) {
            sim = symbolTable[varName];
        }
        string newValueString = (*arr)[index + 3];
        Expression *e5 = nullptr;
        e5 = i1.interpret(newValueString);
        newValue = e5->calculate();
        delete e5;
        if (rightDirection.find(varName) != rightDirection.end()) {
            rightDirection[varName]->SetValue(newValue);
        }
        i1.setVariables(varName + "=" + to_string(newValue));
        return 4;
    }
}
int ConnectCommand::execute(vector<string> *arr, int index, map<string, Command *> *nameCommandMap) {
    string locS = (*arr)[index + 1];
    string s = (*arr)[index + 2];
    const char *c_s = s.c_str();
    Interpreter *i = new Interpreter();
    Expression *e = i->interpret(s);
    double port = e->calculate();
    delete e;
    delete i;
    const char *c_locS = locS.c_str();
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
    address.sin_addr.s_addr = inet_addr("127.0.0.1");  //the localhost address
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
    thread thread2(&ConnectCommand::writeToClient, this, client_socket, queueToConnectCommand);
    thread2.detach();
    close(client_socket);
    return 3;
}





