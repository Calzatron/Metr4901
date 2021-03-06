// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <windows.h> 
#include <tchar.h>
#include <stdio.h> 
#include <strsafe.h>

#define BUFSIZE 4096 

HANDLE g_hChildStd_IN_Rd = NULL;
HANDLE g_hChildStd_IN_Wr = NULL;
HANDLE g_hChildStd_OUT_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;

HANDLE g_hInputFile = NULL;

void CreateChildProcess(void);
void WriteToPipe(void);
void ReadFromPipe(void);


int main(int argc, char* argv[])
{
	SECURITY_ATTRIBUTES saAttr;

	printf("\n->Start of parent execution.\n");

	// Set the bInheritHandle flag so pipe handles are inherited. 

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	// Create a pipe for the child process's STDOUT. 

	if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0)) {
		printf("StdoutRd CreatePipe");
		exit(3);
	}
	// Ensure the read handle to the pipe for STDOUT is not inherited.

	if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0)) {
		printf("Stdout SetHandleInformation"); exit(4);

		// Create a pipe for the child process's STDIN. 
	}
	if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0)) {
		printf("Stdin CreatePipe");
		exit(5);

		// Ensure the write handle to the pipe for STDIN is not inherited. 
	}
	if (!SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0)) {
		printf("Stdin SetHandleInformation");
		exit(6);
	}
	// Create the child process. 

	CreateChildProcess();

	// Get a handle to an input file for the parent. 
	// This example assumes a plain text file and uses string output to verify data flow. 

	//if (argc == 1) {
	//	printf("Please specify an input file.\n");
	//	exit(1);
	//}
	//TCHAR szCurrentDirectory[] = TEXT("C:/Users/Callum/Documents/2017/METR4901/programming/client/apples.txt");
	//g_hInputFile = CreateFile(
	//	szCurrentDirectory,
	//	GENERIC_READ,
	//	0,
	//	NULL,
	//	OPEN_EXISTING,
	//	FILE_ATTRIBUTE_READONLY,
	//	NULL);

	//if (g_hInputFile == INVALID_HANDLE_VALUE) {
	//	printf("INVALID_HANDLE_VALUE\n");
	//	exit(7);
	//}
	//else {
	//	printf("Opened file\n");
	//}
	// Write to the pipe that is the standard input for a child process. 
	// Data is written to the pipe's buffers, so it is not necessary to wait
	// until the child process is running before writing data.

	//WriteToPipe();
	//printf("\n->Contents of %s written to child STDIN pipe.\n", argv[1]);

	// Read from pipe that is the standard output for child process. 

	printf("\n->Contents of child process STDOUT: %s\n\n", argv[1]);
	ReadFromPipe();

	printf("\n->End of parent execution.\n");

	// The remaining open handles are cleaned up when this process terminates. 
	// To avoid resource leaks in a larger application, close handles explicitly. 

	return 0;
}

void CreateChildProcess()
// Create a child process that uses the previously created pipes for STDIN and STDOUT.
{
	//TCHAR szCmdline[] = TEXT("child");
	TCHAR szCmdline[] = TEXT("C:/Users/Callum/Documents/2017/METR4901/programming/client/SDL2-2.0.7/VisualC/Win32/Debug/testjoystick");
	TCHAR szCurrentDirectory[] = TEXT("");
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;
	BOOL bSuccess = FALSE;

	// Set up members of the PROCESS_INFORMATION structure. 

	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

	// Set up members of the STARTUPINFO structure. 
	// This structure specifies the STDIN and STDOUT handles for redirection.

	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdError = g_hChildStd_OUT_Wr;
	siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
	siStartInfo.hStdInput = g_hChildStd_IN_Rd;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	// Create the child process. 

	bSuccess = CreateProcess(NULL,
		szCmdline,     // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		0,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&piProcInfo);  // receives PROCESS_INFORMATION 

	// If an error occurs, exit the application. 
	if (!bSuccess) {
		printf("Creating Process not a success"); exit(8);
	} else {
		// Close handles to the child process and its primary thread.
		// Some applications might keep these handles to monitor the status
		// of the child process, for example. 

		CloseHandle(piProcInfo.hProcess);
		CloseHandle(piProcInfo.hThread);
	}
}

void WriteToPipe(void)

// Read from a file and write its contents to the pipe for the child's STDIN.
// Stop when there is no more data. 
{
	printf("in WriteToPipe\n");
	DWORD dwRead, dwWritten;
	CHAR chBuf[BUFSIZE];
	BOOL bSuccess = FALSE;


		bSuccess = ReadFile(g_hInputFile, chBuf, BUFSIZE, &dwRead, NULL);
		if (!bSuccess || dwRead == 0) { 
			if (!bSuccess) { printf("failed reading from file\n"); //break; 
			}
			printf("dwRead == 0? %d\n %s", dwRead, chBuf);
			//break;
		}

		bSuccess = WriteFile(g_hChildStd_IN_Wr, chBuf, dwRead, &dwWritten, NULL);
		if (!bSuccess) { printf("failed writing to child\n"); //break; 
		}


	// Close the pipe handle so the child process stops reading. 

		if (!CloseHandle(g_hChildStd_IN_Wr)) {
			printf("StdInWr CloseHandle"); exit(9);
		} 
		else { printf("wrote some thusgas\n"); }
		fflush(stdout);
}

void ReadFromPipe(void)

// Read output from the child process's pipe for STDOUT
// and write to the parent process's pipe for STDOUT. 
// Stop when there is no more data. 
{
	DWORD dwRead, dwWritten;
	CHAR chBuf[BUFSIZE];
	BOOL bSuccess = FALSE;
	HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	printf("we made it here\n"); fflush(stdout);
	for (;;)
	{	
		printf("checkion\n");
		bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
		//printf("asdf\n"); fflush(stdout);
		if (!bSuccess || dwRead == 0) { printf("failed reading from child\n"); break; }
		//else { printf("this is chBuf %s\n", chBuf); }
		//printf("aaa\n");

		bSuccess = WriteFile(hParentStdOut, chBuf,
			dwRead, &dwWritten, NULL);
		if (!bSuccess) { printf("failed writing to stdout\n"); break; }
	}
}


