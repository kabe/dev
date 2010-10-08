all : c d cpp dpp

clean :
	-$(RM) c d cpp dpp *~

c : c.c
	gcc -o $@ $<

d : c.c
	gcc -DNDEBUG -o $@ $<

cpp : cpp.cpp
	g++ -o $@ $<

dpp : cpp.cpp
	g++ -DNDEBUG -o $@ $<


