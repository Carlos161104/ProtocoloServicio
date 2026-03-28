#include <stdio.h>
#include <vector>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

void Decoding(vector<unsigned char> &data, vector<int> params, size_t orden);
int nextPrime(int x);
bool isPrime(int x);

using namespace std;

int main()
{
    // Despertar los sockets de Windows
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Crear el socket del cliente
    int cliente;

    // Configurar la dirección del servidor
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5050);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //--------------------------

    // INTENTAR Conectar al servidor
    printf("Intentando conectar al servidor...\n");
    while (true)
    {
        cliente = socket(AF_INET, SOCK_STREAM, 0);
        if (connect(cliente, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == 0)
        {
            break;
        }
        // Espera 2000ms (2 segundos) antes de volver a preguntar.
        // Esto hace que tu CPU esté tranquila.
        closesocket(cliente);
        Sleep(2000);
        printf(".");
    }

    printf("¡Conectado exitosamente!\n");

    int publciKey = 0;
    int bytesRes = recv(cliente, (char *)&publciKey, sizeof(publciKey), 0);

    // Recibir el número de elementos (el tamaño)
    int tamanio = 0;
    int bytesReceived = recv(cliente, (char *)&tamanio, sizeof(tamanio), 0);

    if (bytesReceived > 0)
    {
        printf("El servidor enviara %d datos.\n", tamanio);

        // Preparar el vector y recibir los datos
        // Redimensionamos el vector para que tenga espacio suficiente
        vector<unsigned char> receivedData(tamanio);

        // Recibimos los bytes directamente en la memoria del vector
        int dataBytes = recv(cliente, (char *)receivedData.data(), tamanio * sizeof(int), 0);

        if (dataBytes > 0)
        {
            printf("Datos recibidos correctamente.");
        }

        // Parametros de desifrado
        const char pass[] = "password";
        // Bucle para sumar la contrasena al valor de la semilla
        for (size_t i = 0; pass[i] != '\0'; ++i)
        {
            publciKey += (int)pass[i];
        }
        const vector<int> initial_params = {publciKey, nextPrime(20000), nextPrime(10000), nextPrime(100000)}; // {x (semilla inicial), a, b , base}
        const int orden = 5;

        // Llamar a la funcion de desenctriptacion
        Decoding(receivedData, initial_params, orden);
    }

    // 5. Limpieza
    closesocket(cliente);
    WSACleanup();

    printf("Presiona Enter para salir...");
    getchar();

    return 0;
}

void Decoding(vector<unsigned char> &data, vector<int> params, size_t orden)
{
    // recore el orden
    for (size_t i = 0; i < orden; ++i)
    {
        int x = params[0], A = params[1], B = params[2], BASE = params[3];

        // recorre el anillo congruencial
        for (size_t j = 0; j < data.size(); ++j)
        {
            x = (x * A + B) % BASE;

            // Capturamos los primeros 4 para la SIGUIENTE capa (orden)
            if (j < 4)
                params[j] = x;
            // Aplicamos el bitxor
            data[j] ^= (x % 256);
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