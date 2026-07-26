#include <stdio.h>
#include <string.h>
#include "decode.h" 
#include "types.h"
#include "common.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    decInfo->stego_image_fname = argv[2];

    if (strstr(decInfo->stego_image_fname, ".bmp") == NULL)
    {
        printf("ERROR: Stego image should be a .bmp file\n");
        return e_failure;
    }

    if (argv[3] != NULL)
    {
        strcpy(decInfo->output_fname, argv[3]);
    }
    else
    {
        strcpy(decInfo->output_fname, "output");
    }

    return e_success;
}
Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");

    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        return e_failure;
    }

    decInfo->fptr_output = NULL;

    return e_success;
}
char decode_byte_from_lsb(char *image_buffer)
{
    char data = 0;

    for (int i = 0; i < 8; i++)
    {
        data = (data << 1) | (image_buffer[i] & 1);
    }

    return data;
}
Status decode_data_from_image(char *data, int size, FILE *fptr_stego_image)
{
    char image_buffer[8];

    for (int i = 0; i < size; i++)
    {
        if (fread(image_buffer, 1, 8, fptr_stego_image) != 8)
        {
            return e_failure;
        }

        data[i] = decode_byte_from_lsb(image_buffer);
    }

    return e_success;
}
Status decode_magic_string(DecodeInfo *decInfo)
{
    char magic[3];

    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);

    if (decode_data_from_image(magic, 2, decInfo->fptr_stego_image) == e_failure)
    {
        printf("ERROR: Magic string not found\n");
        return e_failure;
    }

    magic[2] = '\0';

    if (strcmp(magic, "#*") == 0)
    {
        return e_success;
    }

    return e_failure;
}
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    if (decode_data_from_image(decInfo->extn_secret_file,
                               4,
                               decInfo->fptr_stego_image) == e_failure)
    {
        return e_failure;
    }

    decInfo->extn_secret_file[4] = '\0';

    return e_success;
}
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    if (decode_data_from_image((char *)&decInfo->size_secret_file,
                               sizeof(long),
                               decInfo->fptr_stego_image) == e_failure)
    {
        return e_failure;
    }

    return e_success;
}
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char ch;

    /* Create output file as output.txt */
    char fname[50];

strcpy(fname, decInfo->output_fname);
strcat(fname, decInfo->extn_secret_file);

decInfo->fptr_output = fopen(fname, "wb");
    if (decInfo->fptr_output == NULL)
    {
        return e_failure;
    }

    for (long i = 0; i < decInfo->size_secret_file; i++)
    {
        if (decode_data_from_image(&ch, 1,
                                   decInfo->fptr_stego_image) == e_failure)
        {
            fclose(decInfo->fptr_output);
            return e_failure;
        }

        fwrite(&ch, 1, 1, decInfo->fptr_output);
    }

    fclose(decInfo->fptr_output);

    return e_success;
}
Status do_decoding(DecodeInfo *decInfo)
{
    if (open_decode_files(decInfo) == e_failure)
        return e_failure;

    if (decode_magic_string(decInfo) == e_failure)
        return e_failure;

    if (decode_secret_file_extn(decInfo) == e_failure)
        return e_failure;

    if (decode_secret_file_size(decInfo) == e_failure)
        return e_failure;

    if (decode_secret_file_data(decInfo) == e_failure)
        return e_failure;
    fclose(decInfo->fptr_stego_image);
    return e_success;
}