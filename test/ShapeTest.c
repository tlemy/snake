#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../include/Shape.h"
#include <string.h>

#define STR "HELLO\0"
#define NUM_NEW_SHAPES 4

Unit* unt;
Shape* shp;

int setup(void);
int cleanup(void);
void testNewUnit(void);
void testFreeUnit(void);
void testNewShape(void);
void testAddUnitToShape(void);
void testFreeShape(void);

int main(void) 
{
    CU_pSuite pSuite = NULL;

    if (CU_initialize_registry() != CUE_SUCCESS) 
    {
        return CU_get_error();
    }

    pSuite = CU_add_suite("ShapeTestSuite", setup, cleanup);

    if (pSuite == NULL) 
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(pSuite, "Create Unit", testNewUnit) == NULL) 
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(pSuite, "Free Unit", testFreeUnit) == NULL) 
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(pSuite, "New Shape", testNewShape) == NULL) 
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(pSuite, "Add Unit To Shape", testAddUnitToShape) == NULL) 
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(pSuite, "Free Shape", testFreeShape) == NULL) 
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

int setup(void) 
{
    return 0;
}

int cleanup(void) 
{
    return 0;
}

void testNewUnit(void) 
{
    const int x = 1;
    const int y = 2;
    const int len = strlen(STR);

    unt = newUnit(x, y, STR);

    CU_ASSERT_EQUAL(unt->x, x);
    CU_ASSERT_EQUAL(unt->y, y);
    CU_ASSERT_EQUAL(unt->len, len);
    CU_ASSERT_STRING_EQUAL(unt->str, STR);
}

void testFreeUnit(void) 
{
   CU_ASSERT_TRUE(freeUnit(unt));
}

void testNewShape(void) 
{
    unt = newUnit(1, 1, STR);
    shp = newShape(unt);

    CU_ASSERT_EQUAL(shp->idx, 0);
    CU_ASSERT_PTR_EQUAL(shp->unt, unt);
    CU_ASSERT_PTR_NULL(shp->nxt);
}

void testAddUnitToShape(void) 
{
    Shape *last = shp;
    
    for (int i = 0; i < NUM_NEW_SHAPES; i++) 
    {
        last = addUnitToShape(last, newUnit(i, i, STR));
    }

    CU_ASSERT_EQUAL(shp->idx, 0);
    CU_ASSERT_PTR_NOT_NULL(shp->nxt);
    CU_ASSERT_STRING_EQUAL(shp->unt->str, STR);

    CU_ASSERT_EQUAL(shp->nxt->idx, 1);
    CU_ASSERT_PTR_NOT_NULL(shp->nxt->nxt);
    CU_ASSERT_STRING_EQUAL(shp->nxt->unt->str, STR);

    CU_ASSERT_EQUAL(shp->nxt->nxt->idx, 2);
    CU_ASSERT_PTR_NOT_NULL(shp->nxt->nxt->nxt);
    CU_ASSERT_STRING_EQUAL(shp->nxt->nxt->unt->str, STR);

    CU_ASSERT_EQUAL(shp->nxt->nxt->nxt->idx, 3);
    CU_ASSERT_PTR_NOT_NULL(shp->nxt->nxt->nxt->nxt);
    CU_ASSERT_STRING_EQUAL(shp->nxt->nxt->nxt->unt->str, STR);

    CU_ASSERT_EQUAL(shp->nxt->nxt->nxt->nxt->idx, 4);
    CU_ASSERT_PTR_NULL(shp->nxt->nxt->nxt->nxt->nxt);
    CU_ASSERT_STRING_EQUAL(shp->nxt->nxt->nxt->nxt->unt->str, STR);

    CU_ASSERT_EQUAL(last->idx + 1, NUM_NEW_SHAPES + 1);
    CU_ASSERT_PTR_NOT_EQUAL(shp, last);
    CU_ASSERT_PTR_NULL(last->nxt);
}

void testFreeShape(void) 
{
   CU_ASSERT_EQUAL(freeShape(shp), NUM_NEW_SHAPES + 1);
}