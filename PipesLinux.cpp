#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <vector>

using namespace std;
int nrp = 10;  //numar procese
int n = 10000; //numarul total de numere

bool isPrime(int nr)
{
    //functie ce verifica daca numerele sunt prime
    int i;
    for (i = 2; i <= nr / 2; i++)
    {
        if (nr % i == 0)
            return false;
    }
    return true;
}

void childProc(int readPipe, int writePipe)
{

    int i = 1, j;
    int v[1000] = {};
    /*citim din pipe numarul procesului */
    read(readPipe, &i, sizeof(int));
    /*inceputul partii si sf partii */
    for (j = i * (n / nrp) + 1; j <= (i + 1) * (n / nrp); j++)
    {
        if (j <= n)
        {
            if (isPrime(j))
            {
                v[0]++;      //v[0]= size-ul array-ului de numere prime
                v[v[0]] = j; // adaugam numarul j ca fiind prim
            }
        }
    }
    write(writePipe, &v, 4000);

    exit(1);
}

void parentProc(int readPipe, int writePipe)
{
    int i, j;
    int val[1000] = {};

    /*se scrie pt. fiecare proces numarul sau in conducta*/
    for (j = 0; j < nrp; j++)
        /*scriem in pipe numarul procesului */
        write(writePipe, &j, sizeof(int));
    for (j = 0; j < nrp; j++)
    {
        cout << "Proces:" << j << endl;
        /*citim din pipe numere prime ale fiecarui proces */
        read(readPipe, val, 4000);
        for (i = 1; i <= val[0]; i++)
        {
            cout << " " << val[i];
        }
        cout << ("\n");
    }
}

int main()
{
    int i;
    int j;
    int firstPipe[2]; /*0-citit 1-scris*/
    int secondPipe[2];
    int pid = 1;

    if (pipe(firstPipe))
    {
        cout << ("error creare conducta!\n");
        exit(-1);
    }

    if (pipe(secondPipe))
    {
        cout << ("error creare conducta!\n");
        exit(-1);
    }
    for (j = 0; j < nrp; j++)
    {
        pid = fork();
        if (pid)
        {
            if (pid == -1)
            {
                cout << ("error creare proc. copil.\n");
                exit(-1);
            }
        }
        else
        {
            childProc(firstPipe[0], secondPipe[1]);
        }
    }
    parentProc(secondPipe[0], firstPipe[1]);
}