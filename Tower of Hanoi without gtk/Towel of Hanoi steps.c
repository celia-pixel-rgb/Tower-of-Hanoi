#include <stdio.h>

/*
    FUNCTION: hanoi
    --------------------------
    This function solves the Tower of Hanoi problem using recursion.

    PARAMETERS:
    n     -> number of disks
    from  -> the starting rod (source)
    to    -> the destination rod
    aux   -> the helper rod (auxiliary)

    HOW IT WORKS:
    The idea is recursive:
    1. Move (n-1) disks from 'from' to 'aux'
    2. Move the largest disk from 'from' to 'to'
    3. Move (n-1) disks from 'aux' to 'to'
*/
void hanoi(int n, int from, int to, int aux)
{
    // BASE CASE:
    // If there is only one disk, move it directly
    if (n == 1)
    {
        printf("%d -> %d\n", from, to);
        return;
    }

    // STEP 1:
    // Move top (n-1) disks from 'from' to 'aux'
    // using 'to' as temporary storage
    hanoi(n - 1, from, aux, to);

    // STEP 2:
    // Move the largest disk (bottom disk) to destination
    printf("%d -> %d\n", from, to);

    // STEP 3:
    // Move the (n-1) disks from 'aux' to 'to'
    // using 'from' as temporary storage
    hanoi(n - 1, aux, to, from);
}

int main()
{
    int n;

    // Ask the user for number of disks
    printf("Enter number of disks: ");
    scanf("%d", &n);

    printf("\nSteps to solve Tower of Hanoi:\n\n");

    /*
        We start with:
        Rod 1 = source
        Rod 2 = auxiliary
        Rod 3 = destination
    */
    hanoi(n, 1, 3, 2);

    return 0;
}
