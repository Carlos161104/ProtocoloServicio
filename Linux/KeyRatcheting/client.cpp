// compile with:
//  g++ cliente.cpp -o cliente
#include <stdio.h>
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

// Reemplazo de Sleep() de Windows
#include <time.h>

using namespace std;

void Decoding(vector<unsigned char> &data, vector<int> params, size_t orden);
int nextPrime(int x);
bool isPrime(int x);
int ratchetSeed(int seed);

int ratchetSeed(int seed) {
    int A = nextPrime(50100);
    int B = nextPrime(30020);
    int BASE = nextPrime(123000);
    return (seed * A + B) % BASE;
}

// Equivalente a Sleep(ms) de Windows
void sleepMs(int ms)
{
    struct timespec ts;
    ts.tv_sec  = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
}

int main()
{
    int cliente;
    int option = 0;
    int conections = 0;
    int publicKey = 0;
     int tamanio = 1024;

    // Configurar la dirección del servidor
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5050);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");


    while (option != 1){
        // INTENTAR Conectar al servidor
        printf("Intentando conectar al servidor...\n");

        while (true)
        {
            cliente = socket(AF_INET, SOCK_STREAM, 0);
            if (connect(cliente, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == 0)
            {
                break;
            }
            close(cliente);
            sleepMs(2000);
            printf(".");
            fflush(stdout); // Forzar impresion sin salto de linea
        }

        printf("\n¡Conectado exitosamente!\n");

        if (conections == 0) {
            int publicKeyStatus = recv(cliente, (char *)&publicKey, sizeof(publicKey), 0);
            if (publicKeyStatus >= 0) {
                printf("Llave publica recibida: %d\n", publicKey);
            }
            else {
                printf("Error al recibir la llave publica\n");
                break;
            }
        }
            vector<unsigned char> receivedData(tamanio);
            vector<unsigned char> finalReceivedData;

            int dataStatus = 1;
            while (true)
            {
                dataStatus = recv(cliente, (char *)receivedData.data(), tamanio, 0);

                if (dataStatus <= 0)
                    break;

                finalReceivedData.insert(finalReceivedData.end(), receivedData.begin(), receivedData.begin() + dataStatus);
            }

            printf("Datos recibidos correctamente.\n");

            // Parametros de descifrado
            const char pass[] = "password";
            for (char c : string(pass))
                publicKey += c;

            int seed = publicKey;
            const vector<int> initial_params = {seed, nextPrime(20000), nextPrime(10000), nextPrime(100000)};
            const int orden = 5;

            stbi_write_png(
                "encoded_image.png",
                236,
                277,
                3,
                finalReceivedData.data(),
                236 * 3);

            // Llamar a la funcion de desencriptacion
            Decoding(finalReceivedData, initial_params, orden);

            stbi_write_png(
                "decoded_image.png",
                236,
                277,
                3,
                finalReceivedData.data(),
                236 * 3);
        

        // Limpieza
        close(cliente);

        // Preguntar si se desea intentar conectar de nuevo
        printf("¿Deseas intentar conectar al servidor de nuevo? (1 para sí, 0 para no): ");
        scanf("%d", &option);

        // Incrementar el contador de conexiones 
        conections++;

        // Ratchetear la semilla para la siguiente conexion
        publicKey = ratchetSeed(publicKey);
        
    }

    return 0;
}

void Decoding(vector<unsigned char> &data, vector<int> params, size_t orden)
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

            data[j] ^= (x % 256);
        }
    }
}

bool isPrime(int x)
{
    if (x < 2)
        return false;
    if (x == 2)
        return true;
    if (x % 2 == 0)
        return false;

    for (int i = 3; i * i <= x; i += 2)
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