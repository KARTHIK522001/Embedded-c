#include <stdio.h>
#include <string.h>
#include "types.h"
#include "common.h"
#include "decode.h"

/*Function Definitions*/

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if (strcmp(strstr(argv[2], ".bmp"), ".bmp") == 0)
    {
        printf("INFO : BMP validated successfully\n");
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        printf("INFO : ERROR : Not a .bmp file\n");
        return e_failure;
    }
    if (argv[3] != NULL)
    {
        decInfo->secret_data_fname = argv[3];
    }
    else
    {
        decInfo->secret_data_fname = "output.txt";
        printf("INFO : Output file not mentioned. Creating output.txt as default\n");
    }
    return e_success;
}

Status files_open(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    // Do error handling
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "INFO : ERROR : Unable to open file %s\n", decInfo->stego_image_fname);

        return e_failure;
    }

    decInfo->fptr_secret_data = fopen(decInfo->secret_data_fname, "w");
    // Do error handling
    if (decInfo->fptr_secret_data == NULL)
    {
        perror("fopen");
        fprintf(stderr, "INFO : ERROR : Unable to open file %s\n", decInfo->secret_data_fname);

        return e_failure;
    }
    return e_success;
}

Status decode_image_to_data(DecodeInfo *decInfo)
{
    for (int j = 0; j < 8; j++)
    {
        decInfo->decode_data[0] |= (decInfo->str[j] & 1) << j;
    }
    return e_success;
}

Status decode_magic_string(DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
    for (int i = 0; i < strlen(MAGIC_STRING); i++)
    {
        fread(decInfo->str, 8, 1, decInfo->fptr_stego_image);
        decInfo->decode_data[0] = 0;

        decode_image_to_data(decInfo);

        //printf("%c", decInfo->decode_data[0]); // Printing decoded char to the screen
        if (decInfo->decode_data[0] == MAGIC_STRING[i])
        {
            continue;
        }
        else
        {
            return e_failure;
        }
    }
    //printf("\n");
    return e_success;
}

Status decode_size_to_data(DecodeInfo *decInfo)
{
    fread(decInfo->secret_file_size, 32, 1, decInfo->fptr_stego_image);
    decInfo->image_data_size = 0;
    for (int i = 0; i < 32; i++)
    {
        decInfo->image_data_size |= (decInfo->secret_file_size[i] & 1) << i;
    }
    return e_success;
}

Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    decode_size_to_data(decInfo);

    printf("INFO : File extension size %d\n", decInfo->image_data_size);
    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    for (int i = 0; i < decInfo->image_data_size; i++)
    {
        fread(decInfo->str, 8, 1, decInfo->fptr_stego_image);
        decInfo->decode_data[0] = 0;

        decode_image_to_data(decInfo);

        //printf("%c", decInfo->decode_data[0]);
    }
	//printf("\n");
    printf("INFO : File pointer is %ld\n", ftell(decInfo->fptr_stego_image));
    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    decode_size_to_data(decInfo);

    printf("INFO : File size %d\n", decInfo->image_data_size);
    printf("INFO : File pointer is %ld\n", ftell(decInfo->fptr_stego_image));
    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    for (int i = 0; i < decInfo->image_data_size; i++)
    {
        fread(decInfo->str, 8, 1, decInfo->fptr_stego_image);
        decInfo->decode_data[0] = 0;

        decode_image_to_data(decInfo);

        printf("%c", decInfo->decode_data[0]);

        fprintf(decInfo->fptr_secret_data, "INFO : %c", decInfo->decode_data[0]);
    }
    printf("INFO : File pointer is %ld\n", ftell(decInfo->fptr_stego_image));
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    if (files_open(decInfo) == e_success)
    {
        printf("INFO : Files opened successfully\n");
        if (decode_magic_string(decInfo) == e_success)
        {
            printf("INFO : Magic string decoded successfully\n");
            if (decode_secret_file_extn_size(decInfo) == e_success)
            {
                printf("INFO : Secret file extension size decoded successfully\n");
                if (decode_secret_file_extn(decInfo) == e_success)
                {
                    printf("INFO : Secret file extension decoded successfully\n");
                    if (decode_secret_file_size(decInfo) == e_success)
                    {
                        printf("INFO : Secret file size decoded successfully\n");
                        if (decode_secret_file_data(decInfo) == e_success)
                        {
                            printf("INFO : Secret file data decoded successfully\n");
                        }
                        else
                        {
                            printf("INFO : ERROR : Failed to decode secret file data\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("INFO : ERROR : Failed to decode secret file size\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("INFO : ERROR : Failed to decode file extension\n");
                    return e_failure;
                }
            }
            else
            {
                printf("INFO : ERROR : Failed to decode secret file extension size\n");
                return e_failure;
            }
        }
        else
        {
            printf("INFO : ERROR : Failed to decode magic string\n");
            return e_failure;
        }
    }
    else
    {
        printf("INFO : ERROR : Failed to open files\n");
        return e_failure;
    }

    return e_success;
}


