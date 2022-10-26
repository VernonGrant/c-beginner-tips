# C Beginner Tips

If any of the below information is misleading, please submit a pull request or
open an issue.

## The size of a char is guaranteed to be 1 byte.

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

## Structures are packed based on processor word size.

A **word** is the natural unit of data used by a particular processor design.
The amount of data a CPU's internal data registers can hold and process at one
time is referred to as the word size. Modern desktop processors are either 32
or 64 bits, although most people are probably using an 64 bit architecture.

You must be aware of how **struct (structure) padding** works in C. Stucts are
padded based on the processor word size. As you can imagine this optimizes the
number of operations the CPU need to do, at the cost of memory usage.

Let's take a look at the following example:

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
```
