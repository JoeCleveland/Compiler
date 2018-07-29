all: compiler.o parser.o translator.o symtable.o lexical.o codegen.o
	g++ compiler.o parser.o translator.o symtable.o lexical.o codegen.o -o compiler 
compiler.o: compiler.cpp
	g++ -std=c++11 -I . -c compiler.cpp 
parser.o: parser.cpp parser.h
	g++ -std=c++11 -I . -c parser.cpp
translator.o: translator.cpp translator.h
	g++ -std=c++11 -I . -c translator.cpp 
symtable.o: symtable.cpp symtable.h
	g++ -std=c++11 -I . -c symtable.cpp 
lexical.o: lexical.cpp lexical.h
	g++ -std=c++11 -I . -c lexical.cpp
codegen.o: codegen.cpp codegen.h
	g++ -std=c++11 -I . -c codegen.cpp
