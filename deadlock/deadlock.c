/*
* Created by: jmacid
* Project: thread2
* Created date: 24/05/2020
* Description: Thread with mutexes
* Resources:
	https://docs.microsoft.com/en-us/windows/win32/procthread/creating-threads
	https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createthread
	https://stackoverflow.com/questions/7841357/simple-multithreading-mutex-example-is-incorrect
	https://docs.microsoft.com/en-us/windows/win32/sync/using-mutex-objects
	https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/debugging-a-deadlock

	https://stackoverflow.com/questions/57768832/waitforsingleobject-do-not-lock-a-mutex-but-check-the-current-state
	Note: there is a subtle but important difference between a pthread mutex and a Win32 mutex. 
	A Win32 mutex is always recursive. When a thread already has a lock to a Win32 mutex, it can 
	safely relock the same mutex without blocking itself. An internal lock count is incremented each 
	time the mutex is relocked, and the thread simply needs to unlock the mutex as many times as it 
	re)locked in order to release the mutex for other threads to lock. A pthread mutex, on the other hand, 
	is recursive only if the mutex creator explicitly requests it when calling pthread_mutex_init(). So be 
	carefully with your (re)locking to avoid deadlocking your code.

	https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-createmutexa?redirectedfrom=MSDN
	The thread that owns a mutex can specify the same mutex in repeated wait function calls without blocking its execution.
*/

#include <stdio.h>
#include <windows.h>

#define MAX_THREADS 2
#define MAX_COUNT 20000000

typedef struct
{
	DWORDLONG valor;
	HANDLE mutex;
} globalVar_struct;

WORD function1(LPVOID lpParam);
WORD function2(LPVOID lpParam);

int main(void) {
	HANDLE  handleThreadArray[MAX_THREADS];
	WORD(*ptrFunctions[MAX_THREADS]) (LPVOID lpParam) = { function1, function2 };
	globalVar_struct val = { 0, CreateMutex(NULL, FALSE, NULL) };

	// Creo los threads
	for (INT i = 0; i < MAX_THREADS; i++) {
		handleThreadArray[i] = CreateThread(
			NULL,											// default security attributes
			0,												// use default stack size  
			(LPTHREAD_START_ROUTINE)ptrFunctions[i],        // thread function name
			&val,											// argument to thread function 
			0,												// use default creation flags 
			NULL											// returns the thread identifier 
		);

	}

	WaitForMultipleObjects(MAX_THREADS, handleThreadArray, TRUE, INFINITE);
	for (INT i = 0; i < MAX_THREADS; i++) {
		INT exitCode;
		GetExitCodeThread(handleThreadArray[i], &exitCode);
		printf("Exit code thread %d: %d\n", i, exitCode);
		CloseHandle(handleThreadArray[i]);
	}
	CloseHandle(val.mutex);

	printf("valor: %lld\n", val.valor);
	return 0;
}



WORD function1(LPVOID lpParam) {
	globalVar_struct* localVal = (globalVar_struct*)lpParam;
	DWORD dwWaitResult = 0;
	Sleep(5000);

	for (INT i = 0; i < MAX_COUNT; i++) {
		// lock of mutex
		dwWaitResult = WaitForSingleObject(localVal->mutex, INFINITE);
		//Sleep(2500);
		
		(localVal->valor)++;

		// unlock mutex
		//ReleaseMutex(localVal->mutex);
	}
	
	ExitThread(0);
}
WORD function2(LPVOID lpParam) {
	globalVar_struct* localVal = (globalVar_struct*)lpParam;
	DWORD dwWaitResult = 0;

	for (INT i = 0; i < MAX_COUNT; i++) {
		// lock of mutex
		dwWaitResult = WaitForSingleObject(localVal->mutex, INFINITE);
		if ( dwWaitResult ) {
			printf("dwWaitResult: %04X\n", dwWaitResult);
			if (dwWaitResult == WAIT_ABANDONED) printf("Abandoned Mutex by another thread\n");
		}
		(localVal->valor)++;

		// unlock mutex
		ReleaseMutex(localVal->mutex);
	}

	ExitThread(0);
}