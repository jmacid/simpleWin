/*
* Created by: jmacid
* Project: simpleThread
* Created date: 21/03/2020
* Description: 
* Resources:
	https://docs.microsoft.com/en-us/windows/win32/procthread/creating-threads
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define MAX_COUNT 200000000
#define MAX_THREADS 2

long long sum = 0;

DWORD WINAPI  counting_function(LPVOID lpParam) {
	int val = (int)lpParam;
	for (int i = 0; i < MAX_COUNT; i++) {
		// Starts critic section
		sum += val;
		// End critic section
	}

}


int main(void) {
	int offset = 1;

	// threads 
	HANDLE  handle_thread_array[MAX_THREADS];
	DWORD   dwThreadIdArray[MAX_THREADS];

	for (int i = 0; i < MAX_THREADS; i++) {
		handle_thread_array[i] = CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size  
			counting_function,       // thread function name
			offset,          // argument to thread function 
			0,                      // use default creation flags 
			&dwThreadIdArray[i]);   // returns the thread identifier 
	}

	
	//counting_function(offset);
	WaitForMultipleObjects(MAX_THREADS, handle_thread_array, TRUE, INFINITE);
	printf("Sum: %lld\n", sum);

	return 0;
}