#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include "../mutex.h"
#include "../thread.h"

class A : public Thread {
	public:
	Mutex Mute;
	Mutex Mute2;
		void * Main (void * arg) throw (std::runtime_error) {
			Mute.Lock();
			Mute2.Lock();
			std::cout << "Thread A locked the resource" << std::endl;
			sleep(5);
			Mute.Unlock();
			sleep(5);
			Mute2.Unlock();
		};
};

int main () {
	A a;
	a.Start();
	sleep(1);
	std::cout << "Now main thread is trying to lock the resource" << std::endl;
	a.Mute.Lock();
	std::cout << "Now the main thread is back" << std::endl;
	a.Mute2.Lock();
	std::cout << "Mute2 unlocked" << std::endl;
	return 0;
}

