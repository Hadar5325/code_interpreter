#include <iostream>
#include <regex>
#include <stack>
#include <queue>
#include <map>
#include <algorithm>
#include "ex1.h"
#include "Expression.h"
using namespace std;

//values:
Value::Value(double num) {
	this->number = num;
}

double Value::calculate() {
	return this->number;
}

//variables:
Variable::Variable(string s, double num) {
	this->value = num;
	this->name = s;
}

Variable::~Variable() {}
double Variable::calculate() {
	return this->value;
}
//variable operator loading:
Variable& Variable:: operator ++() {
	this->value = this->value + 1;
	return *this;
}
Variable& Variable::operator++(int) {
	this->value = this->value + 1;
	return *this;
}
Variable& Variable::operator--(int) {
	this->value = this->value - 1;
	return *this;
}
Variable& Variable::operator--() {
	this->value = this->value - 1;
	return *this;
}
Variable& Variable::operator+=(double a){
	this->value = this->value + a;
	return *this;
}
Variable& Variable:: operator-=(double a){
	this->value = this->value - a;
	return *this;
}

//UnaryOperator:
UnaryOperator::UnaryOperator(Expression *e) {
	this->expression = e;
}

UnaryOperator::~UnaryOperator(){
	if (this->expression != NULL) {
		delete this->expression;
	}
}

//BinaryOperator:
BinaryOperator::BinaryOperator(Expression *e1, Expression *e2) {
	this->left = e1;
	this->right = e2;
}

BinaryOperator::~BinaryOperator() {
	if (this->left != NULL) {
		delete this->left;
	}
	if (this->right != NULL) {
		delete this->right;
	}
}

//UMinus:
UMinus::UMinus(Expression *e) : UnaryOperator(e) {}
UMinus::~UMinus() {}
double UMinus::calculate() {
	return 0 - this->expression->calculate();
}


//UPlus:
UPlus::UPlus(Expression *e) : UnaryOperator(e) {}
double UPlus::calculate() {
	return this->expression->calculate();
}

//Mul:
Mul::Mul(Expression *e1, Expression *e2) :BinaryOperator(e1, e2) {} 
double Mul::calculate() {
	return this->left->calculate() * this->right->calculate();
}

//Div:
Div::Div(Expression *e1, Expression *e2) : BinaryOperator(e1, e2) {}
double Div::calculate() {
	if (this->right->calculate() == 0) {
		throw "division by zero";
	}
	return this->left->calculate() / this->right->calculate();
}

//Plus:
Plus::Plus(Expression *e1, Expression *e2) :BinaryOperator(e1, e2) {}
double Plus::calculate() {
	return this->left->calculate() + this->right->calculate() ;
}

//Minus:
Minus::Minus(Expression *e1, Expression *e2) :BinaryOperator(e1, e2) {}
double Minus::calculate() {
	return this->left->calculate() - this->right->calculate();
}


Interpreter::Interpreter() {
}

Interpreter::~Interpreter() {
}

Expression* Interpreter::interpret(string s) {
	this->evaluate(s);
	Expression *e = createPtrExp();
	return e;
}

void Interpreter::setVariables(string s) {
	std::vector<string> vs;
	vs = separateFunc(s, ';');
	size_t i;
	for (i = 0; i < vs.size(); i++) {
		vector<string> tempVs = separateFunc(vs[i], '=');
		if (!isVariable(tempVs[0])) {
			throw "illegal variable assignment!";
		}
		if (!this->isNumber(tempVs[1])) {
			throw "illegal variable assignment!";
		}
		std::map<string, double>::iterator it = (this->variablesMap).find(tempVs[0]);
		if (it == this->variablesMap.end()) {
			this->variablesMap.insert(std::pair<string, double>(tempVs[0], atof(tempVs[1].c_str())));
		}
		else {
			this->variablesMap.find(tempVs[0])->second = atof(tempVs[1].c_str());
		}
	}
}

vector<string> Interpreter::separateFunc(string s, char delimiter) {
	std::vector<string> vars;
	string token;
	size_t pos = 0;
	while ((pos = s.find(delimiter)) != string::npos) {
		token = s.substr(0, pos);
		vars.push_back(token);
		s.erase(0, pos + 1);
	}
	token = s.substr(0, pos);
	vars.push_back(token);
	return vars;
}
//return the length of the number or -1 if the number is not valid
bool Interpreter::isNumber(string s) {
	regex r("[0-9]+(\\.[0-9]+)?$");
	bool res = regex_match(s, r);
	return res;
}

vector<string> Interpreter::separateByVarsAndValue(string givenExpressionString) {
	unsigned int i;
	vector<string> tokens;
	vector<string> chars = { "$","*","+","-","/" };
	size_t len = givenExpressionString.length();
	if(isNumber(givenExpressionString)){
        tokens.push_back(givenExpressionString);
        return tokens;
	}
	for (i = 0; i < len; i++) {
		char saveItAsChar = givenExpressionString[i];
		string stemp(1, givenExpressionString[i]);
		string c;
		if (isNumber(stemp) || (saveItAsChar >= 'a' && saveItAsChar <= 'z') || (saveItAsChar >= 'A' && saveItAsChar <= 'Z')) {
			int tempA = stemp.find("$"), tempB = stemp.find("*"), tempC = stemp.find("/"), tempD = stemp.find("-");
			int tempE = stemp.find("+"), tempF = stemp.find("("), tempG = stemp.find(")");
			do {
				c += stemp[0];
				if (i >= len) {
					break;
				}
				stemp = givenExpressionString[i + 1];
				i++;
				tempA = stemp.find("$");
				tempB = stemp.find("*");
				tempC = stemp.find("/");
				tempD = stemp.find("-");
				tempE = stemp.find("+");
				tempF = stemp.find("(");
				tempG = stemp.find(")");
			} while (tempA < 0 && tempB < 0 && tempC < 0 && tempD < 0 && tempE < 0 && tempF < 0 && tempG < 0);
			i--;
			tokens.push_back(c);
		}
		else {
			tokens.push_back(stemp);
		}
	}
	return tokens;
}
int Interpreter::precedence(char op) {
	if (op == '+' || op == '-')
		return 1;
	if (op == '*' || op == '/')
		return 2;
	if (op == ')')
		return 3;
	return 0;
}

// The maths
int Interpreter::applyOp(int a, int b, char op) {
	switch (op) {
	case '+': return a + b;
	case '-': return a - b;
	case '*': return a * b;
	case '/': return a / b;
	}
	return 0;
}

bool Interpreter::isVariable(string s) {
	regex r("^[a-zA-Z][a-zA-Z0-9_]*$");
	bool result = regex_match(s, r);
	return result;
}
bool Interpreter::validExpression(string s) {
	size_t size = s.size();
	int counter1 = 0, counter2 = 0;
	vector<char> operators = { '/','*','+','-' };
	for (unsigned int i = 0; i < size; i++) {
		char current = s[i];

		if (current == '(') {
			counter1++;
			continue;
		}
		else if (current == ')') {
			counter2++;
			continue;
		}
		if (i != 0) {
			char p = s[i - 1];
			if (find(std::begin(operators), end(operators), p) != end(operators) &&
				std::find(std::begin(operators), std::end(operators), current) != std::end(operators)) {
				return false;
			}
		}

		if (!((current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z')))
		{
			if (!isdigit(current)) {
				if (!(find(std::begin(operators), end(operators), current) != end(operators))) {
					if (current != '_' && current != '.') {
						return false;
					}
				}
			}
		}
	}
	if (counter1 != counter2) {
		return false;
	}
	return true;
}

void Interpreter::evaluate(string givenExpressionString) {
	vector<string> tokens = separateByVarsAndValue(givenExpressionString);
	string str = string(tokens.front());
	if (validExpression(givenExpressionString) == false) { throw "illegal math expression"; }
	//if uminus expression recognized,replace with this char:
	char uminus = '$';
	//length of string
	size_t lengthOfTokens = tokens.size();
	unsigned int i;
	// stack to store operators. 
	stack <char> ops;
	for (i = 0; i < lengthOfTokens; i++) {
		string current = tokens[i];
		if (current[0] == ' ') {
			continue;
		}
		else if (current[0] == '-') {
			if (i == 0) {
				ops.push(uminus);
				continue;
			}
			else if ((tokens[i - 1])[0] == '(') {
				ops.push(uminus);
				continue;
			}
		}
		else if (current[0] == '(') {
			ops.push(current[0]);
			continue;
		}
		else if (current[0] == ')') {
			if (ops.size() != 0) {
				while (ops.top() != '(') {
					if (!ops.empty()) {
						string top(1, ops.top());
						ops.pop();
						values.push(top);
					}
				}
				ops.pop();
			}

		}
		else if (current[0] == '*' || current[0] == '/') {

			ops.push(current[0]);
			continue;
		}
		if (current[0] == '+' || current[0] == '-') {
			if (!ops.empty()) {
				char topOperatorFromOps = ops.top();
				string topOperatorFromOpsString(1, ops.top());
				while (!ops.empty() && precedence(topOperatorFromOps) > precedence(current[0])) {
					topOperatorFromOpsString = ops.top();
					ops.pop();
					this->values.push(topOperatorFromOpsString);
				}
				ops.push(current[0]);
			}
			else if (ops.empty()) {
				ops.push(current[0]);
			}
		}
		else if (isNumber(current)) {
			values.push(current);
			if (!ops.empty()) {
				if (ops.top() == uminus) {
					string s(1, uminus);
					values.push(s);
					ops.pop();
				}
			}
			continue;
		}
		else {
			values.push(current);
		}
	}
	while (!ops.empty()) {
		string top(1, ops.top());
		ops.pop();
		values.push(top);
	}
}

Expression* Interpreter::createPtrExp() {
	Expression *exp = nullptr, *last = nullptr, *last2 = nullptr;
	vector<string> chars = { "$","*","+","-","/" };
	while (!values.empty()) {
		string currStringInPostfix = values.front();
        if(values.size() == 1){
            if(isNumber(currStringInPostfix)){
                string number = currStringInPostfix;
                double a = atoi(number.c_str());
                this->helperStack.push(new Value(a));
            }
        }
		values.pop();

		if (isNumber(currStringInPostfix)) {
			char *cstr = new char[currStringInPostfix.length() + 1];
			strcpy(cstr, currStringInPostfix.c_str());			
			this->helperStack.push(new Value(atof(cstr)));
		}
		//variable,contained in map:
		else if (!variablesMap.empty()) {
			if (variablesMap.count(currStringInPostfix) > 0) {
				this->helperStack.push(new Variable(currStringInPostfix, variablesMap.find(currStringInPostfix)->second));
			}
		}
		//else if current is an operator:
		if (find(begin(chars), end(chars), currStringInPostfix) != end(chars)) {
			last = this->helperStack.top();
			this->helperStack.pop();
			if (currStringInPostfix[0] != '$' && !this->helperStack.empty())
			{
				last2 = this->helperStack.top();
				this->helperStack.pop();
			}
			if (currStringInPostfix[0] == '$') {
				exp = new UMinus(last);
			}
			else if (currStringInPostfix[0] == '+') {
				exp = new Plus(last2, last);
			}
			else if (currStringInPostfix[0] == '-') {
				exp = new Minus(last2, last);
			}
			else if (currStringInPostfix[0] == '*') {
				exp = new Mul(last2, last);
			}
			else if (currStringInPostfix[0] == '/') {
				if (last->calculate() == 0) {
					throw "devision by zero";
				}
				else {
					exp = new Div(last2, last);
				}
			}
			this->helperStack.push(exp);
		}
	}

	return this->helperStack.top();
}


