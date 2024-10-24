all : main client

main : main.c
	gcc main.c libseg.a -o main -L. -lseg -Wall -Wextra -Werror -g

client : client.c
	gcc client.c libseg.a -o client -L. -lseg -Wall -Wextra -Werror -g