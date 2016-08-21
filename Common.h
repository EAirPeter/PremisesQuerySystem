// Common.h
// Common header

#ifndef PQS_COMMON_H_
#define PQS_COMMON_H_

// Ensures the program is compiled under Unicode environment.
#ifndef UNICODE
#   define UNICODE
#endif // ifndef UNICODE
#ifndef _UNICODE
#   define _UNICODE
#endif // ifndef _UNICODE

#pragma warning(disable:4706)

#pragma comment(linker,"\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <CommCtrl.h>
#include <Shlwapi.h>
#include <strsafe.h>
#include <windowsx.h>

#define WSTR_MAXLEN 256

typedef WCHAR WSTR[WSTR_MAXLEN];

extern HINSTANCE g_hInst;

#define CONCAT_(a_, b_) a_ ## b_
#define CONCAT(a_, b_) CONCAT_(a_, b_)
#define CONCAT3_(a_, b_, c_) a_ ## b_ ## c_
#define CONCAT3(a_, b_, c_) CONCAT3_(a_, b_, c_)
#define STR_(x_) L#x_
#define STR(x_) STR_(x_)

#define MAlloc(size_) \
    (HeapAlloc(GetProcessHeap(), 0, (SIZE_T) (size_)))
#define MFree(ptr_) \
    ((void) HeapFree(GetProcessHeap(), 0, (ptr_)))

#define StringClear(str_) ((void) (*(str_) = L'\0'))

// Converts a string representing a non-negative integer to dword.
BOOL StringToDword(PCWSTR psz, DWORD *pdw);

// Converts a string representing a non-negative real number to a double.
BOOL StringToNndbl(PCWSTR psz, double *pd);

#endif // ifndef PQS_COMMON_H_
