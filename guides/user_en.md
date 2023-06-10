# User Guide for Compression Tool

## Introduction

This compression tool is a program that can compress
and decompress files using various compression algorithms.
It supports DEFLATE using both dynamic and static Huffman and LZW algorithm.

## CLI

### Installation

To use this tool, you need to have it installed on your system.  Once you have the executable file, you can run it from the command line.

### Basic Usage

The basic usage of the compression tool is as follows:

```shell
$ compresso <file> [options]
```

Where `file` is the relative path to the file you want to target.

## Options

The following options are available:

- `-c, --compress`: Compress the input file.
- `-d, --decompress`: Decompress the input file.
- `-h, --help`: Show help message and exit.
- `-a, --about`: Show information about the program and exit.
- `-l, --log <file>`: Specify the file path where the logs will be written.
- `-o, --output <file>`: Specify the output file name.

## Examples

Here are some examples of how to use the compression tool via CLI:

1. Compress a file using default algorithm (Huffman) and output to compressed.bin:

```shell
$ compresso file.txt -c -o compressed.bin
```

2. Decompress a file:

```shell
$ compresso compressed.bin -d
```

3. Compress a file using LZW algorithm and output to output.lzw:

```shell
$ compresso file.txt -c -alg lzw -o output.lzw
```

Conclusion

This compression tool is a useful command-line program that can compress and decompress files using dynamic Huffman and LZW algorithms.
By following the instructions in this user guide, you can quickly get started with using this tool to compress and decompress your files.

## UI

### Installation

You need to have gcc and gtk4 installed on your system to compile the program. Once you do, run it without any arguments
and it will open the UI.

### Basic Usage

The basic usage of the compression tool is as follows:

1. Select the file you want to compress/decompress by clicking "Add" button in "Chosen Files" section.
2. Select the mode you want to use (compress/decompress) by clicking the corresponding switch in "Controls" section.
3. Press "Begin" button to start the process.
4. Once the process is finished, you will see the logs in "Logs" section, which you can save to a file by clicking "Save to file" button. At this point result of the algorithms will be created.
5. If at any point you want to clear the logs or selected files, just press "Clear" button in appropriate section.
6. If you want to learn more about the program, click "About" button in "Controls" section.

## References

- [Huffman Coding](https://en.wikipedia.org/wiki/Huffman_coding)
- [LZW](https://en.wikipedia.org/wiki/Lempel%E2%80%93Ziv%E2%80%93Welch)
- [Gtk4 Documentation](https://docs.gtk.org/gtk4/)