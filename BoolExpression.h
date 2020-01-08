//
// Created by linor on 29/12/2019.
//

#ifndef EX3_BOOLEXPRESSION_H
#define EX3_BOOLEXPRESSION_H


class BoolExpression {
public:
BoolExpression(map<string,VarInfo*> &symbolT,vector<string> &condVector){
    this->exp1 = condVector.at(0);
    this->oper = condVector.at(1);
    this->exp2 = condVector.at(2);
    this->vars = symbolT;
}

bool trueOrFalseExp(){
    if(vars.find(this->exp1) == vars.end()){
        double valueOfExp = vars.at(this->exp1)->getValue();
    }
}

private:
    string exp1;
    string exp2;
    string oper;
    map<string,VarInfo*> vars;
};


#endif //EX3_BOOLEXPRESSION_H
