# Image Converter: Efficient Parallel C++ Image Processing Tool
## Introduction
Welcome to the Black-White Image Converter, a sophisticated and efficient C++ tool for image processing. This tool stands out for its versatility in handling various image formats and its capability to process images into black and white using a highly optimized, multi-threaded approach. Ideal for both developers and enthusiasts in image processing, this tool leverages modern C++ features and libraries to deliver a top-notch performance.

## Features

- **Multiple Image Formats**: Supports PNG, JPEG, BMP, and TGA formats for both input and output.
- **Efficient Black and White Conversion**: Converts color images to black and white using a multi-threaded approach, optimizing performance on multi-core processors.
- **Modular Design**: Utilizes a strategy pattern for saving images, allowing for easy extension to support additional image formats.
- **Error Handling**: Robust error handling for file reading and writing, ensuring reliability.
- **CLI Integration**: Command-line interface for easy usage and integration into various workflows.

## Dependencies

- STB
: A set of single-file libraries for C/C++ for loading images, etc.
- CLI11
: A command-line parser for C++.

## Installation
Follow these steps to install and compile the STB CLI Black &amp; White Image Converter:

### Prerequisites

- CMake (minimum version 3.15)
- A C++ compiler that supports C++17 standard
- Git (for fetching dependencies)

### Steps

- **Clone the Repository**

First, clone this repository to your local machine:

```bash
git clone https://github.com/hun756/stb_cli_bw_converter.git
cd stb_cli_bw_converter
```

- **Fetch Dependencies**

The project uses CMake's `FetchContent` module to automatically download and set up dependencies (`CLI11` and `stb` libraries). The CMake script handles this, so no manual downloading is required.
- **Configure with CMake**

Create a build directory and run CMake to configure the project:

```bash
mkdir build
cd build
cmake ..
```

- **Compile the Project**

Compile the project using CMake:

```bash
cmake --build .
```

This step will create the `stb_cli_bw_converter` executable within the build directory.
- **Running the Executable**

After successful compilation, you can run the executable directly from the build directory:


```bash
./stb_cli_bw_converter -i <input_image_path> -o <output_image_path>
```

Replace `<input_image_path>` and `<output_image_path>` with your specific file paths.

### Additional Information

- The project is set to use `-Wall -Wextra -pedantic` compile options for rigorous error checking and `-Oz` for optimized binary size.
- Ensure your C++ compiler supports C++17 standard to successfully compile this project.


## Usage
To use the tool, run the compiled executable with the required parameters.

```bash
./stb_cli_bw_converter -i <input_image_path> -o <output_image_path>
```

- `-i, --input`: Specify the input image file path.
- `-o, --output`: Specify the output image file path.

## How It Works
The tool loads an image using the STB library, processes it into black and white using a custom `BlackAndWhiteProcessor`, and saves it in the desired format. The saving strategy is determined based on the file extension, offering flexibility and ease of extension.

## Extending the Tool
To add support for additional image formats, simply extend the `SaveStrategy` class and integrate your new class into the `ImageConverter`.

## Contribution
Contributions to enhance the tool or add more features are always welcome. Please adhere to standard coding conventions and add unit tests where applicable.

## License
This project is licensed under <a target="_new">MIT License</a>.
