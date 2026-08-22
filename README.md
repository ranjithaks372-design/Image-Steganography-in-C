# Image Steganography Using C

## Overview

Image Steganography is a technique used to hide secret information inside an image without affecting its visible appearance.

This project implements image-based data hiding using the Least Significant Bit (LSB) technique in C.

## Features

- Encode secret data into an image
- Decode hidden data from a stego image
- Check file size compatibility
- Handle image and secret file operations

## Technologies Used

- C Programming
- Bitwise Operations
- File Handling
- Structures
- Pointers

## Project Structure

```text
test_encode.c - Main program and command-line interface
encode.c      - Encoding process
encode.h      - Encoding function declarations
decode.c      - Decoding process
decode.h      - Decoding function declarations
common.h      - Common definitions
types.h       - User-defined data types and macros
## Working Principle

1. Read the input BMP image.
2. Check whether the image has sufficient capacity for the secret data.
3. Convert the secret data into binary form.
4. Store the secret data bits inside image pixels using LSB modification.
5. Generate the stego image.
6. Decode the hidden data from the stego image.
7. Store the recovered data in the output file.

## Concepts Implemented

- Bit manipulation
- File handling
- Command-line arguments
- Structures
- Pointers
- Memory management
- BMP image processing

## How to Compile and Run

### Compile

```bash
gcc test_encode.c encode.c decode.c -o encode
./encode -e beautiful.bmp secret.txt stego_img.bmp
./encode -d stego_img.bmp
