#include <stdio.h>
#include <ctime>
#include <vector>

using namespace std;

vector<int> Encoder(int &seed);
vector<int> Decoder(const vector<int> &encoded, int seed);
int nextPrime(int x);
bool isPrime(int x);

#define A nextPrime(20000)
#define B nextPrime(10000)
#define BASE nextPrime(100000)

int main()
{
    int seed;
    vector<int> X;
    vector<int> Y;

    X = Encoder(seed);
    printf("The encoded message is:\n");
    for (size_t i = 0; i < X.size(); ++i)
    {
        printf("%d ", X[i]);
    }
    printf("\n");

    Y = Decoder(X, seed);
    printf("The decoded message is:\n");
    for (size_t i = 0; i < Y.size(); ++i)
    {
        printf("%c", (char)Y[i]);
    }
    printf("\n");

    return 0;
}

vector<int> Encoder(int &seed)
{
    char pass[] = "password";
    int x = (int)time(nullptr);
    vector<int> encoded;

    for (int i = 0; pass[i] != '\0'; ++i)
    {
        x += (int)pass[i];
    }
    seed = x;

    char message[] = "This is a secret message.";
    for (int i = 0; message[i] != '\0'; ++i)
    {
        x = (x * A + B) % BASE;
        int encoded_char = ((int)message[i] + x) % 256;
        encoded.push_back(encoded_char);
    }

    return encoded;
}

vector<int> Decoder(const vector<int> &encoded, int seed)
{
    vector<int> decoded;
    int x = seed;

    for (size_t i = 0; i < encoded.size(); ++i)
    {
        x = (x * A + B) % BASE;
        int decoded_char = (encoded[i] - x + 256) % 256;
        decoded.push_back(decoded_char);
    }

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

// Function to find the next prime after x
int nextPrime(int x)
{
    x = x + 1;
    while (!isPrime(x))
    {
        x = x + 1;
    }
    return x;
}
