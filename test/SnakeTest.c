#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../include/Snake.h"

Snake* snk;
const int x = 0;
const int y = 1;
const int initLen = 4;

int setup(void);
int cleanup(void);
void testNewSnake(void);
void testGrowSnake(void);
void testMoveSnake(void);

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

    if (CU_add_test(pSuite, "New Snake", testNewSnake) == NULL) 
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(pSuite, "Grow Snake", testGrowSnake) == NULL) 
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(pSuite, "Move Snake", testMoveSnake) == NULL) 
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

void testNewSnake(void) 
{
    snk = newSnake(x, y, initLen);

    CU_ASSERT_EQUAL(snk->head->idx, 0);
    CU_ASSERT_EQUAL(snk->head->unt->x, x);
    CU_ASSERT_EQUAL(snk->head->unt->y, y);

    CU_ASSERT_EQUAL(snk->tail->idx, initLen - 1);
    CU_ASSERT_EQUAL(snk->tail->unt->x, x - ((initLen - 1) * 2));
    CU_ASSERT_EQUAL(snk->tail->unt->y, y);

    CU_ASSERT_EQUAL(snk->len, initLen);
}

void testGrowSnake(void) 
{
    snk = growSnake(snk);

    CU_ASSERT_EQUAL(snk->tail->unt->x, -1);
    CU_ASSERT_EQUAL(snk->tail->unt->y, -1);
    CU_ASSERT_EQUAL(snk->len, initLen + 1);
}

void testMoveSnake(void) 
{
    int diffX = 2;
    int diffY = 1;

    CU_ASSERT_EQUAL(moveSnake(snk, diffX, diffY), snk->len);
    CU_ASSERT_EQUAL(snk->head->unt->x, x + diffX);
    CU_ASSERT_EQUAL(snk->head->unt->y, y + diffY);
    CU_ASSERT_EQUAL(snk->tail->unt->x, snk->tail->prv->unt->x - diffX);
    CU_ASSERT_EQUAL(snk->tail->unt->y, snk->tail->prv->unt->y);
}