P2.out : Project2.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o
	g++ -g -o P2.out Project2.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o

Project2.o : Project2.cpp SetLimits.h SyntacticalAnalyzer.h
	g++ -g -c Project2.cpp

SetLimits.o : SetLimits.cpp SetLimits.h
	g++ -g -c SetLimits.cpp

LexicalAnalyzer.o : LexicalAnalyzer.save
	cp LexicalAnalyzer.save LexicalAnalyzer.o

SyntacticalAnalyzer.o : SyntacticalAnalyzer.cpp SyntacticalAnalyzer.h LexicalAnalyzer.h
	g++ -g -c SyntacticalAnalyzer.cpp

clean : 
	rm *.o P2.out *.gch

submit : Project2.cpp LexicalAnalyzer.h LexicalAnalyzer.save SyntacticalAnalyzer.h SyntacticalAnalyzer.cpp makefile README.txt
	rm -rf TeamHJP2
	mkdir TeamHJP2
	cp Project2.cpp TeamHJP2
	cp LexicalAnalyzer.h TeamHJP2
	cp LexicalAnalyzer.save TeamHJP2
	cp SyntacticalAnalyzer.h TeamHJP2
	cp SyntacticalAnalyzer.cpp TeamHJP2
	cp makefile TeamHJP2
	cp README.txt TeamHJP2
	tar cfvz TeamHJP2.tgz TeamHJP2
	cp TeamHJP2.tgz ~tiawatts/cs460drop
