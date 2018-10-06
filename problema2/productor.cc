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

using std::cout;
using std::cerr;
using std::endl;

const int tamCola = 5;
typedef int cola_t[tamCola];

int main() {
	SimpleSemaphore semPC("/semPC",0);
	SimpleSemaphore semCP("/semCP",tamCola);
	SimpleSemaphore semBuffer("/semCola");
	SharedMemory<cola_t> buffer("/buffer");
	SharedMemory<int> cola("/cola");
	SharedMemory<int> cabeza("/cabeza");
	SharedMemory<int> numElem("/numElem");

	cola_t &c = buffer();
	int &i = cola();
	int &r = cabeza();
	int &n = numElem();

	int pid = getpid();
	for (int cont = 0; cont <10; cont++) {
		int pos = -1;
		sleep(3);
		cerr << "Productor " << pid <<": Esperando por buffer" << endl;
		cerr.flush();
		semCP.Wait();
		semBuffer.Wait();
		pos = i;
		c[i] = pid;
		i = (i+1)%tamCola;
		n++;
		semBuffer.Signal();
		cout << "Productor " << pid << ": Generó en  "  << pos << " el valor " << pid << endl;
		cout.flush();
		semPC.Signal();
	}
	return 0;
}
