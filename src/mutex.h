#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <sys/sem.h>
#include <stdexcept>

class Mutex {
	private:
		int SemId;
		sembuf SemBuf[1];
		unsigned short SemVals[1];

		union SemNum {
			int Val;
			semid_ds * Buf;
			unsigned short * Array;
		} SemArg;

		bool IsInit;

	public:

		Mutex () throw (std::runtime_error) {
			IsInit = false;
			SemId = semget(IPC_PRIVATE, 1, 0600 | IPC_CREAT | IPC_EXCL);
			if (SemId == -1) {
				throw std::runtime_error("Can't create a semaphore object");
			}
			IsInit = true;
			SemVals[0] = 1;
			SemArg.Array = SemVals;
			if (semctl(SemId, 0, SETALL, SemArg) == -1) {
				throw std::runtime_error("Error: semctl(); can't initialize semaphore");
			}
			SemBuf[0].sem_flg = SEM_UNDO;
			SemBuf[0].sem_num = 0;

		};

		virtual ~Mutex () {
			if (IsInit) {
				semctl(SemId, 1, IPC_RMID, SemArg);
			}
		};

		void Lock () throw (std::runtime_error) {
			if (!IsInit) {
				throw std::runtime_error("Can't lock non-existing mutex");
			}
			SemBuf[0].sem_op = -1;
			semop(SemId, SemBuf, 1);
		};

		void Unlock () throw (std::runtime_error) {
			if (!IsInit) {
				throw std::runtime_error("Can't unlock non-existing mutex");
			}
			SemBuf[0].sem_op = 1;
			semop(SemId, SemBuf, 1);
		};
};

#endif
