//
// Created by linor on 27/12/2019.
//

#ifndef EX3_VARINFO_H
#define EX3_VARINFO_H

#include <string>
using namespace std;

class VarInfo {
public:
    VarInfo(string name, string sim, string direction,double val);
   // VarInfo(string name,double val);
    void SetValue(double val);
    double getValue();
    string getSim();
private:
    string nameVar;
    string simAdd;
    string simDirection;
    double value;
};
#endif //EX3_VARINFO_H
