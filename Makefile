SRC= word_tree.c hash.c
OBJ= word_tree.o hash.o
   
autofill: ${OBJ} 
	cc -o autofill ${OBJ}
    
clean:
	rm -f ${OBJ} autofill Makefile.bak
    
install: autofill 
	cp autofill ~/bin

depend:
	makedepend -Y ${SRC} &> /dev/null
    
# DO NOT DELETE

word_tree.o: structs.h hash.h
