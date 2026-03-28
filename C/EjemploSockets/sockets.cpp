//compile with:
// g++ .\sockets.cpp -o sockets.exe -lws2_32
#include <stdio.h>
#include <ctime>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Para el uso de sockets
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
    // INICIALIZAR WINSOCK 
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
            
            // imagen encriptada
            vector<int> datos = {2, 3, 4, 5};
            int datosSize = datos.size();

            // enviar tamaño de info
            send(client, (const char *)&datosSize, sizeof(datosSize), 0);

            // enviar info
            send(client, (const char *)datos.data(), datosSize * sizeof(int), 0);

            closesocket(client);
        }
    }
    closesocket(server);
    WSACleanup();

    return 0;
}