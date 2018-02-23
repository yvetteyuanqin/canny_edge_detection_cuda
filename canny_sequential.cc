/**
 *  Sequential canny detection
 *
 *
 */
//Yuan Qin
//Lai Man Tang

#include <assert.h>
#include <string.h>
#include <chrono>
#include <math.h>
#include <Magick++.h>
#include <iostream>

#define pixel_t uint16_t

using namespace std;
using namespace Magick;

struct imgInfo{
    int height;
    int width;
    int depth;
};

void png_read(string &filename){
    imgInfo fingerprint;
    Image img;
    img.read(filename);
    fingerprint.height = img.columns();
    fingerprint.width = img.colums();
    fingerprint.depth = img.depth();
}

void main(){
    string *filename="001.png";
    png_read(filename);
    print()
}
