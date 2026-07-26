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

main.c       - Program execution and user interaction  
encode.c     - Encoding process  
decode.c     - Decoding process  
types.h      - User-defined data types and macros  

## Working Principle

1. Read the input image.
2. Convert secret data into binary format.
3. Store data bits inside image pixels using LSB modification.
4. Decode the hidden data from the modified image.

## Concepts Implemented
- Bit manipulation
- File handling
- Command line arguments
- Structures
- Memory management

## How to Compile and Run

Compile: 
gcc *.c -o steganography


Run:

./steganography -e input.bmp secret.txt output.bmp


Decode:

./steganography -d output.bmp


## Sample Output
(Add screenshots here)

## Future Improvements
- Support more image formats
- Add encryption before hiding data
- Improve security features
