// compile with:
//  g++ .\sockets.cpp -o sockets.exe -lws2_32
#include <stdio.h>
#include <ctime>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <string>

// Para el uso de sockets
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

vector<unsigned char> ReadImage(int &width, int &height, int &channels, const string &path);
void Encoder(vector<unsigned char> &data, vector<int> &params, size_t orden);
int nextPrime(int x);
bool isPrime(int x);

int main()
{
    // Leemos la imagen y la almacenamos en el vector
    int width, height, channels;
    vector<unsigned char> imageData = ReadImage(width, height, channels, "./gato.jpg");

    // Declaramos los paarametros para la encriptacion
    const char pass[] = "password"; // Este es nuestro password (clave privada)
    int x = (int)time(nullptr);     // Semilla a la cual sumar password
    const int publicKey = x;        // La clave publica es la semilla sin sumar el password

    // Bucle para sumar la contrasena al valor de la semilla
    for (char c : string(pass))
        x += c;

    vector<int> initial_params = {x, nextPrime(20000), nextPrime(10000), nextPrime(100000)}; // {x (semilla inicial), a, b , base}
    const int orden = 5;

    Encoder(imageData, initial_params, orden);

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    int server, client;
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(5050);

    printf("Servidor con direccion: %d\n", address.sin_addr.s_addr);

    // Socket con TPC IPv4
    server = socket(address.sin_family, SOCK_STREAM, 0);

    bind(server, (struct sockaddr *)&address, sizeof(address));

    listen(server, 5);

    printf("Esperando conexion\n");

    while (true)
    {
        client = accept(server, NULL, NULL);

        if (client != INVALID_SOCKET)
        {
            printf("Conexion recibida!\n");

            send(client, (const char *)&publicKey, sizeof(publicKey), 0);

            // Calculamos el tamaño de imageData
            const int datosSize = imageData.size();

            // enviar info
            int count = 0;
            while (count < datosSize)
            {
                int bytesToSend = min(1024, datosSize - count);
                short sent = send(client, (const char *)imageData.data() + count, bytesToSend, 0);

                if (sent <= 0)
                    break;

                count += bytesToSend;
            }

            closesocket(client);
        }
    }
    closesocket(server);
    WSACleanup();

    return 0;
}

vector<unsigned char> ReadImage(int &width, int &height, int &channels, const string &path)
{
    unsigned char *image = stbi_load(path.c_str(), &width, &height, &channels, 0);
    vector<unsigned char> data(image, image + width * height * channels);
    stbi_image_free(image);
    return data;
}

void Encoder(vector<unsigned char> &data, vector<int> &params, size_t orden)
{
    const size_t n = data.size();
    // recore el orden
    for (size_t i = 0; i < orden; ++i)
    {
        int x = params[0], A = params[1], B = params[2], BASE = params[3];

        // recorre el anillo congruencial
        for (size_t j = 0; j < n; ++j)
        {
            x = (x * A + B) % BASE;

            // Capturamos los primeros 4 para la SIGUIENTE capa (orden)
            if (j < 4)
                params[j] = x;
            // Aplicamos el bitxor
            data[j] ^= static_cast<unsigned char>(x); // Es lo mismo que x % 256, pero mas directo y eficiente
        }
    }
}

bool isPrime(int x)
{
    if (x < 2)
        return false;
    if (x == 2)
        return true; // El 2 es el único par primo
    if (x % 2 == 0)
        return false; // Si es par fuera.

    // Empezamos en 3 y saltamos de 2 en 2 (3, 5, 7, 9...)
    for (int i = 3; i * i <= x; i += 2)
    {
        if (x % i == 0)
            return false;
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