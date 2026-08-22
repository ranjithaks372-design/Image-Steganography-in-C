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
