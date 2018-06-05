CC = gcc
OBJ: parallel serial strassen_serial strassen_parallel random

%: %.c
	$(CC) -o $@ $< -fopenmp

run: $(OBJ)
	@./serial
	@./parallel
	@./strassen_serial
	@./strassen_parallel

cache-clean:
	echo 1 | sudo tee /proc/sys/vm/drop_caches

clean: 
	-rm -f parallel serial strassen_serial strassen_parallel random 