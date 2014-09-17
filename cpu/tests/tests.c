#include <stdlib.h>
#include "seatest.h"
#include "tcb-funciones.h"
#include "resultados.h"

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
	char* registro = malloc(sizeof(char) + 1);
	int numero = 1, resultado = 1;
	*registro = 'a';
	resultado = copiar_numero_en_registro(tcb, registro, numero);
	assert_int_equal(1, tcb->a);
	assert_int_equal(OK, resultado);
	free(tcb);
	free(registro);
}

void test_fail_poner_1_en_registro_h()
{
	tcb_t* tcb = malloc(sizeof(tcb_t));
	char* registro = malloc(sizeof(char) + 1);
	int numero = 1, resultado = 1;
	*registro = 'h';
	resultado = copiar_numero_en_registro(tcb, registro, numero);
	assert_int_equal(FALLO, resultado);
	free(tcb);
	free(registro);
}

void test_fixture_copiar_numero_en_registro( void )
{
        test_fixture_start();
        run_test(test_poner_1_en_registro_a);
        run_test(test_fail_poner_1_en_registro_h);
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
