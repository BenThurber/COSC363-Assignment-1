//=====================================================================
// LoadBMP.h
// Minimal image loader for files in BMP format.
// Assumption:  24 bits per pixel
//
// Author:
// R. Mukundan, Department of Computer Science and Software Engineering
// University of Canterbury, Christchurch, New Zealand.
//=====================================================================

#if !defined(H_BMP)
#define H_BMP

#include <algorithm>
#include <cstring>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <GL/freeglut.h>
using namespace std;

void loadBMP(char* filename, int mipmap_level)
{
    char* imageData;
	char header1[18], header2[24];
	short int planes, bpp;
    int wid, hgt;
    int nbytes, size, indx, temp;
    ifstream file( filename, ios::in | ios::binary);
	if(!file)
	{
		cout << "No BMP image file named: " << filename << endl;
        return;
	}
	file.read (header1, 18);		//Initial part of header
	file.read ((char*)&wid, 4);		//Width
	file.read ((char*)&hgt, 4);		//Height
	file.read ((char*)&planes, 2);	//Planes
	file.read ((char*)&bpp, 2);		//Bits per pixel
	file.read (header2, 24);		//Remaining part of header

//		cout << "Width =" << wid << "   Height = " << hgt << " Bpp = " << bpp << endl;

	nbytes = bpp / 8;           //No. of bytes per pixels
	size = wid * hgt * nbytes;  //Total number of bytes to be read
	imageData = new char[size];
	file.read(imageData, size);
	//Swap r and b values
	for(int i = 0; i < wid*hgt;  i++)
	{
	    indx = i*nbytes;
	    temp = imageData[indx];
	    imageData[indx] = imageData[indx+2];
	    imageData[indx+2] = temp;
    }
	glTexImage2D(GL_TEXTURE_2D, mipmap_level, 3, wid, hgt, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    delete imageData;
    
    
    
    
    // Try to open the next mipmap recursivley >:)
    char* new_filename = NULL;
    char* c = strrchr(filename, '.') - 1;     // The last character of the filename before the dot
    
    int num_len = 0;
    while (*c >= '0' && *c <= '9' && c > new_filename) {   // Itterate backwards in the filename
        c--;
        num_len++;
    }
    
    if (num_len <= 0 || mipmap_level < 0) return;    // Exit if the filename doen't end in a number
    
    const int N = 10;
    // The string in the filename representing the mipmap level
    char mipmap_str[N + 1] = {'\0'};   // Allocate N+1 so that the array can be used for the incremented value
    
    strncpy(mipmap_str, c+1, min(num_len, N-1));
    int mipmap_num = atoi(mipmap_str);
    
    mipmap_num++;   // Increment the filename number
    
    snprintf(mipmap_str, sizeof(mipmap_str), "%d", mipmap_num);
    
    int new_str_len = (int) ((c - filename) + strlen(mipmap_str) + strlen(c + num_len) + 1); // Calculate new string len
    new_filename = new char[new_str_len];
    fill(new_filename, new_filename + new_str_len, '\0');          // Initialize to zeros
    strncat(new_filename, filename, (c - filename + 1));            // Concat filename before number
    strncat(new_filename, mipmap_str, strlen(mipmap_str));          // Concat number
    strncat(new_filename, c + num_len + 1, strlen(c + num_len));    // Concat file extension
    
//    printf("Level %d\n Old file: %s\n New file: %s\n\n", mipmap_level, filename, new_filename);
    loadBMP(new_filename, mipmap_level + 1);     // And call function again
    
    delete new_filename;
    
}

#endif

