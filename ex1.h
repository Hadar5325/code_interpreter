#include <iostream>
#include <stack>
#include <queue>
#include <map>
#include <algorithm>
#include <regex>
#include "Expression.h"
using namespace std;

class Value : public Expression
{
public:
	Value(double num);
	double calculate();		
public:
	double number;
};

class Variable : public Expression
{
public:
	Variable(string s, double num);
	virtual ~Variable();
	double calculate();
	Variable& operator++ ();
	Variable& operator++(int);
	Variable& operator--(int);
	Variable& operator--();
	Variable& operator+=(double);
	Variable& operator-=(double);
private:
	double value;
	string name;
};

class BinaryOperator :public Expression
{
protected:
	Expression *left,*right;
public:
	BinaryOperator(Expression *e1, Expression *e2);
	BinaryOperator();
	virtual ~BinaryOperator();
	double calculte();
};

class UnaryOperator :public Expression
{
protected:
	Expression *expression;
public:
	UnaryOperator(Expression *e);
	UnaryOperator(); 
	virtual ~UnaryOperator();
};

class Plus :public BinaryOperator
{
public:
	Plus(Expression *e1, Expression *e2);
	double calculate();
};

class Minus :public BinaryOperator
{
public:
	Minus(Expression *e1, Expression *e2);
	double calculate();
};

class Mul : public BinaryOperator
{
public:
	Mul(Expression *e1, Expression *e2);
 	double calculate();
};

class Div : public BinaryOperator
{
public:
	Div(Expression *e1, Expression *e2);
	double calculate();
};

class UPlus : public UnaryOperator
{
public:
	UPlus(Expression *e);
	double calculate();
};

class UMinus : public UnaryOperator
{
public:
	UMinus(Expression* e);
    ~UMinus();
	double calculate();
};

class Interpreter {
public:
	Interpreter();
	~Interpreter();
	Expression* interpret(string s);
	vector<string> separateByVarsAndValue(string givenExpressionString);
	bool validExpression(string s);
	void setVariables(string s);
	void evaluate(string s);
	Expression* createPtrExp();
	bool isNumber(string s);
	int applyOp(int a, int b, char op);
	int precedence(char op);
	std::vector<string> separateFunc(string s, char delimiter);
	bool isVariable(string s);
private:
	queue<string> values;
	stack<Expression*> helperStack;
	map<string, double> variablesMap;
};