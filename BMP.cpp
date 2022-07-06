#include "BMP.h"
#include <fstream>

void BMP1_save(unsigned char* image, int height, int width, char* imageFileName, char** colore, int colore_size)
{
    //unsigned char padding[3] = { 0, 0, 0 };
    //int paddingSize = (4 - width % 4) % 4;

    //int stride = width + paddingSize;

    //FILE* imageFile = fopen(imageFileName, "wb");

    //unsigned char* fileHeader = BMP1_Header(height, stride);
    //fwrite(fileHeader, 1, FILE_HEADER_SIZE, imageFile);

    //unsigned char* infoHeader = BMP1_info(height, width);
    //fwrite(infoHeader, 1, INFO_HEADER_SIZE, imageFile);

    //fwrite(colore[0], 1, sizeof(char) * 4, imageFile);
    //fwrite(colore[1], 1, sizeof(char) * 4, imageFile);

    //for (int i = 0; i < height * width/8; i++)
    //    fwrite(&image[i], 1, sizeof(char), imageFile);

    //fclose(imageFile);
}
unsigned char* BMP1_Header(int height, int stride)
{
    int fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + (stride * height);

    static unsigned char fileHeader[] = {
        0,0,     /// signature
        0,0,0,0, /// image file size in bytes
        0,0,0,0, /// reserved
        0,0,0,0, /// start of pixel array
    };

    fileHeader[0] = (unsigned char)('B');
    fileHeader[1] = (unsigned char)('M');
    fileHeader[2] = (unsigned char)(fileSize);
    fileHeader[3] = (unsigned char)(fileSize >> 8);
    fileHeader[4] = (unsigned char)(fileSize >> 16);
    fileHeader[5] = (unsigned char)(fileSize >> 24);
    fileHeader[10] = (unsigned char)(FILE_HEADER_SIZE + INFO_HEADER_SIZE + 4);
    fileHeader[10] = (unsigned char)((FILE_HEADER_SIZE + INFO_HEADER_SIZE + 4) >> 8);

    return fileHeader;
}
unsigned char* BMP1_info(int height, int width)
{
    static unsigned char infoHeader[] = {
        0,0,0,0, /// header size
        0,0,0,0, /// image width
        0,0,0,0, /// image height
        0,0,     /// number of color planes
        0,0,     /// bits per pixel
        0,0,0,0, /// compression
        0,0,0,0, /// image size
        0,0,0,0, /// horizontal resolution
        0,0,0,0, /// vertical resolution
        0,0,0,0, /// colors in color table
        0,0,0,0, /// important color count
    };

    infoHeader[0] = (unsigned char)(INFO_HEADER_SIZE);
    infoHeader[4] = (unsigned char)(width);
    infoHeader[5] = (unsigned char)(width >> 8);
    infoHeader[6] = (unsigned char)(width >> 16);
    infoHeader[7] = (unsigned char)(width >> 24);
    infoHeader[8] = (unsigned char)(height);
    infoHeader[9] = (unsigned char)(height >> 8);
    infoHeader[10] = (unsigned char)(height >> 16);
    infoHeader[11] = (unsigned char)(height >> 24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(1);
    infoHeader[32] = (unsigned char)(2);

    return infoHeader;
}
void BMP8_save(char* image, int height, int width, char* imageFileName, char** colore, int colore_size)
{
    /*int widthInBytes = width * 8;

    unsigned char padding[3] = { 0, 0, 0 };
    int paddingSize = (4 - (widthInBytes) % 4) % 4;

    int stride = (widthInBytes)+paddingSize;

    FILE* imageFile = fopen(imageFileName, "wb");

    unsigned char* fileHeader = BMP8_Header(height, stride, colore_size);
    fwrite(fileHeader, 1, FILE_HEADER_SIZE, imageFile);

    unsigned char* infoHeader = BMP8_info(height, width, colore_size);
    fwrite(infoHeader, 1, INFO_HEADER_SIZE, imageFile);

    for (int i = 0; i < colore_size; i++)
        fwrite(colore[i], 1, sizeof(char) * 4, imageFile);

    fwrite(image, height * width, sizeof(char), imageFile);

    fclose(imageFile);*/
}
unsigned char* BMP8_Header(int height, int stride, int colore_size)
{
    int fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + (stride * height);

    static unsigned char fileHeader[] = {
        0,0,     /// signature
        0,0,0,0, /// image file size in bytes
        0,0,0,0, /// reserved
        0,0,0,0, /// start of pixel array
    };

    fileHeader[0] = (unsigned char)('B');
    fileHeader[1] = (unsigned char)('M');
    fileHeader[2] = (unsigned char)(fileSize);
    fileHeader[3] = (unsigned char)(fileSize >> 8);
    fileHeader[4] = (unsigned char)(fileSize >> 16);
    fileHeader[5] = (unsigned char)(fileSize >> 24);
    fileHeader[10] = (unsigned char)(FILE_HEADER_SIZE + INFO_HEADER_SIZE + 4 * colore_size);
    fileHeader[10] = (unsigned char)((FILE_HEADER_SIZE + INFO_HEADER_SIZE + 4 * colore_size) >> 8);
    fileHeader[10] = (unsigned char)((FILE_HEADER_SIZE + INFO_HEADER_SIZE + 4 * colore_size) >> 16);
    fileHeader[10] = (unsigned char)((FILE_HEADER_SIZE + INFO_HEADER_SIZE + 4 * colore_size) >> 24);

    return fileHeader;
}
unsigned char* BMP8_info(int height, int width, int colore_size)
{
    static unsigned char infoHeader[] = {
        0,0,0,0, /// header size
        0,0,0,0, /// image width
        0,0,0,0, /// image height
        0,0,     /// number of color planes
        0,0,     /// bits per pixel
        0,0,0,0, /// compression
        0,0,0,0, /// image size
        0,0,0,0, /// horizontal resolution
        0,0,0,0, /// vertical resolution
        0,0,0,0, /// colors in color table
        0,0,0,0, /// important color count
    };

    infoHeader[0] = (unsigned char)(INFO_HEADER_SIZE);
    infoHeader[4] = (unsigned char)(width);
    infoHeader[5] = (unsigned char)(width >> 8);
    infoHeader[6] = (unsigned char)(width >> 16);
    infoHeader[7] = (unsigned char)(width >> 24);
    infoHeader[8] = (unsigned char)(height);
    infoHeader[9] = (unsigned char)(height >> 8);
    infoHeader[10] = (unsigned char)(height >> 16);
    infoHeader[11] = (unsigned char)(height >> 24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(8);
    infoHeader[32] = (unsigned char)(colore_size);
    infoHeader[32] = (unsigned char)(colore_size >> 8);
    infoHeader[32] = (unsigned char)(colore_size >> 16);

    return infoHeader;
}
void BMP24_save(unsigned char* image, int height, int width, char* imageFileName)
{
    int widthInBytes = width * 3;

    unsigned char padding[3] = { 0, 0, 0 };
    int paddingSize = (4 - (widthInBytes) % 4) % 4;

    int stride = (widthInBytes)+paddingSize;

    std::ofstream fout(imageFileName, std::ios::binary);

    unsigned char* fileHeader = BMP24_Header(height, stride);
    for (int i = 0; i < FILE_HEADER_SIZE; i++)
    {
        fout << fileHeader[i];
    }

    unsigned char* infoHeader = BMP24_info(height, width);
    for (int i = 0; i <INFO_HEADER_SIZE; i++)
    {
        fout << infoHeader[i];
    }

    int i;
    for (i = 0; i < height; i++) {
        for (int k = 0; k < 3; k++)
            for (int j = 0; j < width; j++)
            {
                fout << image[(i * widthInBytes+j)+width*k];
            }
        for (int j = 0; j < paddingSize; j++)
        {
            fout << padding[j];
        }
    }
    fout.close();
}
unsigned char* BMP24_Header(int height, int stride)
{
    int fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + (stride * height);

    static unsigned char fileHeader[] = {
        0,0,     /// signature
        0,0,0,0, /// image file size in bytes
        0,0,0,0, /// reserved
        0,0,0,0, /// start of pixel array
    };

    fileHeader[0] = (unsigned char)('B');
    fileHeader[1] = (unsigned char)('M');
    fileHeader[2] = (unsigned char)(fileSize);
    fileHeader[3] = (unsigned char)(fileSize >> 8);
    fileHeader[4] = (unsigned char)(fileSize >> 16);
    fileHeader[5] = (unsigned char)(fileSize >> 24);
    fileHeader[10] = (unsigned char)(FILE_HEADER_SIZE + INFO_HEADER_SIZE);

    return fileHeader;
}
unsigned char* BMP24_info(int height, int width)
{
    static unsigned char infoHeader[] = {
        0,0,0,0, /// header size
        0,0,0,0, /// image width
        0,0,0,0, /// image height
        0,0,     /// number of color planes
        0,0,     /// bits per pixel
        0,0,0,0, /// compression
        0,0,0,0, /// image size
        0,0,0,0, /// horizontal resolution
        0,0,0,0, /// vertical resolution
        0,0,0,0, /// colors in color table
        0,0,0,0, /// important color count
    };

    infoHeader[0] = (unsigned char)(INFO_HEADER_SIZE);
    infoHeader[4] = (unsigned char)(width);
    infoHeader[5] = (unsigned char)(width >> 8);
    infoHeader[6] = (unsigned char)(width >> 16);
    infoHeader[7] = (unsigned char)(width >> 24);
    infoHeader[8] = (unsigned char)(height);
    infoHeader[9] = (unsigned char)(height >> 8);
    infoHeader[10] = (unsigned char)(height >> 16);
    infoHeader[11] = (unsigned char)(height >> 24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(3 * 8);

    return infoHeader;
}
unsigned char* BMP24_read(int& height, int& width, char* imageFileName) {
    std::ifstream fin(imageFileName, std::ios::binary);
    char ch;
    static unsigned char fileHeader[] = {
        0,0,     /// signature
        0,0,0,0, /// image file size in bytes
        0,0,0,0, /// reserved
        0,0,0,0, /// start of pixel array
    };
    int i = 0;
    while (fin.get(ch))
    {
        fileHeader[i++]=(unsigned char)ch;
        if (i > FILE_HEADER_SIZE-1) break;
    }
    static unsigned char infoHeader[] = {
        0,0,0,0, /// header size
        0,0,0,0, /// image width
        0,0,0,0, /// image height
        0,0,     /// number of color planes
        0,0,     /// bits per pixel
        0,0,0,0, /// compression
        0,0,0,0, /// image size
        0,0,0,0, /// horizontal resolution
        0,0,0,0, /// vertical resolution
        0,0,0,0, /// colors in color table
        0,0,0,0, /// important color count
    };
    i = 0;
    while (fin.get(ch))
    {
        infoHeader[i++] = (unsigned char)ch;
        if (i > INFO_HEADER_SIZE-1) break;
    }
    width = (int)infoHeader[4] + ((int)infoHeader[5] << 8) + ((int)infoHeader[6] << 16) + ((int)infoHeader[7] << 24);
    height = (int)infoHeader[8] + ((int)infoHeader[9]<<8) + ((int)infoHeader[10]<<16) + ((int)infoHeader[11]<<24);
    unsigned char* image = new unsigned char[width * height*3];
    int widthInBytes = width * 3;

    unsigned char padding[3] = { 0, 0, 0 };
    int paddingSize = (4 - (widthInBytes) % 4) % 4;

    int stride = (widthInBytes)+paddingSize;
    for (i = 0; i < height; i++) {
        for (int k = 0; k < 3; k++)
            for (int j = 0; j < width; j++)
            {
                fin.get(ch);
                image[(i * widthInBytes + j) + width * k] = ch;
            }
        for (int j = 0; j < paddingSize; j++)
            fin.get(ch);
    }
    fin.close();
    return image;
}

void BMP32_save(unsigned char* image, int height, int width, char* imageFileName) {
    int widthInBytes = width * 4;

    unsigned char padding[3] = { 0, 0, 0 };
    int paddingSize = (4 - (widthInBytes) % 4) % 4;

    int stride = (widthInBytes)+paddingSize;

    std::ofstream fout(imageFileName, std::ios::binary);

    unsigned char* fileHeader = BMP32_Header(height, stride);
    for (int i = 0; i < FILE_HEADER_SIZE; i++)
        fout << fileHeader[i];

    unsigned char* infoHeader = BMP32_info(height, width);
    for (int i = 0; i < 124; i++)
        fout << infoHeader[i];

    int i;
    unsigned char c = 255;
    for (i = 0; i < height; i++) {
        for (int k = 0; k < 4; k++) {
            for (int j = 0; j < width; j++)
            {
                fout << image[(i * widthInBytes + j) + width * k];
            }
        }
        for (int j = 0; j < paddingSize; j++)
        {
            fout << padding[j];
        }
    }
    fout.close();
}
unsigned char* BMP32_Header(int height, int stride)
{
    int fileSize = FILE_HEADER_SIZE + 124 + (stride * height);

    static unsigned char fileHeader[] = {
        0,0,     /// signature
        0,0,0,0, /// image file size in bytes
        0,0,0,0, /// reserved
        0,0,0,0, /// start of pixel array
    };

    fileHeader[0] = (unsigned char)('B');
    fileHeader[1] = (unsigned char)('M');
    fileHeader[2] = (unsigned char)(fileSize);
    fileHeader[3] = (unsigned char)(fileSize >> 8);
    fileHeader[4] = (unsigned char)(fileSize >> 16);
    fileHeader[5] = (unsigned char)(fileSize >> 24);
    fileHeader[10] = (unsigned char)(FILE_HEADER_SIZE + 124);

    return fileHeader;
}
unsigned char* BMP32_info(int height, int width)
{
    static unsigned char infoHeader[] = {
        0,0,0,0, /// header size
        0,0,0,0, /// image width
        0,0,0,0, /// image height
        0,0,     /// number of color planes
        0,0,     /// bits per pixel
        0,0,0,0, /// compression
        0,0,0,0, /// image size
        0,0,0,0, /// horizontal resolution
        0,0,0,0, /// vertical resolution
        0,0,0,0, /// colors in color table
        0,0,0,0, /// important color count
        0,0,255,0, 
        0,255,0,0, 
        255,0,0,0, 
        0,0,0,255, 
        66,71,82,115, 
        144,194,245,40, 
        81,184,30,21, 
        30,133,235,1, 
        51,51, 51,19,
        102,102,102,38,
        102,102,102,6,
        153,153,153,9,
        61,10,215,3,
        41,92,143,50, 
        0,0,0,0, 
        0,0,0,0, 
        0,0,0,0, 
        0,0,0,0, 
        0,0,0,0, 
        0,0,0,0, 
        0,0,0,0, 
    };

    infoHeader[0] = (unsigned char)(124);
    infoHeader[4] = (unsigned char)(width);
    infoHeader[5] = (unsigned char)(width >> 8);
    infoHeader[6] = (unsigned char)(width >> 16);
    infoHeader[7] = (unsigned char)(width >> 24);
    infoHeader[8] = (unsigned char)(height);
    infoHeader[9] = (unsigned char)(height >> 8);
    infoHeader[10] = (unsigned char)(height >> 16);
    infoHeader[11] = (unsigned char)(height >> 24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(4 * 8);
    infoHeader[16] = (unsigned char)(3);
    infoHeader[20] = (unsigned char)(width*height*4);
    infoHeader[21] = (unsigned char)(width*height*4>>8);
    infoHeader[22] = (unsigned char)(width*height*4>>16);
    infoHeader[23] = (unsigned char)(width * height * 4 >> 24);
    infoHeader[24] = (unsigned char)(19);
    infoHeader[25] = (unsigned char)(11);
    infoHeader[28] = (unsigned char)(19);
    infoHeader[29] = (unsigned char)(11);

    return infoHeader;
}