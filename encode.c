#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    //printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
   // printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }
    return e_success;
}
OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1], "-e")==0)
    {
        return e_encode;
    }
    else if(strcmp(argv[1],"-d")==0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;

    }
}
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    encInfo->src_image_fname=argv[2];
    if(strstr(encInfo->src_image_fname,".bmp")==NULL)
    {
        printf("ERRROR:Source image should be .bmp file\n");
        return e_failure;
    }
    encInfo->secret_fname=argv[3];
    char *ptr = strstr(encInfo->secret_fname, ".");

if(ptr == NULL)
{
    printf("No extension found\n");
    return e_failure;
}

strcpy(encInfo->extn_secret_file, ptr);
    if(argv[4]!=NULL)
    {
        encInfo->stego_image_fname=argv[4];
    }
    else
    {
        encInfo->stego_image_fname="stego.bmp";
    }
    return e_success;
}
//printf("enter do\n");
Status do_encoding(EncodeInfo *encInfo)
{

    if (open_files(encInfo) == e_failure)
    {
        //printf("Cant open files\n");
        return e_failure;
    }
    if (check_capacity(encInfo) == e_failure)
    {
        //printf("Cant open capacity\n");
        return e_failure;
    }
    if (copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_failure)
    {
      //printf("Cant open bmp\n");
        return e_failure;
    }
    if (encode_magic_string("#*", encInfo) == e_failure)
    {
        //printf("Cant open magic\n");
        return e_failure;
    }
    if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_failure)
    {
        return e_failure;
    }
    if (encode_secret_file_size(encInfo->size_secret_file,encInfo) == e_failure)
    {
        return e_failure;
    }
    if (encode_secret_file_data(encInfo) == e_failure)
    {
        return e_failure;
    }
    if (copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_failure)
    {
        return e_failure;
    }
    fclose(encInfo->fptr_src_image);
    fclose(encInfo->fptr_secret);
    fclose(encInfo->fptr_stego_image);
    return e_success;
}
Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    rewind(encInfo->fptr_src_image);
    int required_size;

    required_size = 54 +(strlen("#*") * 8) +
                    32 +
                    (strlen(encInfo->extn_secret_file) * 8) +
                    32 +
                    (encInfo->size_secret_file * 8);

    if (encInfo->image_capacity >= required_size)
    {
        return e_success;
    }

    return e_failure;
}
uint get_file_size(FILE *fptr)
{
   uint  size;
    fseek(fptr,0,SEEK_END);
    size=ftell(fptr);
    rewind(fptr);
    return size;
}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char header[54];

    if (fread(header, 1, 54, fptr_src_image) != 54)
        return e_failure;

    if (fwrite(header, 1, 54, fptr_dest_image) != 54)
        return e_failure;

    return e_success;
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    if (encode_data_to_image(magic_string, strlen(magic_string),encInfo->fptr_src_image,
    encInfo->fptr_stego_image) == e_failure)
    {
        return e_failure;
    }
    return e_success;
}
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    if (encode_data_to_image(file_extn,strlen(file_extn),encInfo->fptr_src_image,
                 encInfo->fptr_stego_image) == e_failure)
    {
        return e_failure;
    }
    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    return encode_data_to_image((char *)&file_size,sizeof(long),encInfo->fptr_src_image,
                                encInfo->fptr_stego_image);
}
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    while (fread(encInfo->secret_data, 1, 1, encInfo->fptr_secret) > 0)
    {
        if (encode_data_to_image(encInfo->secret_data,1,encInfo->fptr_src_image,
            encInfo->fptr_stego_image) == e_failure)
        {
            return e_failure;
        }
    }
    return e_success;
}
Status encode_data_to_image(const char *data, int size,
                            FILE *fptr_src_image,
                            FILE *fptr_stego_image)
{
    char image_buffer[8];

    for (int i = 0; i < size; i++)
    {
        if (fread(image_buffer, 1, 8, fptr_src_image) != 8)
            return e_failure;

        encode_byte_to_lsb(data[i], image_buffer);

        if (fwrite(image_buffer, 1, 8, fptr_stego_image) != 8)
            return e_failure;
    }

    return e_success;
}
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for (int i = 0; i < 8; i++)
    {
        image_buffer[i] = (image_buffer[i] & 0xFE) | ((data >> (7 - i)) & 1);
    }
    return e_success;
}
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while (fread(&ch, 1, 1, fptr_src) == 1)
    {
        if (fwrite(&ch, 1, 1, fptr_dest) != 1)
        {
            perror("fwrite");
            printf("Write failed at position %ld\n", ftell(fptr_dest));
            return e_failure;
        }
    }
    return e_success;
}