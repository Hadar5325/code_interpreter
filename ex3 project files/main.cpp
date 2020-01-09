#include <string>
#include <vector>
#include "Lexer.h"
#include "Utils.h"
#include "Parser.h"
#include "SinletonMutex.h"

//SinletonMutex* SinletonMutex::instance = nullptr;

int main(int argc, char *argv[]) {
    vector<string> lexedVector;
    Parser *p = new Parser();
    //string file name
    string filename(argv[1]);
    // lex the vector:
    Lexer* lexer = new Lexer();
    lexedVector = lexer->toLex(filename);
    // parse it :
    p->parse(&lexedVector);
    return 0;
}