#pragma once
#include <iostream>
const int FILE_HEADER_SIZE = 14;
const int INFO_HEADER_SIZE = 40;

unsigned char* BMP1_Header(int height, int stride);
unsigned char* BMP1_info(int height, int width); 
unsigned char* BMP8_Header(int height, int stride, int colore_size);
unsigned char* BMP8_info(int height, int width, int colore_size);
unsigned char* BMP24_Header(int height, int stride);
unsigned char* BMP24_info(int height, int width);
unsigned char* BMP32_Header(int height, int stride);
unsigned char* BMP32_info(int height, int width);

void BMP1_save(unsigned char* image, int height, int width, char* imageFileName, char** colore, int colore_size = 2);
void BMP8_save(char* image, int height, int width, char* imageFileName, char** colore , int colore_size = 256);
void BMP24_save(unsigned char* image, int height, int width, char* imageFileName);
unsigned char* BMP24_read(int &height, int &width, char* imageFileName);

void BMP32_save(unsigned char* image, int height, int width, char* imageFileName);

