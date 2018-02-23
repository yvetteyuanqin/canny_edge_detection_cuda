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

void png_read(string filename){
    imgInfo fingerprint;
    Image img;
    img.read(filename);
    fingerprint.height = img.rows();
    fingerprint.width = img.columns();
    fingerprint.depth = img.depth();
    
    /*    Geometry geo(fingerprint.width,fingerprint.height);
    img.scale(geo);
    */
   unsigned char **pixels = new unsigned char *[fingerprint.width];
    for(int i=0;i<fingerprint.width;i++){
        pixels[i] = new unsigned char [fingerprint.height];
    }
    
    img.write(0,0,fingerprint.width,fingerprint.height,"R",CharPixel, pixels);
    for(int i =0; i<fingerprint.width; i++){
        for(int j=0;j<fingerprint.height;j++){
            cout << pixels[i][j];
        }
        cout << endl;
    }
    
}

int main(){
    string filename="001.png";
    png_read(filename);

}
