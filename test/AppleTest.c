#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../include/Apple.h"

Apple* apl;

int setup(void);
int cleanup(void);
void testNewApple(void);
void testSpawnApple(void);
void testFreeApple(void);

int main(void) 
{
    CU_pSuite pSuite = NULL;

    if (CU_initialize_registry() != CUE_SUCCESS) 
    {
        return CU_get_error();
    }

    pSuite = CU_add_suite("AppleTestSuite", setup, cleanup);

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

    if (CU_add_test(pSuite, "Free Apple", testFreeApple) == NULL) 
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

void testNewApple(void) 
{
    int minX = 0;
    int minY = 0;
    int maxX = 2;
    int maxY = 2;

    apl = newApple(minX, minY, maxX, maxY);
    
    CU_ASSERT_EQUAL(apl->minX, minX);
    CU_ASSERT_EQUAL(apl->minY, minY);
    CU_ASSERT_EQUAL(apl->maxX, maxX);
    CU_ASSERT_EQUAL(apl->maxY, maxY);
    CU_ASSERT_EQUAL(apl->shp->unt->x, 0);
    CU_ASSERT_EQUAL(apl->shp->unt->y, 0);
}

void testSpawnApple(void) 
{
    spawnApple(apl);
    
    CU_ASSERT_NOT_EQUAL(apl->shp->unt->x, 0);
    CU_ASSERT_NOT_EQUAL(apl->shp->unt->y, 0);
}

void testFreeApple(void) 
{
    CU_ASSERT_EQUAL(freeApple(apl), 0);
}