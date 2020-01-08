//
// Created by linor on 25/12/2019.
//
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

using namespace std;

vector<string> toLex(string fileName) {
    fstream f("../" + fileName, ios::in);
    if (!f.is_open()) {
        cout << "nope" << endl;
    }
    int i, flag = 0;
    vector<string> tokensVector;
    string line, firstWord, word;
    string onePlaceInVector;
    while (getline(f, line)) {
        int sizeOfLine = line.size(), flag = 0;
        string con;
        string helper;
        char toRemove = '(', toRemove2 = ')', toRemoveSpace = ' ';
        std::string input = line;
        std::string firstWord = input.substr(0, input.find(" "));
        string specialCases = input.substr(0, input.find("("));
        //if the string contain space
        specialCases.erase(remove(specialCases.begin(), specialCases.end(), toRemoveSpace), specialCases.end());
        // input.erase(remove(input.begin(),input.end(),toRemove2),input.end());
        //remove spaces of specialCases string
        if(input.rfind("\t",0) == 0){
            line = input.substr(1);
        }
        if(input == "}"){
            tokensVector.push_back(input);
        }
        else if (specialCases == "openDataServer") {
            line.erase(remove(line.begin(), line.end(), toRemoveSpace), line.end());
            replace(line.begin(), line.end(), toRemove, ' ');
            replace(line.begin(), line.end(), toRemove2, ' ');
            stringstream ss(line);
            while (ss >> word) {
                word.erase(remove(word.begin(), word.end(), toRemoveSpace), word.end());
                tokensVector.push_back(word);
            }
        } else if (specialCases == "connectControlClient") {
            char toReplace = ',';
            //promise no spaces..
            line.erase(remove(line.begin(), line.end(), toRemoveSpace), line.end());
            //replace the operators with space
            replace(line.begin(), line.end(), toReplace, ' ');
            replace(line.begin(), line.end(), toRemove, ' ');
            replace(line.begin(), line.end(), toRemove2, ' ');
            stringstream ss(line);
            while (ss >> word) {
                word.erase(remove(word.begin(), word.end(), toRemoveSpace), word.end());
                tokensVector.push_back(word);
            }
        }
            //openDataServer and connection
            //check if is while or var command:
        else if (firstWord == "while") {
            flag = 1;
            tokensVector.push_back(firstWord);
            string con = line.substr(6,line.size()-7 );
            con.erase(remove(con.begin(), con.end(), toRemoveSpace), con.end());
            tokensVector.push_back(con);
            tokensVector.push_back("{");
        } else if (firstWord == "var") {
            flag = 1;
            replace(line.begin(), line.end(), '(', ' ');
            replace(line.begin(), line.end(), ')', ' ');
            stringstream ss(line);
            while (ss >> word) {
                tokensVector.push_back(word);
            }
        } else if (firstWord == "if") {
            flag = 1;
            //int indexFind = line.find("{");
            con = line.substr(3, line.size() - 5);
            tokensVector.push_back(con);
            helper = line.substr(sizeOfLine - 2, 1);
            tokensVector.push_back(helper);
        } else if (flag == 0) {
            string s = line.substr(0, 5);
            if (s == "Print" || s == "Sleep") {
                tokensVector.push_back(s);
                int findIndex = line.find("(");
                int findIndex2 = line.find(")");
                string strNew = line.substr(findIndex+1,findIndex2-findIndex-1);
                tokensVector.push_back(strNew);
            } else {
                size_t indexFind = line.find('=');
                string specialCases = line.substr(0, indexFind);
                specialCases.erase(remove(specialCases.begin(), specialCases.end(), toRemoveSpace), specialCases.end());
                tokensVector.push_back(specialCases);
                if(indexFind>0){
                    char char2 = line.at(indexFind);
                    //convert char to string
                    string str2(1, char2);
                    tokensVector.push_back(str2);
                    string str3 = line.substr(indexFind + 1);
                    str3.erase(remove(str3.begin(), str3.end(), toRemoveSpace), str3.end());
                    tokensVector.push_back(str3);
                }
            }
        }
    }
   /* for (int i = 0; i < tokensVector.size(); i++) {
        cout << tokensVector.at(i) << endl;
    }*/
    return tokensVector;
}

















/*
replace(line.begin(), line.end(), '(', ' ');
//replace(line.begin(), line.end(), ',', ' ');
replace(line.begin(), line.end(), ')', ' ');
stringstream ss(line);
while(getline(f,line)){
for (i = 0; i < line.length(); i++) {
ss>>token;
if (token.find('\"')) {

while(){

}
while ( != '\"') {
cout << line[i];
i++;
}
}
cout<<c;
}
}



/*   if(line.find("Print")){
       ss >> token;

   }


   while (ss >> token) {
       if (token.find('"')) {
           if (flag == 1) {

           } else if (flag == 0) {

               flag = 1;
           }
       }
       tokensVector.push_back(token);
   }

int size = tokensVector.size();
for (int i = 0; i < size; i++) {
cout << tokensVector[i] << endl;
}

return tokensVector;
*/