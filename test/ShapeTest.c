#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../include/Shape.h"
#include <string.h>

#define STR "HELLO\0"
#define NUM_NEW_SHAPES 4

void testNewShape();

void testGrowShape();

void testFreeShape();

void testIsCollidingWithPoint();

int main()
{
    CU_pSuite pSuite = NULL;

    if (CU_initialize_registry() != CUE_SUCCESS)
    {
        return CU_get_error();
    }

    pSuite = CU_add_suite("Shape", NULL, NULL);

    if (pSuite == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(pSuite, "New Shape", testNewShape) == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(pSuite, "Grow Shape", testGrowShape) == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(pSuite, "Free Shape", testFreeShape) == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(pSuite, "Free Shape", testIsCollidingWithPoint) == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

void testNewShape()
{
    Shape* shp = newShape(1, 2, STR);

    CU_ASSERT_EQUAL(shp->idx, 0);
    CU_ASSERT_EQUAL(shp->x, 1);
    CU_ASSERT_EQUAL(shp->y, 2);
    CU_ASSERT_STRING_EQUAL(shp->str, STR);
    CU_ASSERT_PTR_NULL(shp->nxt);

    freeShape(shp);
}

void testGrowShape()
{
    Shape* shp = newShape(1, 2, STR);
    Shape* last = shp;

    for (int i = 0; i < NUM_NEW_SHAPES; i++)
    {
        last = growShape(last, newShape(i, i, STR));
    }

    CU_ASSERT_EQUAL(shp->idx, 0);
    CU_ASSERT_PTR_NOT_NULL(shp->nxt);
    CU_ASSERT_PTR_NULL(shp->prv);
    CU_ASSERT_STRING_EQUAL(shp->str, STR);

    CU_ASSERT_EQUAL(shp->nxt->idx, 1);
    CU_ASSERT_PTR_NOT_NULL(shp->nxt->prv);
    CU_ASSERT_PTR_NOT_NULL(shp->nxt->nxt);
    CU_ASSERT_STRING_EQUAL(shp->nxt->str, STR);

    CU_ASSERT_EQUAL(shp->nxt->nxt->idx, 2);
    CU_ASSERT_PTR_NOT_NULL(shp->nxt->nxt->prv);
    CU_ASSERT_PTR_NOT_NULL(shp->nxt->nxt->nxt);
    CU_ASSERT_STRING_EQUAL(shp->nxt->nxt->str, STR);

    CU_ASSERT_EQUAL(shp->nxt->nxt->nxt->idx, 3);
    CU_ASSERT_PTR_NOT_NULL(shp->nxt->nxt->nxt->prv);
    CU_ASSERT_PTR_NOT_NULL(shp->nxt->nxt->nxt->nxt);
    CU_ASSERT_STRING_EQUAL(shp->nxt->nxt->nxt->str, STR);

    CU_ASSERT_EQUAL(shp->nxt->nxt->nxt->nxt->idx, 4);
    CU_ASSERT_PTR_NOT_NULL(shp->nxt->nxt->nxt->nxt->prv);
    CU_ASSERT_PTR_NULL(shp->nxt->nxt->nxt->nxt->nxt);
    CU_ASSERT_STRING_EQUAL(shp->nxt->nxt->nxt->nxt->str, STR);

    CU_ASSERT_EQUAL(last->idx + 1, NUM_NEW_SHAPES + 1);
    CU_ASSERT_PTR_NOT_EQUAL(shp, last);
    CU_ASSERT_PTR_NULL(last->nxt);

    freeShape(last);
}

void testFreeShape()
{
    Shape* shp1 = newShape(2, 3, STR);
    Shape* shp2 = newShape(4, 5, STR);
    Shape* final = growShape(shp1, shp2);

    CU_ASSERT_EQUAL(freeShape(shp1), 2);
    CU_ASSERT_PTR_EQUAL(shp2, final);
}

void testIsCollidingWithPoint()
{
    Shape* shp1 = newShape(2, 3, STR);
    Shape* shp2 = newShape(4, 5, STR);
    Shape* final = growShape(shp1, shp2);

    CU_ASSERT_TRUE(isCollidingWithPoint(*shp1, 4, 5, 2));
}