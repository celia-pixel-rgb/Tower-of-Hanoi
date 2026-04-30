#include <stdio.h>
#include <windows.h>

#define MAX 10  
// MAX defines the maximum number of disks the program can handle.
// We use a fixed size array instead of dynamic memory for simplicity.

int pegs[3][MAX];  
/* 
   This 2D array represents the 3 pegs:

   pegs[0] ? Peg 1 (source)
   pegs[1] ? Peg 2 (auxiliary)
   pegs[2] ? Peg 3 (destination)

   Each column stores disks in order (bottom to top).
*/

int height[3];  
/*
   height[i] stores how many disks are currently on peg i.

   Example:
   height[0] = 3 means peg 0 has 3 disks stacked.
*/

int n;       // number of disks entered by user
int step = 0; // counts how many moves have been performed

/* ============================================================
   DISPLAY FUNCTION
   PURPOSE:
   - Show the full Tower of Hanoi state in console
   - Always prints EXACTLY 3 towers (even if empty)

   IDEA:
   - We print from TOP level to BOTTOM level
   - Each disk is centered visually using "=" characters
   ============================================================ */
void display() {

    int level, peg, s, k;

    printf("\n");

    // We start from top of tower down to bottom
    for (level = n - 1; level >= 0; level--) {

        // Loop through all 3 pegs
        for (peg = 0; peg < 3; peg++) {

            // If there is a disk at this level of this peg
            if (level < height[peg]) {

                int disk = pegs[peg][level];
                // disk size (1 = smallest, n = largest)

                // LEFT SPACING:
                // smaller disks need more padding so they align centered
                for (s = 0; s < n - disk; s++)
                    printf(" ");

                // LEFT PART OF DISK
                for (k = 0; k < disk; k++)
                    printf("=");

                // PEG CENTER LINE
                printf("|");

                // RIGHT PART OF DISK
                for (k = 0; k < disk; k++)
                    printf("=");

                // RIGHT SPACING
                for (s = 0; s < n - disk; s++)
                    printf(" ");
            }
            else {
                // If no disk exists at this level
                // we only draw the peg line "|"

                for (s = 0; s < n; s++)
                    printf(" ");

                printf("|");

                for (s = 0; s < n; s++)
                    printf(" ");
            }

            // spacing between pegs
            printf("   ");
        }

        printf("\n");
    }

    // base line of the tower
    printf("-----------------------------\n");
    printf("   1           2           3\n");
}

/* ============================================================
   MOVE FUNCTION
   PURPOSE:
   - Move ONE disk from one peg to another

   LOGIC:
   - Remove top disk from source peg
   - Place it on destination peg
   - Update heights
   ============================================================ */
void move(int from, int to) {

    // Step 1: remove top disk from source peg
    int disk = pegs[from][--height[from]];

    // Step 2: place disk on destination peg
    pegs[to][height[to]++] = disk;

    // Step 3: increase move counter
    step++;

    // Step 4: show result after move
    printf("\nMove %d:\n", step);
    display();

    // Step 5: pause so user can visually follow moves
    Sleep(1000);
}

/* ============================================================
   HANOI RECURSIVE ALGORITHM
   PURPOSE:
   Solve Tower of Hanoi using recursion

   RULES:
   - Move n-1 disks to auxiliary peg
   - Move largest disk to destination
   - Move n-1 disks onto destination
   ============================================================ */
void hanoi(int n, int from, int to, int aux) {

    // BASE CASE:
    // If only one disk, move directly
    if (n == 1) {
        move(from, to);
        return;
    }

    // STEP 1: move n-1 disks to auxiliary peg
    hanoi(n - 1, from, aux, to);

    // STEP 2: move largest disk to destination
    move(from, to);

    // STEP 3: move n-1 disks from auxiliary to destination
    hanoi(n - 1, aux, to, from);
}

/* ============================================================
   MAIN FUNCTION
   ============================================================ */
int main() {

    int from, aux, to;
    int i;

    // Ask user for number of disks
    printf("Enter number of disks: ");
    scanf("%d", &n);

    // Ask peg configuration
    printf("Enter source peg (1-3): ");
    scanf("%d", &from);

    printf("Enter auxiliary peg (1-3): ");
    scanf("%d", &aux);

    printf("Enter destination peg (1-3): ");
    scanf("%d", &to);

    // Convert to 0-based indexing (important in C arrays)
    from--;
    aux--;
    to--;

    // Initialize all pegs as empty
    for (i = 0; i < 3; i++)
        height[i] = 0;

    // Place disks on source peg
    // Largest disk goes first (bottom)
    for (i = n; i >= 1; i--) {
        pegs[from][height[from]++] = i;
    }

    // Show starting configuration
    printf("\nInitial State:");
    display();

    // Solve puzzle
    hanoi(n, from, to, aux);

    // Final message
    printf("\nDone! Total moves = %d\n", step);

    return 0;
}
