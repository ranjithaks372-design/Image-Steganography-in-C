#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;

    if (argc < 3)
    {
        printf("Usage:\n");
        printf("./encode -e <src.bmp> <secret.txt> [stego.bmp]\n");
        printf("./encode -d <stego.bmp>\n");
        return 1;
    }

    if (check_operation_type(argv) == e_encode)
    {
        if (read_and_validate_encode_args(argv, &encInfo) == e_success)
        {
            if (do_encoding(&encInfo) == e_success)
                printf("Encoding Successful\n");
            else
                printf("Encoding Failed\n");
        }
        else
        {
            printf("Argument Validation Failed\n");
        }
    }
    else if (check_operation_type(argv) == e_decode)
    {
        if (read_and_validate_decode_args(argv, &decInfo) == e_success)
        {
            if (do_decoding(&decInfo) == e_success)
                printf("Decoding Successful\n");
            else
                printf("Decoding Failed\n");
        }
        else
        {
            printf("Argument Validation Failed\n");
        }
    }
    else
    {
        printf("Unsupported Operation\n");
    }

    return 0;
} 