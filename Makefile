

arith_utils_test: arith_utils.c arith_utils_test.c prime_gen.c
	gcc -g prime_gen.c arith_utils.c arith_utils_test.c -o arith_utils_test

prime_gen_test: prime_gen.c prime_gen_test.c
	gcc -g prime_gen.c prime_gen_test.c -o prime_gen_test

test: prime_gen_test arith_utils_test
	./prime_gen_test
	./arith_utils_test