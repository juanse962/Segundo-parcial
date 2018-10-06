#ifndef __SIMPLESEMAPHORE_H_
#include <SimpleSemaphore.h>
#define __SIMPLESEMAPHORE_H_
#endif

#ifndef __SHAREDMEMORY_H_
#include <SharedMemory.h>
#define __SHAREDMEMORY_H_
#endif

#ifndef __IOSTREAM__
#include <iostream>
#define __IOSTREAM__
#endif

#ifndef __UNISTD_H_
#include <unistd.h>
#define __UNISTD_H_
#endif

using std::cout;
using std::cerr;
using std::endl;

const int tamCola = 3;
const int tamColaCarriles = 5;
typedef int cola_t[tamCola];

bool pesas[3] = {false};

int buscarPesaLibre() {
	int i = 0;
	while (pesas[i] && i < 3) {
		i++;
	}
	if (i >= 3) {
		//No se encontró ninguna pesa libre
		return -1;
	} else {
		return i;
	}
}

int main() {
	SimpleSemaphore semPCPesas("/semPCPesas",0);
	SimpleSemaphore semCPPesas("/semCPPesas",tamCola);
	SimpleSemaphore semBufferPesas("/semBufferPesas");
	SharedMemory<cola_t> bufferPesas("/bufferPesas");
	SharedMemory<int> colaPesas("/colaPesas");
	SharedMemory<int> cabezaPesas("/cabezaPesas");
	SharedMemory<int> numElemPesas("/numElemPesas");

	cola_t &c = bufferPesas();
	int &i = colaPesas();
	int &r = cabezaPesas();
	int &n = numElemPesas();

	SimpleSemaphore semPCCarriles("/semPCCarriles",0);
	SimpleSemaphore semCPCarriles("/semCPCarriles",tamColaCarriles);
	SimpleSemaphore semBufferCarriles("/semBufferCarriles");
	SharedMemory<cola_t> bufferCarriles("/bufferCarriles");
	SharedMemory<int> colaCarriles("/colaCarriles");
	SharedMemory<int> cabezaCarriles("/cabezaCarriles");
	SharedMemory<int> numElemCarriles("/numElemCarriles");

	cola_t &cC = bufferCarriles();
	int &iC = colaCarriles();
	int &rC = cabezaCarriles();
	int &nC = numElemCarriles();

	int pid = getpid();
	int dato = -1;
	int pos = -1;
	int  arr[3];

	while (true) {
		cerr << "Consumidor pesas " << pid  << ": Esperando que termine el productor" << endl;
		cerr.flush();
		semPCPesas.Wait();
		
		cerr << "Consumidor pesas " << pid  << ": Recibiendo dato" << endl;
		arr[pos] = pid;
		cerr.flush();
		semBufferPesas.Wait();
		
		pos = r;
		dato = c[r];
		r = (r+1)%tamCola;
		
		semBufferPesas.Signal();
		
		cout << "Consumidor pesas " << pid  << ": El valor recibido es " << dato << endl;
		cout.flush();

		int pesaLibre = buscarPesaLibre();
		semCPPesas.Signal();

		//Se demora 3 segundos para pesar
		sleep(3);
	}
	return 0;
}
