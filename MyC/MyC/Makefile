all :	lang

syntax : lexic	lang.y
	bison -v -y  -d  lang.y
lexic : lang.l
	flex lang.l


PCode/PCode.o : PCode/PCode.c PCode/PCode.h
	cd PCode; make pcode

lang		:	syntax Table_des_symboles.c PCode/PCode.o
	gcc -o lang lex.yy.c y.tab.c PCode/PCode.o Table_des_symboles.c

clean		:	
	rm -f lex.yy.c *.o y.tab.h y.tab.c lang *~ y.output; cd PCode; make clean

