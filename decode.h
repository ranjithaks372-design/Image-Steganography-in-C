#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "types.h"

#define MAX_FILE_SUFFIX 4
#define MAX_MAGIC_STRING 3

typedef struct _DecodeInfo
{
    char *stego_image_fname;
    FILE *fptr_stego_image;

    char output_fname[30];
    FILE *fptr_output;

    char extn_secret_file[MAX_FILE_SUFFIX];
    long size_secret_file;

} DecodeInfo;

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);
Status open_decode_files(DecodeInfo *decInfo);
Status do_decoding(DecodeInfo *decInfo);

Status decode_magic_string(DecodeInfo *decInfo);
Status decode_secret_file_extn(DecodeInfo *decInfo);
Status decode_secret_file_size(DecodeInfo *decInfo);
Status decode_secret_file_data(DecodeInfo *decInfo);

Status decode_data_from_image(char *data, int size, FILE *fptr_stego_image);
char decode_byte_from_lsb(char *image_buffer);

#endif