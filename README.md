# C Beginner Tips

#### The size of a char is guaranteed to be 1 byte.

It's important to understand that the `sizeof(char)` is always going to be 1
byte, that's guaranteed. But the number of bits in a byte can differ across
devices. That's why `limites.h` defines the constant `CHAR_BIT`, so to write
truly portable code it's important to be aware that the size of a byte is not
always 8 bits, although it's most likely the case on modern systems.

Knowing this, please avoid doing something like this:

```C
// Do you see why this is stupid?
char *allocated_string = malloc((strlen(some_string) + 1) * sizeof(char));

// Do this instead. As you can see theres no need to use sizeof(char) here,
// because it's guaranteed to be 1.
char *allocated_string = malloc((strlen(some_string) + 1));
```
