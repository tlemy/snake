#include "Player.h"
#include "Direction.h"
#include "Incrementation.h"

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

void testNewPlayer();

void testGetYIncPlayer();

void testGetXIncPlayer();

void testFreePlayer();

int main()
{
    CU_pSuite pSuite = NULL;

    if (CU_initialize_registry() != CUE_SUCCESS)
    {
        return CU_get_error();
    }

    pSuite = CU_add_suite("Player", NULL, NULL);

    if (pSuite == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(pSuite, "New Player", testNewPlayer) == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(pSuite, "Get Y Inc Player", testGetYIncPlayer) == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(pSuite, "Get X Inc Player", testGetXIncPlayer) == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(pSuite, "Free Player", testFreePlayer) == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

void testNewPlayer()
{
    Player* ply = newPlayer(10, 0, 0, EAST);

    CU_ASSERT_PTR_NOT_NULL(ply);

    freePlayer(ply);
}

void testGetYIncPlayer()
{
    Player* ply = newPlayer(10, 0, 0, NORTH);

    CU_ASSERT_EQUAL(getYIncPlayer(ply), Y_INC_SNAKE * -1);

    freePlayer(ply);
}

void testGetXIncPlayer()
{
    Player* ply = newPlayer(10, 0, 0, WEST);

    CU_ASSERT_EQUAL(getXIncPlayer(ply), X_INC_SNAKE * -1);

    freePlayer(ply);
}

void testFreePlayer()
{
    Player* ply = newPlayer(10, 0, 0, WEST);

    CU_ASSERT_EQUAL(freePlayer(ply), 0);
}