# CMSC 15200 Project 1

.PHONY: clean

p36: project1.h project1-utils.c p36.c
	clang -Wall -o p36 project1-utils.c p36.c -lm

p63: project1.h project1-utils.c p63.c
	clang -Wall -o p63 project1-utils.c p63.c -lm

pz3: project1.h project1-utils.c pz3.c
	clang -Wall -o pz3 project1-utils.c pz3.c -lm

p3z: project1.h project1-utils.c p3z.c
	clang -Wall -o p3z project1-utils.c p3z.c -lm

# These targets include -g for debugging.

dp36: project1.h project1-utils.c p36.c
	clang -g -Wall -o p36 project1-utils.c p36.c -lm

dp63: project1.h project1-utils.c p63.c
	clang -g -Wall -o p63 project1-utils.c p63.c -lm

dpz3: project1.h project1-utils.c pz3.c
	clang -g -Wall -o pz3 project1-utils.c pz3.c -lm

dp3z: project1.h project1-utils.c p3z.c
	clang -g -Wall -o p3z project1-utils.c p3z.c -lm

# Type "make push" to run three git phases.

push:
	git add *.h *.c && git commit -m 'proj1 via make' && git push

# Remove executables and debugging info (in *dSYM directories).

clean:
	rm -rf p36 p63 pz3 p3z a.out *.dSYM
