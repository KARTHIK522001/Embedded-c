#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    //Atleast 4 arguments are required ( out of which 3 are mandatory (-e/-d .bmp .txt)
    if (argc <= 3)
    {
        printf("ERROR : Please pass CLA\n");
        return 1;
    }

    EncodeInfo encInfo;         // Structure variable declaration
	DecodeInfo decInfo;

    int res = check_operation_type(argv);
    
    if (res == e_encode)
    {
        printf("INFO : Selected Encoding\n");
        //Collecting return value directly by function call (which return 0 -> e_success   1 -> e_failure )
        if (read_and_validate_encode_args(argv, &encInfo) == e_success)
        {
            printf("INFO : Read and validate args are success\n");
            if (do_encoding(&encInfo) == e_success )
            {
                printf("INFO : Encoding is Success\n");
            }
            else
            {
                printf("INFO : Unable to perform Encoding\n");
                return 2;
            }
        }
        else
        {
            printf("INFO : Read and validate args are failure\n");
            return 1;
        }
    }
    else if (res == e_decode)
    {
        printf("INFO : Selected Decoding\n");
		if (read_and_validate_decode_args(argv, &decInfo) == e_success)
        {
            printf("INFO : Read and validate args are success\n");
            if (do_decoding(&decInfo) == e_success )
            {
                printf("INFO : Decoding is Success\n");
            }
            else
            {
                printf("INFO : Unable to perform Decoding\n");
                return 2;
            }
        }
        else
        {
            printf("INFO : Read and validate args are failure\n");
            return 1;
        }
    }
    else
    {
        printf("ERROR : Unsupported option\n");
        printf("For Encoding : ./a.out -e image.bmp secret.txt [stego.bmp]\n");
        printf("For Decoding : ./a.out -d stego.bmp [data.txt]\n");
    }
    return 0;
}

OperationType check_operation_type(char *argv[])
{
    if (strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}

