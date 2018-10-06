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

const int tamCola = 5;
typedef int cola_t[tamCola];

int main() {
	SimpleSemaphore semPCCarriles("/semPCCarriles",0);
	SimpleSemaphore semCPCarriles("/semCPCarriles",10);
	SimpleSemaphore semBufferCarriles("/semBufferCarriles");
	SharedMemory<cola_t> bufferCarriles("/bufferCarriles");
	SharedMemory<int> colaCarriles("/colaCarriles");
	SharedMemory<int> cabezaCarriles("/cabezaCarriles");
	SharedMemory<int> numElemCarriles("/numElemCarriles");

	cola_t &c = bufferCarriles();
	int &i = colaCarriles();
	int &r = cabezaCarriles();
	int &n = numElemCarriles();

	int pid = getpid();
	int dato = -1;
	int pos = -1;
	for (int cont = 0; cont < 10; cont++) {
		cerr << "Consumidor " << pid  << ": Esperando que termine el productor" << endl;
		cerr.flush();
		semPCCarriles.Wait();
		cerr << "Consumidor " << pid  << ": Recibiendo dato" << endl;
		cerr.flush();
		semBufferCarriles.Wait();
		
		pos = r;
		dato = c[r];
		r = (r+1)%tamCola;
		
		semBufferCarriles.Signal();
		semCPCarriles.Signal();
		sleep(2);
		cout << "Consumidor " << pid  << ": El valor recibido en " << pos << " es " << dato << endl;
		cout.flush();
	}
	return 0;
}
