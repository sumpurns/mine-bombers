#ifndef __THREAD_H__
#define __THREAD_H__

#include <stdexcept>
#include <pthread.h>

class Thread {
	protected:
		struct ArgWrapper {
			Thread * Self;
			void * Arg;
		};
	public:
		Thread ();
		virtual ~Thread ();

		void Start (void * arg = NULL) throw (std::runtime_error);
		virtual void * Main (void * arg) throw (std::runtime_error);
	protected:
		static void * FunctionWrapper (void * arg) throw (std::runtime_error);
		pthread_t PThread;
		ArgWrapper Args;
	private:
};

#endif
