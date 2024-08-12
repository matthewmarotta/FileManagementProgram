#makefile for generating filemanagementprogram

CC = gcc
CFLAGS = -Wall -Werror -ansi -lm
	
main.out: main.o file_io_getcontent.o txt_encryption.o file_io_writecontent.o txt_decryption.o image_encryption.o image_decryption.o txt_compression.o
	$(CC) $(CFLAGS) -o main.out main.o file_io_getcontent.o txt_encryption.o file_io_writecontent.o txt_decryption.o image_encryption.o image_decryption.o txt_compression.o 

file_io_getcontent.o: file_io_getcontent.c filemanagementprogram.h
	$(CC) $(CFLAGS) -c -o file_io_getcontent.o file_io_getcontent.c 

txt_encryption.o: txt_encryption.c filemanagementprogram.h
	$(CC) $(CFLAGS) -c -o txt_encryption.o txt_encryption.c

file_io_writecontent.o: file_io_writecontent.c filemanagementprogram.h
	$(CC) $(CFLAGS) -c -o file_io_writecontent.o file_io_writecontent.c 

txt_decryption.o: txt_decryption.c filemanagementprogram.h
	$(CC) $(CFLAGS) -c -o txt_decryption.o txt_decryption.c 

txt_compression.o: txt_compression.c filemanagementprogram.h
	$(CC) $(CFLAGS) -c -o txt_compression.o txt_compression.c 

image_encryption.o: image_encryption.c filemanagementprogram.h
	$(CC) $(CFLAGS) -c -o image_encryption.o image_encryption.c 

image_decryption.o: image_decryption.c filemanagementprogram.h
	$(CC) $(CFLAGS) -c -o image_decryption.o image_decryption.c 
