
# shell_group26:main.c cmds.o ps.o ls.o
# 	gcc main.c -lreadline cmds.o ps.o ls.o -o shell_group26

# cmds.o: cmds.c
# 	gcc -c cmds.c 

# ps.o: ps.c
# 	gcc -c ps.c

# ls.o: ls.c
# 	gcc -c ls.c

# clean:
# 	rm *.o

objects = main.o cmds.o commands.o

edit: $(objects)
	cc -o -lreadline -edit ${objects}

main.o: cmds.h
cmds.o: cmds.h commands.h
commands.o: commands.h

.PHONY: clean
clean:
	rm edit ${objects}
