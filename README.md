# C Beginner Tips

If any of the below information is misleading, please submit a pull request or
open an issue.

## The size of a char is guaranteed to be 1 byte

It's important to understand that the `sizeof(char)` is always going to be 1
byte, that's guaranteed. But the number of bits in a byte is implementation
defined. That's why `limites.h` defines the constant `CHAR_BIT`, so to write
truly portable code it's important to be aware that the size of a byte is not
always going to be 8 bits, although it's most likely the case on modern
systems. Think of a byte as the minimum addressable unit in context of the
memory model.

Knowing this, please avoid doing things like this:

```C
// Do you see why this is stupid?
char *allocated_string = malloc((strlen(some_string) + 1) * sizeof(char));

// Do this instead. As you can see theres no need to use sizeof(char) here,
// because it's guaranteed to be 1.
char *allocated_string = malloc((strlen(some_string) + 1));
```

## Structures padding is related to the processors word size

Structure padding is a concept in C that adds the one or more empty bytes
between the memory addresses to align the data in memory. A processor reads
more than one byte at a time, we call the amount of data read by the processor
a **word**. On a 32 bit processor a word is equal to 4 bytes and on a 64 bit
processor a word is 8 bytes.

Generally speaking, structure members are aligned based on your processors word
size. This results in less CPU operations at the cost of extra memory usage.

```C
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
    short int a;      // Is 2 bytes. --- Can be packed into a single 8 bytes.
    short int c;      // Is 2 bytes.  /
    unsigned int d;   // Is 4 bytes. /
    long long int b;  // Is 8 bytes.
                      // Total is : 16 bytes.
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
```
