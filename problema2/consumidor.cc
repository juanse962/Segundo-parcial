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
	SimpleSemaphore semPC("/semPC",0);
	SimpleSemaphore semCP("/semCP",10);
	SimpleSemaphore semBuffer("/semBuffer");
	SharedMemory<cola_t> buffer("/buffer");
	SharedMemory<int> cola("/cola");
	SharedMemory<int> cabeza("/cabeza");
	SharedMemory<int> numElem("/numElem");

	cola_t &c = buffer();
	int &i = cola();
	int &r = cabeza();
	int &n = numElem();

	int pid = getpid();
	int dato = -1;
	int pos = -1;
	for (int cont = 0; cont < 10; cont++) {
		cerr << "Consumidor " << pid  << ": Esperando que termine el productor" << endl;
		cerr.flush();
		semPC.Wait();
		cerr << "Consumidor " << pid  << ": Recibiendo dato" << endl;
		cerr.flush();
		semBuffer.Wait();
		pos = r;
		dato = c[r];
		r = (r+1)%tamCola;
		semBuffer.Signal();
		semCP.Signal();
		sleep(3);
		cout << "Consumidor " << pid  << ": El valor recibido en " << pos << " es " << dato << endl;
		cout.flush();
	}
	return 0;
}
