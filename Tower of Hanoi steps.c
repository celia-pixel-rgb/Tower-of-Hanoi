#include <stdio.h>   /* printf, scanf */
#include <string.h>  /* memset – to clear arrays */

#define MAX_DISKS 10   /* maximum disks we support */
#define NUM_PEGS   3   /* always 3 pegs */

int pegs[NUM_PEGS][MAX_DISKS];

/*
 * height[p] = how many disks are currently on peg p.
 * When height[p] = 0, the peg is empty.
 */
int height[NUM_PEGS];

int n;        /* number of disks chosen by the user */
int step = 0; /* counter: how many moves have happened so far */

void display(void)
{
    int level, peg, k;

    printf("\n");

    /*
     * We go from the top level (n-1) down to the bottom level (0).
     * Think of it like printing a skyscraper from the roof downward.
     */
    for (level = n - 1; level >= 0; level--) {

        /* For each of the 3 pegs, print what is at this level */
        for (peg = 0; peg < NUM_PEGS; peg++) {

            if (level < height[peg]) {
                /*
                 * There IS a disk at this level of this peg.
                 * Retrieve the disk size stored in our array.
                 */
                int disk = pegs[peg][level];

                /*
                 * CENTERING TRICK:
                 * A disk of size 1 is narrow,  a disk of size n is wide.
                 * To center them all, smaller disks need extra padding spaces.
                 * Number of padding spaces on each side = (n - disk).
                 */

                /* Left padding spaces */
                for (k = 0; k < n - disk; k++)
                    printf(" ");

                /* Left half of the disk body (the "=" characters) */
                for (k = 0; k < disk; k++)
                    printf("=");

                /* Center of peg (visible through the disk hole) */
                printf("|");

                /* Right half of the disk body */
                for (k = 0; k < disk; k++)
                    printf("=");

                /* Right padding spaces */
                for (k = 0; k < n - disk; k++)
                    printf(" ");

            } else {
                /*
                 * This level is EMPTY on this peg.
                 * Just draw the bare peg pole: spaces + "|" + spaces.
                 */
                for (k = 0; k < n; k++)
                    printf(" ");
                printf("|");
                for (k = 0; k < n; k++)
                    printf(" ");
            }

            /* Gap between pegs so they don't look squished together */
            printf("    ");

        } /* end peg loop */

        printf("\n");   /* move to the next row */

    } /* end level loop */

    /* Draw the base/floor of the tower */
    printf("-------------------------------------\n");
    printf("       Peg 1         Peg 2       Peg 3\n");
}

/* ============================================================
   FUNCTION: move_disk
   PURPOSE:
     Actually move ONE disk from peg 'from' to peg 'to'.

   STEPS:
     1. Take the top disk off 'from'
     2. Put that disk on top of 'to'
     3. Update the heights
     4. Print what happened
     5. Redraw the towers
   ============================================================ */
void move_disk(int from, int to)
{
  
    int disk = pegs[from][--height[from]];

    /* Clear the slot we just removed the disk from */
    pegs[from][height[from]] = 0;

  
    pegs[to][height[to]] = disk;
    height[to]++;

    /* Step 3: Count the move */
    step++;

    /* Step 4: Print a human-readable description of the move */
    printf("\n--- Move %d ---\n", step);
    printf("  Move disk %d  from peg %d  to peg %d\n",
           disk,
           from + 1,   /* +1 to show peg numbers as 1, 2, 3 */
           to   + 1);

    /* Step 5: Redraw the towers so the user can see the result */
    display();
}


void hanoi(int num_disks, int from, int to, int aux)
{
    /* BASE CASE: Only one disk to move — just do it directly */
    if (num_disks == 1) {
        move_disk(from, to);
        return;   /* stop recursing, we are done with this sub-problem */
    }

    /*
     * STEP 1 — Recursive call:
     * Move (num_disks - 1) disks out of the way, to the auxiliary peg.
     * Notice: 'to' and 'aux' swap roles here.
     */
    hanoi(num_disks - 1, from, aux, to);

    /*
     * STEP 2 — Base action:
     * The largest disk in this sub-problem is now exposed.
     * Move it directly to its destination.
     */
    move_disk(from, to);

    /*
     * STEP 3 — Recursive call:
     * Move those (num_disks - 1) disks from the auxiliary peg
     * onto the destination peg (on top of the big disk).
     * Notice: 'from' and 'aux' swap roles here.
     */
    hanoi(num_disks - 1, aux, to, from);
}

int main(void)
{
    int i;

    /* -- Step 1: Get user input -- */
    printf("===========================================\n");
    printf("       TOWER OF HANOI — Steps Version      \n");
    printf("===========================================\n\n");
    printf("Enter number of disks (1 to %d): ", MAX_DISKS);
    scanf("%d", &n);

    /* Simple input validation */
    if (n < 1 || n > MAX_DISKS) {
        printf("Please enter a number between 1 and %d.\n", MAX_DISKS);
        return 1;   /* exit with error code 1 */
    }

    /* -- Step 2: Initialize pegs -- */

    /* Clear all pegs (set every slot to 0 = empty) */
    memset(pegs,   0, sizeof(pegs));
    memset(height, 0, sizeof(height));

    /*
     * Place all disks on peg 0 (the leftmost peg).
     * The LARGEST disk (size = n) goes to the BOTTOM (index 0).
     * The SMALLEST disk (size = 1) goes to the TOP (index n-1).
     *
     * Loop: start with i = n (big), count down to 1 (small).
     */
    for (i = n; i >= 1; i--) {
        pegs[0][height[0]] = i;   /* place disk of size i */
        height[0]++;               /* increment peg 0's height */
    }

    /*
     * Expected total number of moves = 2^n - 1.
     * For 3 disks: 2^3 - 1 = 7 moves.
     */
    int expected_moves = (1 << n) - 1;   /* (1 << n) means 2 to the power n */
    printf("\nWith %d disks, the puzzle requires %d moves.\n", n, expected_moves);
    printf("Peg 1 ? Peg 3  (using Peg 2 as helper)\n");

    /* -- Step 3: Show starting configuration -- */
    printf("\n=== STARTING POSITION ===");
    display();

    /* -- Step 4: Solve the puzzle -- */
    /*
     * Call hanoi() with:
     *   n       = number of disks
     *   from    = 0 (peg 1, leftmost, 0-indexed)
     *   to      = 2 (peg 3, rightmost, 0-indexed)
     *   aux     = 1 (peg 2, middle, 0-indexed)
     */
    hanoi(n, 0, 2, 1);

    /* -- Step 5: Summary -- */
    printf("\n===========================================\n");
    printf("  PUZZLE SOLVED in %d moves!  \n", step);
    printf("===========================================\n");

    return 0;   /* exit code 0 means "success" in C */
}
