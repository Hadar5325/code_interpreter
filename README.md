Created by Linor Salhov and Shira Kirsh.

Home work 3
Code Interperter

This is about a code interpreter that works with the flightGear simulator.

Files included in this project:

ex1.h		commands.h		Expression.h		Lexer.h		Parser.h
ex1.cpp		commands.cpp	Expression.cpp		Lexer.cpp	Parser.cpp
BoolExpression.h	Utils.h		VarInfo.h		main.cpp		fly.txt

NOTE: make sure you have passed valid arguments to FlightGear.
ex3
How to compile:
enter these two arguments in the Settings->Additional Settings box:

--generic=socket,out,10,nnn.nnn.nnn.nnn,5400,tcp,generic_small -
with IP of computer running interpreter (and put the included generic small xml file in the FlightGear installation folder "data/Protocol").   

--telnet=socket,in,10,nnn.nnn.nnn.nnn,5402,tcp -
with IP of computer running FlightGear.

How to run:
we support both command-line input and file input (including a command-line "run"
command which takes a file path as an argument).
The interpreter will run until "exit" command is inputed (via file or command-line).
Each argument passed via running the exexcutable file is read and executed command by command.

Make sure you run our Interpreter BEFORE running FlightGear!!!

The project is about implemnting an interpreter for new languagh that will work with a flight simulator.
We implemented a parser, a lexer and some more classes for the commands and expressions used in the languagh.

First the interpreter lexes the words belonged to the languagh.
Than it parses them into methods and behavior we want them to do.

For the commands of the languagh:

We implemented this by using the command design pattern and making a hashMap for all the words in the languagh
and matched them to an object from type Command:
we made an inteface of Command and all the classes that inheritted it should implement the method execute
When the interpreter will read the code it will lex it to tokens, then parse each of them to a command on the languagh
and run the execute method of that specific command.
The execute method will do what the command should do and return the number of parameters it should get so that we know
how many "words" we should jump over to reach the next command in the code.

for the variables in the languagh:
For each variable we keep its state (name, type, value, scope, simulator value etc.) and update its state whenever it get changed.

we support conditions, loops, functions, expressions.

during the entire running time of the interpreter (more presicley upon calling openDataServer command),
another thread runs in the background updating the symbol table with current values read from FlightGear.

NOTE: sleep command is implemented by sleeping the amount inputed in MILLISECONDS!

GitHub link:
https://github.com/sshiraa/code_interpreter

Enjoy your flight with us :)