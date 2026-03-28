#include <stdio.h>
#include <ctime>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

void Encoder(vector<unsigned char> &data, vector<int> params, size_t orden);
int nextPrime(int x);
bool isPrime(int x);

int main()
{
    int width, height, channels;
    // Leemos la imagen y la guardamos en un vector de unsigned char
    unsigned char *image = stbi_load("./gato.jpg", &width, &height, &channels, 0);

    // Convertimos el puntero a un vector de unsigned char para facilitar su manejo
    vector<unsigned char> data(image, image + width * height * channels);

    // Liberamos la memoria asignada por stbi_load
    stbi_image_free(image);

    const char pass[] = "password";   // Este es nuestro password (clave privada)
    int x = (int)time(nullptr); // Semilla a la cual sumar password

    // Bucle para sumar la contrasena al valor de la semilla
    for (size_t i = 0; pass[i] != '\0'; ++i)
    {
        x += (int)pass[i];
    }

    const int seed = x;
    const vector<int> initial_params = {x, nextPrime(20000), nextPrime(10000), nextPrime(100000)}; // {x (semilla inicial), a, b , base}
    const int orden = 5; // Cantidad de veces que se encripta la imagen

    // Mandamos llamar a la funcion recursiva
    Encoder(data, initial_params, orden);
    printf("Se encripto la imagen con la semilla: %d\n", seed);

    stbi_write_png(
        "encoded_image.png",
        width,
        height,
        channels,
        data.data(),
        width * channels
    );

    Encoder(data, initial_params, orden);
    vector<unsigned char> result(data.begin(), data.end());
    stbi_write_png(
        "decoded_image.png",
        width,
        height,
        channels,
        result.data(),
        width * channels
    );

    printf("Se desencripto la imagen con la semilla: %d\n", seed);

    return 0;
}

void Encoder(vector<unsigned char> &data, vector<int> params, size_t orden)
{
    //recore el orden
    for (size_t i = 0; i < orden; ++i)
    {
        int x = params[0], A = params[1], B = params[2], BASE = params[3];

        //recorre el anillo congruencial
        for (size_t j = 0; j < data.size(); ++j)
        {
            x = (x * A + B) % BASE;

            //Capturamos los primeros 4 para la SIGUIENTE capa (orden)
            if (j < 4) params[j] = x;
            // Aplicamos el bitxor
            data[j] ^= (x % 256);
            
        }
    }
}

bool isPrime(int x) {
    if (x < 2) return false;
    if (x == 2) return true;    // El 2 es el único par primo
    if (x % 2 == 0) return false; // Si es par fuera.

    // Empezamos en 3 y saltamos de 2 en 2 (3, 5, 7, 9...)
    for (int i = 3; i * i <= x; i += 2) {
        if (x % i == 0) return false;
    }
    return true;
}

int nextPrime(int x)
{
    x = x + 1; //-----------sobra?
    while (!isPrime(x))
    {
        x = x + 1;
    }
    return x;
}