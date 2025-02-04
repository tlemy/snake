#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../include/Apple.h"

void testNewApple();

void testSpawnApple();

void testSpawnAppleSamePosition();

void testFreeApple();

int main()
{
    CU_pSuite pSuite = NULL;

    if (CU_initialize_registry() != CUE_SUCCESS)
    {
        return CU_get_error();
    }

    pSuite = CU_add_suite("AppleTestSuite", NULL, NULL);

    if (pSuite == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(pSuite, "New Apple", testNewApple) == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(pSuite, "Spawn Apple", testSpawnApple) == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(pSuite, "Spawn Apple Same Position", testSpawnAppleSamePosition) == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(pSuite, "Free Apple", testFreeApple) == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

void testNewApple()
{
    int minX = 0;
    int minY = 0;
    int maxX = 40;
    int maxY = 20;

    Apple* apl = newApple(minX, minY, maxX, maxY);

    CU_ASSERT_TRUE(apl->minX > minX);
    CU_ASSERT_TRUE(apl->minY > minY);
    CU_ASSERT_TRUE(apl->maxX < maxX);
    CU_ASSERT_TRUE(apl->maxY < maxY);
    CU_ASSERT_EQUAL(apl->shp->x, 0);
    CU_ASSERT_EQUAL(apl->shp->y, 0);

    freeApple(apl);
}

void testSpawnApple()
{
    Apple* apl = newApple(0, 0, 10, 10);

    CU_ASSERT_EQUAL(spawnApple(apl), 0);
    CU_ASSERT_NOT_EQUAL(apl->shp->x, 0);
    CU_ASSERT_NOT_EQUAL(apl->shp->y, 0);

    freeApple(apl);
}

void testSpawnAppleSamePosition()
{
    Apple* apl = newApple(0, 0, 10, 10);

    apl->maxX = 2;
    apl->maxY = 2;

    spawnApple(apl);

    CU_ASSERT_EQUAL(spawnApple(apl), 1);

    freeApple(apl);
}

void testFreeApple(void)
{
    Apple* apl = newApple(0, 0, 10, 10);

    CU_ASSERT_EQUAL(freeApple(apl), 0);
}