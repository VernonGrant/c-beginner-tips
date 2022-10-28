# C Beginner Tips

If any of the below information is misleading, please submit a pull request or
open an issue.

## The size of a char is guaranteed to be 1 byte

It's important to understand that the `sizeof(char)` is always going to be 1
byte, that's guaranteed. But in reality, the number of bits in a byte is
implementation defined. That's why `limites.h` defines the constant `CHAR_BIT`,
so to write truly portable code it's important to be aware that the size of a
byte is not always going to be 8 bits. Although, on almost all modern
computers, a byte is equal to 8 bits. You can think of a byte as the minimum
addressable unit in context of the memory model.

```C
// Do you see why this multiplication is pointless?
char *allocated_string = malloc((strlen(some_string) + 1) * sizeof(char));

// As you can see there's no need to use sizeof(char) here, because it's
// guaranteed to be 1.
char *allocated_string = malloc((strlen(some_string) + 1));
```

## Be aware of structure padding and alignment

**The order in which you place a structure's member's effects, it's size and
memory layout.** The essence of why this is the case, relates to the concept of
memory alignment and the compiler's mission to make memory access as efficient
as possible. The way in which the compiler chooses to make memory access more
efficient, greatly depends on the architecture that's being used.

Let's take a look at a couple examples:

```C
// I might assume that the total size is 8 bytes, but that will be false.
struct example {
    char a;  //  1 byte
    short b; //  2 bytes
    char c;  //  1 byte
    int d;   //  4 bytes
};

// Outputs: The size of structure example is: 12 bytes.
printf("The size of structure example is: %lu bytes.\n", sizeof(struct example));
```

Based on the output, you can see that this structure has an additional 4 bytes,
that should be unexpected if you're unfamiliar with memory alignment. Now let's make
a small adjustment by moving member `b` below member `c` and recheck the
output.

```C
struct example {
    char a;  //  1 byte
    char c;  //  1 byte
    short b; //  2 bytes
    int d;   //  4 bytes
};

// Outputs: The size of structure example is: 8 bytes.
printf("The size of structure example is: %lu bytes.\n", sizeof(struct example));
```

The structure's size is now 4 bytes less. As you can see, the way in which you
order a structures members can have a dramatic effect on memory usage,
especially if your dealing with thousands of instances.

**Truthfully speaking, it's not all that important for you to understand exactly
how the compiler is going to align your structure in memory, especially as a
beginner. As you can always just perform manual checks to find the most optimal
ordering for your use case.**

Knowing exactly how the compiler chooses to align a structure members can be
difficult to determine without a deep understanding of the architecture your
using. But I think the following quote from [The Lost Art of
Structure Packing](http://www.catb.org/esr/structure-packing/) can help you
build an intuition for most cases.

> Each type except char has an alignment requirement; chars can start on any
> byte address, but 2-byte shorts must start on an even address, 4-byte ints or
> floats must start on an address divisible by 4, and 8-byte longs or doubles
> must start on an address divisible by 8. The jargon for this is that basic C
> types on a vanilla ISA (Instruction Set Architectures) are self-aligned.
> Pointers, whether 32-bit (4-byte) or 64-bit (8-byte) are self-aligned too.

To better understand the above statement, let's look at the memory layout of the
above two examples, and break down the memory alignment checks.

```C
// 12 byte struct layout.
struct example {
    char a;  //  1 byte
    short b; //  2 bytes
    char c;  //  1 byte
    int d;   //  4 bytes
};

// Addresses +0     +1     +2     +3
// -------------------------------------------
// 7156      0x41   0x7f   0x64   0x00   A?d?
// 7160      0x41   0xe0   0xff   0xff   A???
// 7164      0x64   0x00   0x00   0x00   d???

struct example test = {'A', 100, 'A', 100};
// 0x41, [0x64,0x00], 0x41, [0x64,0x00,0x00,0x00]
```

- `test.a` is stored at address: `7156`.
- `test.b`, (short) is stored at address: `7158`.
    - `7157 % sizeof(short)` is non-zero, a 2-byte short must start on an even address.
- `test.c`, (char) is stored at address: `7160`.
- `test.d`, (int) is stored at address: `7164`
    - `7161 % sizeof(int)` is non-zero. A 4-byte int must start on an address divisible by 4.
    - `7162 % sizeof(int)` is non-zero. A 4-byte int must start on an address divisible by 4.
    - `7163 % sizeof(int)` is non-zero. A 4-byte int must start on an address divisible by 4.

So basically the 4-bytes that were skipped in favor of memory alignment is referred to as padding.

```C
// 8 byte struct layout.
struct example {
    char a;  //  1 byte
    char c;  //  1 byte
    short b; //  2 bytes
    int d;   //  4 bytes
};

// Addresses +0     +1     +2     +3
// --------------------------------------------
// 7156      0x41   0x41   0x64   0x00   AAd?
// 7160      0x64   0x00   0x00   0x00   d???

struct example test = {'A', 'A', 100, 100};
// 0x41, 0x41, [0x64,0x00],[0x64,0x00,0x00,0x00]
```

- `test.a` is stored at address: `7156`.
- `test.c`, (char) is stored at address: `7157`.
- `test.b`, (short) is stored at address: `7158`.
- `test.d`, (int) is stored at address: `7160`
    - `7159 % sizeof(int)` is non-zero. A 4-byte int must start on an address divisible by 4.

The above concept can also be put this way:

> Unaligned memory accesses occur when you try to read N bytes of data starting
> from an address that is not evenly divisible by N (i.e. addr % N != 0). For
> example, reading 4 bytes of data from address 0x10004 is fine, but reading 4
> bytes of data from address 0x10005 would be an unaligned memory access.
