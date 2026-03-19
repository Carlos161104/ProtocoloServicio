#include <stdio.h>
#include <ctime>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

// Function to read image and convert to pixel vector
vector<unsigned char> readImageToVector(const char *filename, int &width, int &height, int &channels)
{
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 0);
    vector<unsigned char> pixels(data, data + (width * height * channels));
    stbi_image_free(data);
    return pixels;
}

// Function to recreate and save image from pixel vector
void saveVectorToImage(const char *filename, int width, int height, int channels, const vector<unsigned char> &pixels)
{
    stbi_write_jpg(filename, width, height, channels, pixels.data(), 100);
}

// Usage example in main
int main()
{
    int width, height, channels;
    vector<unsigned char> pixels = readImageToVector("./gato.jpg", width, height, channels);

    // Optionally modify pixels here...

    saveVectorToImage("./newimage.jpg", width, height, channels, pixels);

    return 0;
}