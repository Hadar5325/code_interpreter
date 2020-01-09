//
// Created by linor on 09/01/2020.
//
#include "VarInfo.h"
#include <string>

// constructor of var info object
VarInfo::VarInfo(string name, string sim, string direction, double val) {
    this->nameVar = name;
    this->simAdd = sim;
    this->simDirection = direction;
    this->value = val;
}
// set value function
void VarInfo::SetValue(double val){
    this->value = val;
}
// getters
double VarInfo::getValue(){
    return this->value;
}
string VarInfo:: getSim(){
    return this->simAdd;
}