CC = gcc
OBJ: parallel serial strassen_serial strassen_parallel random

%: %.c
	$(CC) -o $@ $< -fopenmp

run: $(OBJ)
	@./random 1024 input.txt
	@./serial
	@./parallel
	@./strassen_serial
	@./strassen_parallel
	@echo "difference between serial.txt and parallel.txt :"
	@diff serial.txt parallel.txt
	@echo "difference between parallel.txt and strassen_serial.txt :"
	@diff parallel.txt strassen_serial.txt
	@echo "difference between strassen_serial.txt and strassen_parallel.txt :"
	@diff strassen_serial.txt strassen_parallel.txt 

cache-clean:
	echo 1 | sudo tee /proc/sys/vm/drop_caches

.PHONY: clean
clean:
	-rm -f parallel serial strassen_serial strassen_parallel random *.txt
