# User Guide for Compression Tool

## Introduction

This compression tool is a program that can compress
and decompress files using various compression algorithms.
It supports DEFLATE using both dynamic and static Huffman and LZW algorithm.

## CLI

### Installation

To use this tool, you need to have it installed on your system. To install it, download the executable file from the official website or compile it from source code. Once you have the executable file, you can run it from the command line.

### Basic Usage

The basic usage of the compression tool is as follows:

```shell
$ compresso <file> [options]
```

Where `file` is the relative path to the file you want to target.

**Note:** you can also use wildcard characters (`*` and `?`) to target multiple files at once.

## Options

The following options are available:

- `-c, --compress`: Compress the input file.
- `-d, --decompress`: Decompress the input file.
- `-h, --help`: Show help message and exit.
- `-a, --about`: Show information about the program and exit.
- `-alg, --algorithm <algorithm>`: Specify the compression algorithm to use. Valid options are `huffman` and `lzw`.
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