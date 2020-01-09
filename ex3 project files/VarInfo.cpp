//
// Created by linor on 09/01/2020.
//
#include "VarInfo.h"
#include <string>

VarInfo::VarInfo(string name, string sim, string direction, double val) {
    this->nameVar = name;
    this->simAdd = sim;
    this->simDirection = direction;
    this->value = val;
}
/*
VarInfo::VarInfo(string name,double val){
this->nameVar = name;
this->value = val;
}*/
void VarInfo::SetValue(double val){
    this->value = val;
}
double VarInfo::getValue(){
    return this->value;
}
string VarInfo:: getSim(){
    return this->simAdd;
}