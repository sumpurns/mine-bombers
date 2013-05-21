#include "thread.h"

#include <iostream>

Thread::Thread () {
}

Thread::~Thread () {
}

void Thread::Start (void * arg) throw (std::runtime_error) {
	Args.Arg = arg;
	Args.Self = this;
	pthread_create(&PThread, NULL, FunctionWrapper, &Args);
}

void * Thread::FunctionWrapper (void * arg) throw (std::runtime_error) {
	ArgWrapper * args = reinterpret_cast<ArgWrapper*>(arg);
	return args->Self->Main(args->Arg);
}

void * Thread::Main (void * arg) throw (std::runtime_error) {
	std::cout << "New thread started" << std::endl;
	return NULL;
}
