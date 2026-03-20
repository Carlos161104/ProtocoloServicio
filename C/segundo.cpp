#include <stdio.h>
#include <ctime>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

vector<int> Encoder(const vector<int> &pixels, vector<int> initial_params, int orden);
int nextPrime(int x);
bool isPrime(int x);

#define A nextPrime(20000)
#define B nextPrime(10000)
#define BASE nextPrime(100000)

int main()
{
    int seed;
    int width, height, channels;
    // Leemos la imagen y la guardamos en un vector de unsigned char
    unsigned char *data = stbi_load("./gato.jpg", &width, &height, &channels, 0);
    // Convertimos el puntero a un vector de unsigned char para facilitar su manejo
    vector<int> pixels(data, data + width * height * channels);
    // Liberamos la memoria asignada por stbi_load
    stbi_image_free(data);

    vector<int> X; // Sera nuestro vector ya codificado
    vector<int> Y; // Sera nuestro vector ya decodificado

    char pass[] = "password";   // Este esnuestro password (clave privada)
    int x = (int)time(nullptr); // Semilla a la caulsumar password

    // Bucle para sumar la contrasena al valor de la semilla
    for (int i = 0; pass[i] != '\0'; ++i)
    {
        x += (int)pass[i];
    }
    seed = x;

    vector<int> initial_params = {A, B, BASE, x}; // {a, b , base , x (semilla inicial)}

    int orden = 4; // Orden de la encriptacion (cantidad de veces que se encripta la imagen)

    // Mandamos llamar a la funcioncita recursiva
    X = Encoder(pixels, initial_params, orden);
    printf("Se encripto la imagen con la semilla: %d\n", seed);

    Y = Encoder(X, initial_params, orden);

    vector<unsigned char> result(Y.begin(), Y.end());

    stbi_write_png(
        "decoded_image.png",
        width,
        height,
        channels,
        result.data(),
        width * channels);

    printf("Se desencripto la imagen con la semilla: %d\n", seed);

    return 0;
}

vector<int> Encoder(const vector<int> &pixels, vector<int> initial_params, int orden)
{
    vector<int> encoded;
    int Xseg = initial_params[0];
    int Aseg = initial_params[1];
    int Bseg = initial_params[2];
    int BASEseg = initial_params[3];

    initial_params.clear();

    for (size_t i = 0; i < pixels.size(); ++i)
    {
        Xseg = (Xseg * Aseg + Bseg) % BASEseg;
        if (i < 4)
        {
            initial_params.push_back(Xseg);
        }
        int encoded_pixel = pixels[i] ^ (Xseg % 256); // Aplicamos el bitxor
        encoded.push_back(encoded_pixel);
    }

    if (orden > 1)
    {
        encoded = Encoder(encoded, initial_params, orden - 1);
    }
    return encoded;
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