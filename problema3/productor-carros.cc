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

#ifndef __SYS_TYPES_H_
#include <sys/types.h>
#define __SYS_TYPES_H_
#endif

#ifndef __UNISTD_H_
#include <unistd.h>
#define __UNISTD_H_
#endif

#ifndef _STDLIB_H_
#include <stdlib.h>
#define _STDLIB_H_
#endif

using std::cout;
using std::cerr;
using std::endl;

const int tamCola = 3;
typedef int cola_t[tamCola];

int main() {
	SimpleSemaphore semPC("/semPCPesas",0);
	SimpleSemaphore semCP("/semCPPesas",tamCola);
	SimpleSemaphore semBufferPesas("/semColaPesas");
	SharedMemory<cola_t> buffer("/bufferPesas");
	SharedMemory<int> cola("/colaPesas");
	SharedMemory<int> cabeza("/cabezaPesas");
	SharedMemory<int> numElem("/numElemPesas");

	cola_t &c = buffer();
	int &i = cola();
	int &r = cabeza();
	int &n = numElem();

	//Productor de camiones
	int pid = getpid();
	while (true) {
		int pos = -1;
		cerr << "Productor de camiones " << pid <<": Esperando por buffer de pesas" << endl;
		cerr.flush();
		semCP.Wait();
		semBufferPesas.Wait();

		int peso = (rand() % 5) + 1;

		pos = i;
		c[i] = peso;
		i = (i+1)%tamCola;
		n++;

		semBufferPesas.Signal();
		cout << "Productor " << pid << ": Generó "  << pos << " el nuevo camión con peso " << peso << endl;
		cout.flush();
		semPC.Signal();

		int tiempo = (rand() % 3) + 1;
		cout << "Esperando " << tiempo << " segundos" << endl;
		cout.flush();
		sleep(tiempo);
	}
	return 0;
}
