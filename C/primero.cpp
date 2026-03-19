#include <stdio.h>
#include <ctime>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

vector<int> Encoder(const vector<unsigned char> &pixels, int seed, int width, int height, int channels);
vector<int> Decoder(const vector<int> &encoded, int seed, int width, int height, int channels);
int nextPrime(int x);
bool isPrime(int x);

#define A nextPrime(20000)
#define B nextPrime(10000)
#define BASE nextPrime(100000)

int main()
{
    int seed;
    int width, height, channels;
    unsigned char *data = stbi_load("./gato.jpg", &width, &height, &channels, 0);
    vector<unsigned char> pixels(data, data + width * height * channels);
    stbi_image_free(data);

    vector<int> X;
    vector<int> Y;

    X = Encoder(pixels, seed, width, height, channels);
    printf("Se encripto la imagen con la semilla: %d\n", seed);

    Y = Decoder(X, seed, width, height, channels);
    printf("Se desencripto la imagen con la semilla: %d\n", seed);

    return 0;
}

vector<int> Encoder(const vector<unsigned char> &pixels, int seed, int width, int height, int channels)
{
    char pass[] = "password";
    int x = (int)time(nullptr);
    vector<int> encoded;

    for (int i = 0; pass[i] != '\0'; ++i)
    {
        x += (int)pass[i];
    }
    seed = x;

    for (size_t i = 0; i < pixels.size(); ++i)
    {
        x = (x * A + B) % BASE;
        int encoded_pixel = (pixels[i] + x) % 256;
        encoded.push_back(encoded_pixel);
    }

    stbi_write_png(
        "coded_image.png",
        width,
        height,
        channels,
        pixels.data(),
        width * channels);

    return encoded;
}

vector<int> Decoder(const vector<int> &encoded, int seed, int width, int height, int channels)
{
    vector<int> decoded;
    int x = seed;

    for (size_t i = 0; i < encoded.size(); ++i)
    {
        x = (x * A + B) % BASE;
        int decoded_pixel = (encoded[i] - x + 256) % 256;
        decoded.push_back(decoded_pixel);
    }

    vector<unsigned char> pixels(decoded.begin(), decoded.end());

    stbi_write_png(
        "decoded_image.png",
        width,
        height,
        channels,
        pixels.data(),
        width * channels);

    return decoded;
}

bool isPrime(int x)
{
    if (x < 2)
        return false;
    for (int i = 2; i * i <= x; ++i)
    {
        if (x % i == 0)
            return false;
    }
    return true;
}

int nextPrime(int x)
{
    x = x + 1;
    while (!isPrime(x))
    {
        x = x + 1;
    }
    return x;
}

// optimizar el algoritmito
// agregar niveles de codificacion (generar los parametros inicailes de manera aleatoria)
// implementar miltihilos para encriptar

// 1. Si es de orden 4 por ejemplo, generar condiciones iniciales de los sigueintes 3 ordenes (las siguientes 3 semillas)
// 2. Se gaurda los 3 primos usados y el factor inicial (las siguinetes semillas y siguientes primos se generan
// a partir de la semilla inicial y los primos iniciales)
