CC = gcc-6 -fopenmp -std=c99

congruences_test: congruences.c test/congruences_test.c test/congruences_test.h arith_utils.c prime_gen.c
	$(CC) -g prime_gen.c arith_utils.c congruences.c test/congruences_test.c -o congruences_test

arith_utils_test: arith_utils.c test/arith_utils_test.c test/arith_utils_test.h prime_gen.c
	$(CC) -g prime_gen.c arith_utils.c test/arith_utils_test.c -o arith_utils_test

prime_gen_test: prime_gen.c test/prime_gen_test.c test/prime_gen_test.h
	$(CC) -g prime_gen.c test/prime_gen_test.c -o prime_gen_test

test: prime_gen_test arith_utils_test congruences_test
	./prime_gen_test
	./arith_utils_test
	./congruences_test

clean:
	rm -f prime_gen_test arith_utils_test congruences_test
	rm -rf prime_gen_test.dSYM arith_utils_test.dSYM congruences_test.dSYM
	
