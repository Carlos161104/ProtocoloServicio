#include <stdio.h>
#include <vector>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
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
    while( true ) {
        cliente = socket(AF_INET, SOCK_STREAM, 0);
        if (connect(cliente, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == 0) {
            break;
        }
        // Espera 2000ms (2 segundos) antes de volver a preguntar. 
        // Esto hace que tu CPU esté tranquila.
        closesocket(cliente);
        Sleep(2000);
        printf(".");
    }
    
    printf("¡Conectado exitosamente!\n");

    // Recibir el número de elementos (el tamaño)
    int tamanio = 0;
    int bytesReceived = recv(cliente, (char*)&tamanio, sizeof(tamanio), 0);
    
    if (bytesReceived > 0) {
        printf("El servidor enviara %d datos.\n", tamanio);

        // Preparar el vector y recibir los datos 
        // Redimensionamos el vector para que tenga espacio suficiente
        vector<int> receivedData(tamanio);

        // Recibimos los bytes directamente en la memoria del vector
        int dataBytes = recv(cliente, (char*)receivedData.data(), tamanio * sizeof(int), 0);

        if (dataBytes > 0) {
            printf("Datos recibidos correctamente.");
            
        }
    }

    // 5. Limpieza
    closesocket(cliente);
    WSACleanup();
    
    printf("Presiona Enter para salir...");
    getchar();

    return 0;
}