all: compiler.o parser.o translator.o symtable.o lexical.o codegen.o
	g++ compiler.o parser.o translator.o symtable.o lexical.o codegen.o -o compiler -g
compiler.o: compiler.cpp
	g++ -std=c++11 -I . -c compiler.cpp -g
parser.o: parser.cpp parser.h
	g++ -std=c++11 -I . -c parser.cpp -g
translator.o: translator.cpp translator.h
	g++ -std=c++11 -I . -c translator.cpp -g
symtable.o: symtable.cpp symtable.h
	g++ -std=c++11 -I . -c symtable.cpp -g
lexical.o: lexical.cpp lexical.h
	g++ -std=c++11 -I . -c lexical.cpp -g
codegen.o: codegen.cpp codegen.h
	g++ -std=c++11 -I . -c codegen.cpp -g
