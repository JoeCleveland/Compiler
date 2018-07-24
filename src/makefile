all: compiler.o parser.o translator.o symtable.o lexical.o
	g++ compiler.o parser.o translator.o symtable.o lexical.o -o compiler 
compiler.o: compiler.cpp
	g++ -I . -c compiler.cpp 
parser.o: parser.cpp parser.h
	g++ -I . -c parser.cpp
translator.o: translator.cpp translator.h
	g++ -I . -c translator.cpp 
symtable.o: symtable.cpp symtable.h
	g++ -I . -c symtable.cpp 
lexical.o: lexical.cpp lexical.h
	g++ -I . -c lexical.cpp
