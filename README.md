# Intcode-C

`intc.c` and `intcode.c` are a pair of C files that can help you compile textual Intcode programs to binary (`intc.c`) and run a compiled binary of an intcode program (`intcode.c`).

Intcode is a special type of computer, introduced in [the 2019 edition of Advent of Code](https://adventofcode.com/2019). More information about Intcode can be found in the [day 2](https://adventofcode.com/2019/day/2), [day 5](https://adventofcode.com/2019/day/5) and [day 9](https://adventofcode.com/2019/day/9) problem descriptions.

## Requirements

Make sure that you've got `gcc` and `make` installed in your system.

## Compilation

To compile `intc.c`, run the following command:

```bash
make intc
```

To compile `intcode.c`, run the following command:

```bash
make intcode
```

## Usage

If you have a valid Intcode file named `input.inc` that you want to compile and run, there are two steps you will have to do.

*Step 1:* Compile the Intcode file to binary

```bash
./intc input.inc
```

The compiler will print out the name of the compiled file, which in this case would be `input.bin`.

*Step 2:* Run the binary

```bash
./intcode input.bin
```

 If the expected inputs and outputs are ASCII characters, use the `-a` flag as follows

```bash
./intcode input.bin -a
```

## License

[MIT](https://choosealicense.com/licenses/mit/)
