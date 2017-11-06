/*
Sonic 3D Blast Sonic Graphic Compressor
Written by TheStoneBanana on November 4th/5th, 2017
*/
#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
FILE *in, *out;

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("= Sonic 3D Blast Sonic Art Compressor =\nWritten by TheStoneBanana\n\nUsage: s3dcomp.exe [DATA TO COMPRESS].bin\nOutput: [DATA TO COMPRESS]_c.bin\n");
		exit(1);
	}

	in = fopen(argv[1], "rb");
	if (in == NULL) {
		printf("ERROR: Could not open uncompressed data!\n");
		exit(1);
	}
	
	fseek(in, 0, SEEK_END);
	long data_size = ftell(in);
	fseek(in, 0, SEEK_SET);
	unsigned char *in_buffer = malloc(data_size);
	
	fread(in_buffer, data_size, 1, in);
	fclose(in);

	unsigned char *out_buff = malloc(0x80);
	unsigned char *out_buff_art = malloc(0x400);
	memset(&out_buff_art[0], 0xFF, 0x400);
	int out_buffer_pos = 0x0;
	int out_buffer_art_pos = 0x0;
	int bufin_pos = 0x0;	
	
	while (bufin_pos < data_size) {
		int com = 0x0;	
		int data1 = (in_buffer[bufin_pos+0]<<24) + (in_buffer[bufin_pos+1]<<16) + (in_buffer[bufin_pos+2]<<8) + (in_buffer[bufin_pos+3]);
		int data2 = (in_buffer[bufin_pos+4]<<24) + (in_buffer[bufin_pos+5]<<16) + (in_buffer[bufin_pos+6]<<8) + (in_buffer[bufin_pos+7]);
		int data3 = (in_buffer[bufin_pos+8]<<24) + (in_buffer[bufin_pos+9]<<16) + (in_buffer[bufin_pos+10]<<8) + (in_buffer[bufin_pos+11]);
		
		if (data1 == data2) {
			if (data1 == 0) {
				com = 0x7A;
				// handle data1, data2, data3, and possibly onwards
				if (data3 == data2) {
					com = com - 0x2;
					bufin_pos = bufin_pos + 0xC;						
					if (bufin_pos >= data_size) {
						break;
					}
					int done = 0;
					while (done == 0) {
						// compare data 3 to data 4
						// if the same, cut off the command here
						if (((in_buffer[bufin_pos+0]<<24) + (in_buffer[bufin_pos+1]<<16) + (in_buffer[bufin_pos+2]<<8) + (in_buffer[bufin_pos+3]))!=((in_buffer[bufin_pos-4]<<24) + (in_buffer[bufin_pos-3]<<16) + (in_buffer[bufin_pos-2]<<8) + (in_buffer[bufin_pos-1]))) {
							done = 1;
						}
						// otherwise, extend the current draw
						else {
							com = com - 0x2;
							bufin_pos = bufin_pos + 0x4;
							if (bufin_pos >= data_size) {
								break;
							}
						}
						if (com <= 0x54) {
							done = 1;
						}
					}
					
				}
				// just draw data1 and data2, since the rest is not the same
				else {
					bufin_pos = bufin_pos + 0x8;
					if (bufin_pos >= data_size) {
						break;
					}					
				}			
			}
			else {
				out_buff_art[out_buffer_art_pos] = in_buffer[bufin_pos+0];
				out_buff_art[out_buffer_art_pos+1] = in_buffer[bufin_pos+1];
				out_buff_art[out_buffer_art_pos+2] = in_buffer[bufin_pos+2];
				out_buff_art[out_buffer_art_pos+3] = in_buffer[bufin_pos+3];	
				out_buffer_art_pos = out_buffer_art_pos + 0x4;
				com = 0x4E;
				// handle data1, data2, data3, and possibly onwards
				if (data3 == data2) {
					com = com - 0x2;
					bufin_pos = bufin_pos + 0xC;						
					if (bufin_pos >= data_size) {
						break;
					}
					int done = 0;
					while (done == 0) {
						// compare data 3 to data 4
						// if the same, cut off the command here
						if (((in_buffer[bufin_pos+0]<<24) + (in_buffer[bufin_pos+1]<<16) + (in_buffer[bufin_pos+2]<<8) + (in_buffer[bufin_pos+3]))!=((in_buffer[bufin_pos-4]<<24) + (in_buffer[bufin_pos-3]<<16) + (in_buffer[bufin_pos-2]<<8) + (in_buffer[bufin_pos-1]))) {
							done = 1;
						}
						// otherwise, extend the current draw
						else {
							com = com - 0x2;
							bufin_pos = bufin_pos + 0x4;
							if (bufin_pos >= data_size) {
								break;
							}
						}
						if (com <= 0x2) {
							done = 1;
						}
					}
					
				}
				// just draw data1 and data2, since the rest is not the same
				else {
					bufin_pos = bufin_pos + 0x8;
					if (bufin_pos >= data_size) {
						break;
					}					
				}
			}
		}
		else if (data1 == 0) {
			com = 0x7C;
			if (bufin_pos == data_size-4) {
				out_buff[out_buffer_pos] = com;	
				break;
			}
			bufin_pos = bufin_pos + 0x4;
			if (bufin_pos >= data_size) {
				break;
			}
		}
		else {
			com = 0xFE;
			if (bufin_pos == data_size-4) {
				out_buff[out_buffer_pos] = com;	
				break;
			}
			
			else if (data2 == 0) {
				out_buff_art[out_buffer_art_pos] = in_buffer[bufin_pos+0];
				out_buff_art[out_buffer_art_pos+1] = in_buffer[bufin_pos+1];
				out_buff_art[out_buffer_art_pos+2] = in_buffer[bufin_pos+2];
				out_buff_art[out_buffer_art_pos+3] = in_buffer[bufin_pos+3];
				bufin_pos = bufin_pos + 0x4;	
				out_buffer_art_pos = out_buffer_art_pos + 0x4;
			}
			
			else if (data2 == data3) {
				// just draw data1, since the repeating data needs to be handled later
				out_buff_art[out_buffer_art_pos] = in_buffer[bufin_pos+0];
				out_buff_art[out_buffer_art_pos+1] = in_buffer[bufin_pos+1];
				out_buff_art[out_buffer_art_pos+2] = in_buffer[bufin_pos+2];
				out_buff_art[out_buffer_art_pos+3] = in_buffer[bufin_pos+3];
				out_buffer_art_pos = out_buffer_art_pos + 0x4;
				bufin_pos = bufin_pos + 0x4;

				if (bufin_pos >= data_size) {
					break;
				}
			}
			else {
				// draw data1 and data2, but compare onward to see exactly how long
				com = com - 0x2;
				out_buff_art[out_buffer_art_pos] = in_buffer[bufin_pos+0];
				out_buff_art[out_buffer_art_pos+1] = in_buffer[bufin_pos+1];
				out_buff_art[out_buffer_art_pos+2] = in_buffer[bufin_pos+2];
				out_buff_art[out_buffer_art_pos+3] = in_buffer[bufin_pos+3];
				out_buff_art[out_buffer_art_pos+4] = in_buffer[bufin_pos+4];
				out_buff_art[out_buffer_art_pos+5] = in_buffer[bufin_pos+5];
				out_buff_art[out_buffer_art_pos+6] = in_buffer[bufin_pos+6];
				out_buff_art[out_buffer_art_pos+7] = in_buffer[bufin_pos+7];
				out_buffer_art_pos = out_buffer_art_pos + 0x8;
				bufin_pos = bufin_pos + 0x8;
				if (bufin_pos >= data_size) {
					break;
				}

				int done = 0;
				while (done == 0) {
					// compare data 3 to data 4
					// if the same, cut off the command here
					if (((in_buffer[bufin_pos+0]<<24) + (in_buffer[bufin_pos+1]<<16) + (in_buffer[bufin_pos+2]<<8) + (in_buffer[bufin_pos+3]))==((in_buffer[bufin_pos+4]<<24) + (in_buffer[bufin_pos+5]<<16) + (in_buffer[bufin_pos+6]<<8) + (in_buffer[bufin_pos+7]))) {
						done = 1;
					}
					else if (((in_buffer[bufin_pos+0]<<24) + (in_buffer[bufin_pos+1]<<16) + (in_buffer[bufin_pos+2]<<8) + (in_buffer[bufin_pos+3])) == 0) {
						done =1 ;
					}
					// otherwise, extend the current draw
					else {
						com = com - 0x2;
						out_buff_art[out_buffer_art_pos] = in_buffer[bufin_pos+0];
						out_buff_art[out_buffer_art_pos+1] = in_buffer[bufin_pos+1];
						out_buff_art[out_buffer_art_pos+2] = in_buffer[bufin_pos+2];
						out_buff_art[out_buffer_art_pos+3] = in_buffer[bufin_pos+3];
						out_buffer_art_pos = out_buffer_art_pos + 0x4;
						bufin_pos = bufin_pos + 0x4;
						if (bufin_pos >= data_size) {
							break;
						}
					}
					if (com <= 0x80) {
						done = 1;
					}
				}
				
			}
			
		}
		out_buff[out_buffer_pos] = com;
		out_buffer_pos++;
	}
	out_buff[out_buffer_pos] = 0;
	
	int len;
	// get length of the offsets
	for (len = 0; len < 0x80; len++) {
		if (out_buff[len] == 0) {
			break;
		}
	}
	
	char temp_filename[256];
	memset(&temp_filename[0], 0, sizeof(temp_filename));
	strncpy(temp_filename, argv[1], strlen(argv[1])-4);
	strcat(temp_filename, "_c");
	strcat(temp_filename, argv[1]+(strlen(argv[1])-4));
	
	realloc(out_buff_art, out_buffer_art_pos);
	realloc(out_buff, len+1);
	unsigned char *final_buffer = malloc(len+1+out_buffer_art_pos);
	memcpy(final_buffer, out_buff, len+1);
	memcpy(final_buffer+len+1, out_buff_art, out_buffer_art_pos);
	
	out = fopen(temp_filename, "wb+");
	fwrite(final_buffer, len+1+out_buffer_art_pos, 1, out);
	fclose(out);
	
	// Clean up
	free(final_buffer);
	free(out_buff);
	free(out_buff_art);
	free(in_buffer);
	
	printf("Successfully compressed!\n");
	return(0);
}