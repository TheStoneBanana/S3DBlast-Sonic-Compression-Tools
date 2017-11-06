/*
Sonic 3D Blast Sonic Graphic Decompressor
Written by TheStoneBanana on November 4th/5th, 2017
*/
#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
FILE *in, *out;

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("= Sonic 3D Blast Sonic Art Decompressor =\nWritten by TheStoneBanana\n\nUsage: s3ddecomp.exe [DATA TO DECOMPRESS].bin\nOutput: [DATA TO DECOMPRESS]_u.bin\n");
		exit(1);
	}

	in = fopen(argv[1], "rb");
	if (in == NULL) {
		printf("ERROR: Could not open compressed data!\n");
		exit(1);
	}
	
	// Loading the compressed data
	fseek(in, 0, SEEK_END);
	long data_size = ftell(in);
	fseek(in, 0, SEEK_SET);	
	unsigned char *in_buffer = malloc(data_size);
	fread(in_buffer, data_size, 1, in);
	fclose(in);

	// Buffer for the decompressed art (shouldn't exceed 32 tiles, but lemme know if there is a case that it does)
	unsigned char *out_buff = malloc(0x400);
	int out_buffer_pos = 0x0;
	int in_buffer_pos = 0x0;
	int table_len = 0x0;
	// Looking for the end of the dump routine offsets, which ends with 0x0
	for (int x = 0; x < data_size; x++) {
		if (in_buffer[x] == 0x0) {
			table_len = x+1;
			break;
		}
	}

	if (table_len == 0x0) {
		printf("ERROR: File does not seem to be compressed data!\n");
		exit(1);		
	}
		
	for (int y = 0; y < table_len; y++) {
		int dumprout_off = in_buffer[y];
		
		// [move.l  (a0),(a2)+] FUNCTION
		// Filling with defined pixels
		if (dumprout_off >= 0x2 && dumprout_off <= 0x52) {
			int draw_length = ((0x52-dumprout_off)/2);
			for (int z = 0; z < draw_length; z++) {
				out_buff[out_buffer_pos] = in_buffer[table_len + in_buffer_pos];
				out_buff[out_buffer_pos+1] = in_buffer[table_len + in_buffer_pos+1];
				out_buff[out_buffer_pos+2] = in_buffer[table_len + in_buffer_pos+2];
				out_buff[out_buffer_pos+3] = in_buffer[table_len + in_buffer_pos+3];
				out_buffer_pos = out_buffer_pos + 4;
			}
			in_buffer_pos = in_buffer_pos + 4;
		}
		
		// [move.l  d1,(a2)+] FUNCTION
		// Filling with transparent pixels
		else if (dumprout_off >= 0x54 && dumprout_off <= 0x7E) {	
			int draw_length = ((0x7E - dumprout_off)/2);
			for (int z = 0; z < draw_length; z++) {
				out_buff[out_buffer_pos] = 0x0;
				out_buff[out_buffer_pos+1] = 0x0;
				out_buff[out_buffer_pos+2] = 0x0;
				out_buff[out_buffer_pos+3] = 0x0;
				out_buffer_pos = out_buffer_pos + 4;
			}				
		}
		
		// [move.l  (a0)+,(a2)+] FUNCTION
		// Drawing
		else if (dumprout_off >= 0x80 && dumprout_off <= 0x100) {
			int draw_length = ((0x100-dumprout_off)/2);
			for (int z = 0; z < draw_length; z++) {
				out_buff[out_buffer_pos] = in_buffer[table_len + in_buffer_pos];
				out_buff[out_buffer_pos+1] = in_buffer[table_len + in_buffer_pos+1];
				out_buff[out_buffer_pos+2] = in_buffer[table_len + in_buffer_pos+2];
				out_buff[out_buffer_pos+3] = in_buffer[table_len + in_buffer_pos+3];		
				out_buffer_pos = out_buffer_pos + 4;
				in_buffer_pos = in_buffer_pos + 4;
			}
		}
		
		// End of decompression
		else if (dumprout_off == 0x0) {
			break;
		}
	}
	
	char temp_filename[256];
	memset(&temp_filename[0], 0, sizeof(temp_filename));
	strncpy(temp_filename, argv[1], strlen(argv[1])-4);
	strcat(temp_filename, "_u");
	strcat(temp_filename, argv[1]+(strlen(argv[1])-4));
	realloc(out_buff, out_buffer_pos);
	out = fopen(temp_filename, "wb+");
	fwrite(out_buff, out_buffer_pos, 1, out);
	fclose(out);
	
	// Clean up
	free(out_buff);
	free(in_buffer);
	printf("Successfully decompressed!\n");
	return(0);
}