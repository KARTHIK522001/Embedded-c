#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)

typedef struct _DecodeInfo
{
    /* Stego Image info */
    char *stego_image_fname;
    FILE *fptr_stego_image;
    char str[MAX_IMAGE_BUF_SIZE];
    char secret_file_size[4 * MAX_IMAGE_BUF_SIZE];
    int image_data_size;

    /* Secret File Info */
    char *secret_data_fname;
    FILE *fptr_secret_data;
    char decode_data[MAX_IMAGE_BUF_SIZE];

} DecodeInfo;

/* Decoding function prototype */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo);

/*Open Decode files*/
Status files_open(DecodeInfo *decInfo);

/*Decode Magic String*/
Status decode_magic_string(DecodeInfo *decInfo);

/*Image to data*/
Status decode_image_to_data(DecodeInfo *decInfo);

/*Size to data*/
Status decode_size_to_data(DecodeInfo *decInfo);

/*Decode secret file extn size*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/*Decode secret file extn*/
Status decode_secret_file_extn(DecodeInfo *decInfo);

/*Decode secret file size*/
Status decode_secret_file_size(DecodeInfo *decInfo);

/*Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

#endif

