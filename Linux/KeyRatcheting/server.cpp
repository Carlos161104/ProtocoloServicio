// compile with:
//  g++ sockets.cpp -o sockets -lm

#include <stdio.h>
#include <ctime>
#include <vector>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// POSIX sockets (Linux)
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

vector<unsigned char> ReadImage(int &width, int &height, int &channels, const string &path);
void Encoder(vector<unsigned char> &data, vector<int> &params, size_t orden);
int nextPrime(int x);
bool isPrime(int x);
int ratchetSeed(int seed);

// RATCHET: mismos primos que el cliente
int ratchetSeed(int seed) {
    int A = nextPrime(50100);
    int B = nextPrime(30020);
    int BASE = nextPrime(123000);
    return (seed * A + B) % BASE;
}

int main()
{
    // --- Semilla inicial ---
    const char pass[] = "password";
    int x = (int)time(nullptr);
    const int publicKey = x;   // lo que se envia al cliente UNA sola vez

    // El servidor suma el password igual que el cliente lo hara despues de recibirla
    for (char c : string(pass))
        x += c;

    // publicKey es la semilla antes del password
    // x       es la semilla activa (con password), usada para cifrar
    // El ratchet opera sobre publicKey (igual que el cliente)
    int ratchetKey = publicKey; // variable que evoluciona igual que en el cliente
    int conections = 0;

    // --- Setup del socket ---
    int server, client;
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(5050);

    server = socket(AF_INET, SOCK_STREAM, 0);

    // Permite reusar el puerto inmediatamente tras cerrar
    int opt = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bind(server, (struct sockaddr *)&address, sizeof(address));
    listen(server, 5);

    printf("Servidor listo en puerto 5050.\n");

    while (true)
    {
        printf("Esperando conexion...\n");
        client = accept(server, NULL, NULL);

        if (client < 0) {
            printf("Error en accept, continuando...\n");
            continue;
        }

        printf("Conexion recibida! (sesion %d)\n", conections + 1);

        // Solo en la primera conexion enviamos la publicKey
        if (conections == 0) {
            send(client, (const char *)&publicKey, sizeof(publicKey), 0);
            printf("Llave publica enviada: %d\n", publicKey);
        }

        // Reconstruir la semilla activa igual que lo hara el cliente:
        // cliente recibe publicKey → suma password → esa es su semilla de descifrado
        // el servidor debe cifrar con esa misma semilla
        int activeSeed = ratchetKey;
        for (char c : string(pass))
            activeSeed += c;

        // Leer y cifrar la imagen
        int width, height, channels;
        vector<unsigned char> imageData = ReadImage(width, height, channels, "./gato.jpg");

        vector<int> params = {activeSeed, nextPrime(20000), nextPrime(10000), nextPrime(100000)};
        const int orden = 5;
        Encoder(imageData, params, orden);

        // Enviar imagen cifrada
        const int datosSize = imageData.size();
        int count = 0;
        while (count < datosSize) {
            int bytesToSend = min(1024, datosSize - count);
            int sent = send(client, (const char *)imageData.data() + count, bytesToSend, 0);
            if (sent <= 0) break;
            count += sent;
        }

        close(client);
        printf("Imagen enviada.\n");

        conections++;

        // *** RATCHET: avanzar ratchetKey igual que el cliente ***
        ratchetKey = ratchetSeed(ratchetKey);
        printf("Ratchet aplicado. Listo para la siguiente sesion.\n");
    }

    close(server);
    return 0;
}

vector<unsigned char> ReadImage(int &width, int &height, int &channels, const string &path)
{
    unsigned char *image = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (!image) {
        printf("Error: no se pudo cargar la imagen en %s\n", path.c_str());
        return {};
    }
    vector<unsigned char> data(image, image + width * height * channels);
    stbi_image_free(image);
    return data;
}

void Encoder(vector<unsigned char> &data, vector<int> &params, size_t orden)
{
    const size_t n = data.size();
    for (size_t i = 0; i < orden; ++i)
    {
        int x = params[0], A = params[1], B = params[2], BASE = params[3];

        for (size_t j = 0; j < n; ++j)
        {
            x = (x * A + B) % BASE;

            if (j < 4)
                params[j] = x;

            data[j] ^= static_cast<unsigned char>(x);
        }
    }
}

bool isPrime(int x)
{
    if (x < 2) return false;
    if (x == 2) return true;
    if (x % 2 == 0) return false;
    for (int i = 3; i * i <= x; i += 2)
        if (x % i == 0) return false;
    return true;
}

int nextPrime(int x)
{
    x = x + 1;
    while (!isPrime(x))
        x = x + 1;
    return x;
}