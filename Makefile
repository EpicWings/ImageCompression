build:
	gcc -std=gnu11 -o quadtree main.c functions.c
run:
	./quadtree
clean:
	rm quadtree
valgrind:
	valgrind --leak-check=yes --track-origins=yes ./quadtree