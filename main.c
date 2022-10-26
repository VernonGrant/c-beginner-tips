#include <stdio.h>

// We're on a 64 bit processor, 64/8 = 8 bytes alignment.

struct example_a {
    short int a;      // Is 2 bytes. // Will take 8 bytes.
    long long int b;  // Is 8 bytes. // Will take 8 bytes.
    short int c;      // Is 2 bytes. -- Will take 8 bytes
    unsigned int d;   // Is 4 bytes. /
                      // Total should be: 16 bytes but it's actually 24 bytes.
};

// Same structure but improved alignment.

struct example_b {
    short int a;      // Is 2 bytes. --- Can be aligned in a single 8 bytes.
    short int c;      // Is 2 bytes.  /
    unsigned int d;   // Is 4 bytes. /
    long long int b;  // Is 8 bytes.
};

int main(int argc, char *argv[])
{
    // Print out the sizes of the structures member types.
    printf("The size of short int is: %lu\n", sizeof(short int));
    printf("The size of unsigned int is: %lu\n", sizeof(unsigned int));
    printf("The size of long long int is: %lu\n", sizeof(long long int));

    // Print the size of example structure a, 24 bytes.
    printf("The size of structure example a is: %lu\n", sizeof(struct example_a));

    // Print the size of example structure b, 16 bytes.
    printf("The size of structure example b is: %lu\n", sizeof(struct example_b));

    // Output
    // The size of short int is: 2
    // The size of unsigned int is: 4
    // The size of long long int is: 8
    // The size of structure example a is: 24
    // The size of structure example b is: 16
}
