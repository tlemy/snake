#include "Snake.h"
#include "Direction.h"

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

void testNewSnake();

void testGrowSnake();

void testMoveSnake();

void testFreeSnake();

int main()
{
    CU_pSuite pSuite = NULL;

    if (CU_initialize_registry() != CUE_SUCCESS)
    {
        return CU_get_error();
    }

    pSuite = CU_add_suite("Snake", NULL, NULL);

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

    if (CU_add_test(pSuite, "Free Snake", testFreeSnake) == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

void testNewSnake()
{
    int x = 1;
    int y = 8;
    int initLen = 20;

    Snake* snk = newSnake(x, y, initLen, EAST);

    CU_ASSERT_EQUAL(snk->head->idx, 0);
    CU_ASSERT_EQUAL(snk->head->x, x);
    CU_ASSERT_EQUAL(snk->head->y, y);

    CU_ASSERT_EQUAL(snk->tail->idx, initLen - 1);
    CU_ASSERT_EQUAL(snk->tail->x, x - ((initLen - 1) * 2));
    CU_ASSERT_EQUAL(snk->tail->y, y);

    CU_ASSERT_EQUAL(snk->len, initLen);

    freeSnake(snk);
}

void testGrowSnake()
{
    int initLen = 20;

    Snake* snk = growSnake(newSnake(0, 0, initLen, EAST));

    CU_ASSERT_EQUAL(snk->tail->x, -1);
    CU_ASSERT_EQUAL(snk->tail->y, -1);
    CU_ASSERT_EQUAL(snk->len, initLen + 1);
}

void testMoveSnake()
{
    int diffX = 2;
    int diffY = 1;
    int x = 1;
    int y = 8;
    int initLen = 20;

    Snake* snk = newSnake(x, y, initLen, EAST);

    CU_ASSERT_EQUAL(moveSnake(snk, diffX, diffY), snk->len);
    CU_ASSERT_EQUAL(snk->head->x, x + diffX);
    CU_ASSERT_EQUAL(snk->head->y, y + diffY);
    CU_ASSERT_EQUAL(snk->tail->x, snk->tail->prv->x - diffX);
    CU_ASSERT_EQUAL(snk->tail->y, snk->tail->prv->y);

    freeSnake(snk);
}

void testFreeSnake()
{
    int x = 1;
    int y = 8;
    int initLen = 20;

    Snake* snk = newSnake(x, y, initLen, EAST);

    CU_ASSERT_EQUAL(freeSnake(snk), 0);
}