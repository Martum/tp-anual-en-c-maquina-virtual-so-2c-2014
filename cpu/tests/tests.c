#include <stdlib.h>
#include "seatest.h"
#include "tcb-funciones.h"
#include "resultados.h"

void test_actualizar_valor_en_registro_a()
{
	tcb_t* tcb = malloc(sizeof(tcb_t));
	actualizar_valor_en_registro(tcb, 'a', 20);
	assert_int_equal(20, tcb->a);
	free(tcb);
}

//void test_fail_actualizar_valor_en_registro_h()
//{
//	tcb_t* tcb = malloc(sizeof(tcb_t));
//	actualizar_valor_en_registro(tcb, 'h', 20);
//	assert_int_equal(20, tcb->a);
//	free(tcb);
//}

void test_fixture_copiar_numero_en_registro( void )
{
        test_fixture_start();
        run_test(test_actualizar_valor_en_registro_a);
        test_fixture_end();
}

void all_tests( void )
{
        test_fixture_copiar_numero_en_registro();
}
