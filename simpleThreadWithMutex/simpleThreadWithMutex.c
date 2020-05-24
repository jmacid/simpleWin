/*
* Created by: jmacid
* Project: thread2
* Created date: 24/05/2020
* Description: Thread with mutexes
* Resources:
	https://docs.microsoft.com/en-us/windows/win32/procthread/creating-threads
	https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createthread
	https://stackoverflow.com/questions/7841357/simple-multithreading-mutex-example-is-incorrect
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
	WORD (*ptrFunctions [MAX_THREADS]) (LPVOID lpParam) = { function1, function2 };
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
	printf("valor: %lld\n", val);
	return 0;
}



WORD function1(LPVOID lpParam) {
	globalVar_struct* localVal = (globalVar_struct*)lpParam;

	// lock of mutex
	WaitForSingleObject(localVal->mutex, INFINITE);

	for (INT i = 0; i < MAX_COUNT; i++) {
		(localVal->valor)++;
	}
	// unlock mutex
	ReleaseMutex(localVal->mutex);
	return 0;
}
WORD function2(LPVOID lpParam) {
	globalVar_struct* localVal = (globalVar_struct*)lpParam;

	// lock of mutex
	WaitForSingleObject(localVal->mutex, INFINITE);

	for (INT i = 0; i < MAX_COUNT; i++) {
		(localVal->valor)++;
	}
	// unlock mutex
	ReleaseMutex(localVal->mutex);
	return 0;
}