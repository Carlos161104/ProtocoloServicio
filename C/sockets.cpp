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
// #include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int main()
{
    int server = 0, client = 0;
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(5050);

    printf("Servidor con dirección: %d\n", address.sin_addr.s_addr);

    // Socket con TPC IPv4
    server = socket(address.sin_family, SOCK_STREAM, 0);

    bind(server, (struct sockaddr *)&address, sizeof(address));

    listen(server, 1);

    printf("Esperando conexión\n");

    while (true)
    {
        client = accept(server, NULL, NULL);

        if (client == INVALID_SOCKET)
        {
            // Si llega aquí sin un cliente, WSAGetLastError te dirá por qué

            continue; // Reintenta esperar
        }

        printf("conexión recibida\n");

        // imagen encriptada
        vector<int> encoded = {2, 3, 4, 5};
        int mesageSize = encoded.size();

        // enviar tamaño de info
        send(client, (const char *)&mesageSize, sizeof(mesageSize), 0);

        // enviar info
        send(client, (const char *)encoded.data(), mesageSize * sizeof(int), 0);

        closesocket(client);
    }
    closesocket(server);
    WSACleanup();

    return 0;
}