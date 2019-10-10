#include "eng_int.h"

/* If this symbol is defined then ENGINE_get_default_SM2(), the function that is
 * used by SM2 to hook in implementation code and cache defaults (etc), will
 * display brief debugging summaries to stderr with the 'nid'. */
/* #define ENGINE_SM2_DEBUG */

static ENGINE_TABLE *sm2_table = NULL;
static const int dummy_nid = 1;

void ENGINE_unregister_SM2(ENGINE *e)
	{
	engine_table_unregister(&sm2_table, e);
	}

static void engine_unregister_all_SM2(void)
	{
	engine_table_cleanup(&sm2_table);
	}

int ENGINE_register_SM2(ENGINE *e)
	{
	if(e->sm2_meth)
		return engine_table_register(&sm2_table,
				engine_unregister_all_SM2, e, &dummy_nid, 1, 0);
	return 1;
	}

void ENGINE_register_all_SM2()
	{
	ENGINE *e;

	for(e=ENGINE_get_first() ; e ; e=ENGINE_get_next(e))
		ENGINE_register_SM2(e);
	}

int ENGINE_set_default_SM2(ENGINE *e)
	{
	if(e->sm2_meth)
		return engine_table_register(&sm2_table,
				engine_unregister_all_SM2, e, &dummy_nid, 1, 1);
	return 1;
	}

/* Exposed API function to get a functional reference from the implementation
 * table (ie. try to get a functional reference from the tabled structural
 * references). */
ENGINE *ENGINE_get_default_SM2(void)
	{
	return engine_table_select(&sm2_table, dummy_nid);
	}

/* Obtains an SM2 implementation from an ENGINE functional reference */
const SM2_METHOD *ENGINE_get_SM2(const ENGINE *e)
	{
	return e->sm2_meth;
	}

/* Sets an SM2 implementation in an ENGINE structure */
int ENGINE_set_SM2(ENGINE *e, const SM2_METHOD *sm2_meth)
	{
	e->sm2_meth = sm2_meth;
	return 1;
	}

