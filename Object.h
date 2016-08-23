// Object.h
// Basic types: premises, building, room

#ifndef PQS_OBJECTS_H_
#define PQS_OBJECTS_H_

#include "Common.h"

extern DWORD g_dwPrms;
extern DWORD g_dwBlds;
extern DWORD g_dwRoms;

typedef struct tagPREMISES_ PREMISES;
typedef PREMISES *          PPREMISES;
typedef const PREMISES *    PCPREMISES;
typedef struct tagBUILDING_ BUILDING;
typedef BUILDING *          PBUILDING;
typedef const BUILDING *    PCBUILDING;
typedef struct tagROOM_     ROOM;
typedef ROOM *              PROOM;
typedef const ROOM *        PCROOM;
typedef struct tagCMPINFO_  CMPINFO;
typedef CMPINFO *           PCMPINFO;
typedef const CMPINFO *     CPCMPINFO;

extern PPREMISES g_pPrmHead;
extern PBUILDING g_pBldHead;
extern PROOM     g_pRomHead;

struct tagPREMISES_ {
    // Linked List
    PPREMISES   pPrev;
    PPREMISES   pNext;
    // Premises - Information
    DWORD       dwNum;      // The number
    WSTR        szName;     // The name
    WSTR        szAddress;  // The address
    WSTR        szDevel;    // The developer
    WSTR        szPmc;      // The property management company
    // Premises - Structural
    DWORD       dwBlds;     // The total buildings
    DWORD       dwRoms;     // The total rooms
};

struct tagBUILDING_ {
    // Linked List
    PBUILDING   pPrev;
    PBUILDING   pNext;
    // Building - Information
    DWORD       dwNum;      // The number
    WSTR        szName;     // The name
    DWORD       dwUnits;    // The total units
    DWORD       dwFloors;   // The total floors
    // Building - Structural
    DWORD       dwRoms;     // The total rooms
    PPREMISES   pPrm;       // The parent premises
};

struct tagROOM_ {
    // Linked List
    PROOM       pPrev;
    PROOM       pNext;
    // Room - Information
    DWORD       dwNum;      // The number
    WSTR        szType;     // The type
    double      dArea;      // The area
    double      dPrice;     // The price
    // Room - Structural
    PBUILDING   pBld;       // The parent premises
};

struct tagCMPINFO_ {
    int         nReverse;
    SIZE_T      uOffset;
    SIZE_T      uOffset2;
    SIZE_T      uOffset3;
};

// Comparators
int CALLBACK ObjCmpNndbl(LPARAM pLhs, LPARAM pRhs, LPARAM lpar);
int CALLBACK ObjCmpDword(LPARAM pLhs, LPARAM pRhs, LPARAM lpar);
int CALLBACK ObjCmpNtstr(LPARAM pLhs, LPARAM pRhs, LPARAM lpar);
int CALLBACK ObjCmpDwLv2(LPARAM pLhs, LPARAM pRhs, LPARAM lpar);
int CALLBACK ObjCmpDwLv3(LPARAM pLhs, LPARAM pRhs, LPARAM lpar);

// Return values other than S_OK for ObjAdd functions and ObjMod functions
enum OBJECTS_HRESULT_ERRORS_ {
    OBJ_E_NUM = -0x0001,
    OBJ_E_MEM = -0x0002,
};

// Initialization
BOOL ObjInitialize(void);

// Uninitialization
void ObjShutdown(void);

// Loads data from disk.
BOOL ObjLoad(void);

// Saves data to disk.
BOOL ObjSave(void);

// Adds a premises.
HRESULT ObjAddPrm(DWORD dwNum, PCWSTR pszName, PCWSTR pszAddress,
    PCWSTR pszDevel, PCWSTR pszPmc);

// Adds a buildling.
HRESULT ObjAddBld(DWORD dwNum, PCWSTR pszName, DWORD dwUnits,
    DWORD dwFloors, PPREMISES pPrm);

// Adds a room.
HRESULT ObjAddRom(DWORD dwNum, PCWSTR pszType, double dArea,
    double dPrice, PBUILDING pBld);

// Removes a premises.
void ObjRmvPrm(PPREMISES pPrm);

// Removes a building.
void ObjRmvBld(PBUILDING pBld);

// Removes a room.
void ObjRmvRom(PROOM pRom);

// Modifies a premises.
HRESULT ObjModPrm(PPREMISES pPrm, DWORD dwNum, PCWSTR pszName,
    PCWSTR pszAddress, PCWSTR pszDevel, PCWSTR pszPmc);

// Modifies a building.
HRESULT ObjModBld(PBUILDING pBld, DWORD dwNum, PCWSTR pszName,
    DWORD dwUnits, DWORD dwFloors);

// Modifies a room.
HRESULT ObjModRom(PROOM pRom, DWORD dwNum, PCWSTR pszType,
    double dArea, double dPrice);

#endif // ifndef PQS_OBJECTS_H_
