CFLAGS= -Wall -Werror -std=gnu++11 -pedantic -ggdb3

myShell: ffosh.cpp my_shell.cpp shell_cmd.cpp
	g++ $(CFLAGS) -o ffosh ffosh.cpp my_shell.cpp shell_cmd.cpp

clean:
	rm -f test *~
