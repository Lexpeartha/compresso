# Compresso - tool for compressing files

## Usage

The app has two modes: CLI and UI. The mode is selected by defining the `USE_CLI` or `USE_UI` macro.
You can compile the app with the following commands:
```shell
// Compile the CLI version
$ gcc -D USE_CLI main.c -o cli_app

// Compile the UI version
$ gcc -D USE_UI main.c -o ui_app
```

**Note:** Everything is tested on Linux, and as such we've only used the `gcc` compiler. Things may or may not work on other platforms.

## CLI

**Note:** The CLI version is not yet implemented.

## UI

**Note:** The UI version is not yet implemented.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

## Contributing

Currently, we are not accepting any contributions. This is mandatory for the course we are taking, and as such we are not allowed to accept any contributions.
But due to the license, you are free to fork this project and do whatever you want with it :)

## Credits

- [**Aleksa Sević**](https://github.com/Lexpeartha): UI, CLI, client-facing code and overall project management
- [**Lazar Senović**](https://github.com/amoscientiam): Static and dynamic Huffman coding, Deflate algorithm
- [**Petar Rančić**](https://github.com/petar030): LZW coding, Error Handling