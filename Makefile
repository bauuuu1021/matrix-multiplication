CC = gcc
OBJ: parallel serial strassen_parallel opt random

%: %.c
	$(CC) -o $@ $< -fopenmp

run: $(OBJ)
	@./serial
	@./parallel
	@./strassen_parallel
	@./opt
	@echo "difference between serial.txt and parallel.txt :"
	@diff serial.txt parallel.txt
	@echo "difference between parallel.txt and strassen_parallel.txt :"
	@diff parallel.txt strassen_parallel.txt
	@echo "difference between strassen_parallel.txt and opt.txt :"
	@diff strassen_parallel.txt opt.txt

cache-clean:
	echo 1 | sudo tee /proc/sys/vm/drop_caches

.PHONY: clean
clean:
	-rm -f parallel serial strassen_parallel opt random *.txt
