#include <stdlib.h>
#include "seatest.h"
#include "tcb-funciones.h"

//
// create a test...
//
void test_hello_world()
{
        char *s = "hello world!";
        assert_string_equal("hello world!", s);
        assert_string_contains("hello", s);
        assert_string_doesnt_contain("goodbye", s);
        assert_string_ends_with("!", s);
        assert_string_starts_with("hell", s);
}

//
// put the test into a fixture...
//
void test_fixture_hello( void )
{
        test_fixture_start();
        run_test(test_hello_world);
        test_fixture_end();
}

void test_poner_1_en_registro_a()
{
	tcb_t* tcb = malloc(sizeof(tcb_t));
	tcb->a = 0;
	char* registro = malloc(sizeof(char) + 1);
	*registro = 'a';
	int numero = 1;
	copiar_numero_en_registro(tcb, registro, &numero);
	assert_int_equal(1, tcb->a);
	free(tcb);
}

void test_fixture_copiar_numero_en_registro( void )
{
        test_fixture_start();
        run_test(test_poner_1_en_registro_a);
        test_fixture_end();
}


//
// put the fixture into a suite...
//
void all_tests( void )
{
        test_fixture_hello();
        test_fixture_copiar_numero_en_registro();
}
