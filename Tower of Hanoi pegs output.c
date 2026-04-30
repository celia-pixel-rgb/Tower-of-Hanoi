#include <stdio.h>
#include <windows.h>   // Needed for Sleep()

#define MAX 10   // Maximum number of disks allowed

int tower[3][MAX];

// top[i] tells us how many disks are currently in peg i
int top[3];

// number of disks
int n;

void delay()
{
    Sleep(500);  // Pause for 0.5 seconds
}

void initialize()
{
    int i;

    // Initialize all pegs as empty
    for (i = 0; i < 3; i++)
    {
        top[i] = 0;
    }

    // Put all disks on peg 0 (which is peg 1 visually)
    // Larger number = bigger disk
    for (i = n; i >= 1; i--)
    {
        tower[0][top[0]] = i;
        top[0]++;
    }
}

void print_tower()
{
    system("cls");  // Clear the screen

    int i, j, k;

    // We print from TOP to BOTTOM
    for (i = MAX - 1; i >= 0; i--)
    {
        for (j = 0; j < 3; j++) // for each peg
        {
            if (i < top[j])
            {
                // Get disk size
                int size = tower[j][i];

                // Print spaces BEFORE disk (for alignment)
                for (k = 0; k < MAX - size; k++)
                    printf(" ");

                // Print the disk (using '=')
                for (k = 0; k < size; k++)
                    printf("=");

                // Print the center of disk
                printf("|");

                // Print other half
                for (k = 0; k < size; k++)
                    printf("=");

                // Spaces AFTER disk
                for (k = 0; k < MAX - size; k++)
                    printf(" ");
            }
            else
            {
                // If no disk ? print empty peg
                for (k = 0; k < MAX; k++)
                    printf(" ");
                printf("|");
                for (k = 0; k < MAX; k++)
                    printf(" ");
            }

            printf("   "); // space between pegs
        }
        printf("\n");
    }

    // Print base
    printf("------------------------------------------------------------\n");
    printf("      1                 2                 3\n");
}

// --------------------------------------------------
// FUNCTION: move_disk
// PURPOSE: Move one disk from one peg to another
// --------------------------------------------------
void move_disk(int from, int to)
{
    // Remove disk from source peg
    int disk = tower[from][top[from] - 1];
    top[from]--;

    // Place disk on destination peg
    tower[to][top[to]] = disk;
    top[to]++;

    // Show movement
    print_tower();
    delay();
}


void hanoi(int n, int from, int to, int aux)
{
    // BASE CASE: only 1 disk
    if (n == 1)
    {
        move_disk(from, to);
        return;
    }

    // STEP 1: Move n-1 disks to auxiliary peg
    hanoi(n - 1, from, aux, to);

    // STEP 2: Move largest disk
    move_disk(from, to);

    // STEP 3: Move n-1 disks to destination
    hanoi(n - 1, aux, to, from);
}

// --------------------------------------------------
// MAIN FUNCTION
// --------------------------------------------------
int main()
{
    printf("Enter number of disks (max %d): ", MAX);
    scanf("%d", &n);

    // Safety check
    if (n > MAX || n <= 0)
    {
        printf("Invalid number of disks.\n");
        return 0;
    }

    // Initialize towers
    initialize();

    // Show initial state
    print_tower();
    delay();

    // Solve the problem visually
    hanoi(n, 0, 2, 1);

    printf("\nFinished!\n");

    return 0;
}
