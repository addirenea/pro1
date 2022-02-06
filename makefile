all: tree

clean:
	rm tree

tree: tree.c
	gcc -Wall -g tree.c -o tree

run:
	./tree
