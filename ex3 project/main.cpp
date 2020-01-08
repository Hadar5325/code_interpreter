#include <string>
#include <vector>
#include "Lexer.cpp"
#include "Utils.h"
#include "Parser.h"
using namespace std;

int main(int argc, char *argv[]) {
    vector<string> lexedVector;
    Parser *p = new Parser();
    //string file name
    string filename(argv[1]);
    lexedVector = toLex(filename);
    p->parse(&lexedVector);
    return 0;
}