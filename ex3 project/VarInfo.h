//
// Created by linor on 27/12/2019.
//

#ifndef EX3_VARINFO_H
#define EX3_VARINFO_H


class VarInfo {
public:
    VarInfo(string name, string sim, string direction) {
        this->nameVar = name;
        this->simAdd = sim;
        this->simDirection = direction;
    }

    VarInfo(string name,double val){
        this->nameVar = name;
        this->value = val;
    }

    void SetValue(double val){
        this->value = val;
    }
    double getValue(){
        return this->value;
    }
    string getSim(){
        return this->simAdd;
    }
private:
    string nameVar;
    string simAdd;
    string simDirection;
    double value;
};
#endif //EX3_VARINFO_H
