// Object.c
// Implementing basic object operations

#include "Common.h"

#include "Object.h"
#include "Resources.h"

DWORD g_dwPrms;
DWORD g_dwBlds;
DWORD g_dwRoms;

PPREMISES   g_pPrmHead = NULL;
PBUILDING   g_pBldHead = NULL;
PROOM       g_pRomHead = NULL;

int ObjCmpNndbl(LPARAM pLhs, LPARAM pRhs, LPARAM lpar) {
    PCMPINFO pci = (PCMPINFO) lpar;
    int nRes = pci->nReverse;
    double lhs = *(double *) (pLhs + pci->uOffset);
    double rhs = *(double *) (pRhs + pci->uOffset);
    double dlt = lhs - rhs;
    return dlt > 1e-8 ? nRes * 1 : dlt < -1e-8 ? nRes * -1 : 0;
}

int ObjCmpDword(LPARAM pLhs, LPARAM pRhs, LPARAM lpar) {
    PCMPINFO pci = (PCMPINFO) lpar;
    DWORD lhs = *(PDWORD) (pLhs + pci->uOffset);
    DWORD rhs = *(PDWORD) (pRhs + pci->uOffset);
    return pci->nReverse * (int) (lhs - rhs);
}

int ObjCmpNtstr(LPARAM pLhs, LPARAM pRhs, LPARAM lpar) {
    PCMPINFO pci = (PCMPINFO) lpar;
    PCWSTR lhs = (PCWSTR) (pLhs + pci->uOffset);
    PCWSTR rhs = (PCWSTR) (pRhs + pci->uOffset);
    return pci->nReverse * lstrcmpW(lhs, rhs);
}

int ObjCmpDwLv2(LPARAM pLhs, LPARAM pRhs, LPARAM lpar) {
    PCMPINFO pci = (PCMPINFO) lpar;
    char *lhs1 = *(char **) (pLhs + pci->uOffset);
    char *rhs1 = *(char **) (pRhs + pci->uOffset);
    DWORD lhs = *(PDWORD) (lhs1 + pci->uOffset2);
    DWORD rhs = *(PDWORD) (rhs1 + pci->uOffset2);
    return pci->nReverse * (int) (lhs - rhs);
}

int ObjCmpDwLv3(LPARAM pLhs, LPARAM pRhs, LPARAM lpar) {
    PCMPINFO pci = (PCMPINFO) lpar;
    char *lhs1 = *(char **) (pLhs + pci->uOffset);
    char *rhs1 = *(char **) (pRhs + pci->uOffset);
    char *lhs2 = *(char **) (lhs1 + pci->uOffset2);
    char *rhs2 = *(char **) (rhs1 + pci->uOffset2);
    DWORD lhs = *(PDWORD) (lhs2 + pci->uOffset3);
    DWORD rhs = *(PDWORD) (rhs2 + pci->uOffset3);
    return pci->nReverse * (int) (lhs - rhs);
}

#define LlLnk(pl_, pr_) do {            \
    (pl_)->pNext = (pr_);               \
    (pr_)->pPrev = (pl_);               \
} while (FALSE)

#define LlIns(p_, pHint_) do {          \
    (p_)->pPrev = (pHint_)->pPrev;      \
    (p_)->pNext = (pHint_);             \
    (p_)->pPrev->pNext = (p_);          \
    (p_)->pNext->pPrev = (p_);          \
} while (FALSE)

#define LlRmv(p_) do {                  \
    (p_)->pPrev->pNext = (p_)->pNext;   \
    (p_)->pNext->pPrev = (p_)->pPrev;   \
} while (FALSE)

#define DEFNIL(name_, type_) static inline type_ *CONCAT(name_, Nil)(void) {\
    type_ *p = (type_ *) MAlloc(sizeof(type_));                             \
    if (!p)                                                                 \
        return NULL;                                                        \
    LlLnk(p, p);                                                            \
    return p;                                                               \
}

DEFNIL(Prm, PREMISES)
DEFNIL(Bld, BUILDING)
DEFNIL(Rom, ROOM)

static inline void GetNndbl(HANDLE hf, double *pd) {
    ReadFile(hf, pd, (DWORD) sizeof(double), NULL, NULL);
}

static inline void GetDword(HANDLE hf, PDWORD pdw) {
    ReadFile(hf, pdw, (DWORD) sizeof(DWORD), NULL, NULL);
}

static inline void GetNtstr(HANDLE hf, PWSTR psz) {
    DWORD dwLen;
    GetDword(hf, &dwLen);
    ReadFile(hf, psz, dwLen * (DWORD) sizeof(WCHAR), NULL, NULL);
    psz[dwLen] = L'\0';
}

static inline void PutNndbl(HANDLE hf, double d) {
    WriteFile(hf, &d, (DWORD) sizeof(double), NULL, NULL);
}

static inline void PutDword(HANDLE hf, DWORD dw) {
    WriteFile(hf, &dw, (DWORD) sizeof(DWORD), NULL, NULL);
}

static inline void PutNtstr(HANDLE hf, PCWSTR psz) {
    DWORD dwLen = (DWORD) lstrlenW(psz);
    PutDword(hf, dwLen);
    WriteFile(hf, psz, dwLen * (DWORD) sizeof(WCHAR), NULL, NULL);
}

static inline void FileTouch(PCWSTR pszFName) {
    HANDLE hf = CreateFileW(pszFName,
        GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    if (!hf)
        return;
    PutDword(hf, 0);
    CloseHandle(hf);
}

BOOL ObjInitialize(void) {
    FileTouch(IDS_FNAME_PRM);
    FileTouch(IDS_FNAME_BLD);
    FileTouch(IDS_FNAME_ROM);
    if (!(g_pPrmHead = PrmNil()))
        goto jFAllocPrm;
    if (!(g_pBldHead = BldNil()))
        goto jFAllocBld;
    if (!(g_pRomHead = RomNil()))
        goto jFAllocRom;
    return TRUE;

jFAllocRom:
    MFree(g_pBldHead);
jFAllocBld:
    MFree(g_pPrmHead);
jFAllocPrm:
    return FALSE;
}

void ObjShutdown(void) {
    for (PPREMISES p = g_pPrmHead->pNext; p != g_pPrmHead; ) {
        PVOID pTmp = p;
        p = p->pNext;
        MFree(pTmp);
    }
    MFree(g_pPrmHead);
    for (PBUILDING p = g_pBldHead->pNext; p != g_pBldHead; ) {
        PVOID pTmp = p;
        p = p->pNext;
        MFree(pTmp);
    }
    MFree(g_pBldHead);
    for (PROOM p = g_pRomHead->pNext; p != g_pRomHead; ) {
        PVOID pTmp = p;
        p = p->pNext;
        MFree(pTmp);
    }
    MFree(g_pRomHead);
}

#define DEFRMV(name_, tPtr_)                                            \
    static inline void CONCAT3(Rmv, name_, s)(void) {                   \
        tPtr_ p = CONCAT3(g_p, name_, Head)->pNext;                     \
        while (p != CONCAT3(g_p, name_, Head)) {                        \
            PVOID pTmp = p;                                             \
            p = p->pNext;                                               \
            MFree(pTmp);                                                \
        }                                                               \
        LlLnk(CONCAT3(g_p, name_, Head), CONCAT3(g_p, name_, Head));    \
        CONCAT3(g_dw, name_, s) = 0;                                    \
    }

DEFRMV(Prm, PPREMISES)
DEFRMV(Bld, PBUILDING)
DEFRMV(Rom, PROOM)

static inline BOOL LoadPrms(HANDLE hf) {
    GetDword(hf, &g_dwPrms);
    for (DWORD i = 0; i < g_dwPrms; ++i) {
        PPREMISES p = (PPREMISES) MAlloc(sizeof(PREMISES));
        GetDword(hf, &p->dwNum);
        GetNtstr(hf, p->szName);
        GetNtstr(hf, p->szAddress);
        GetNtstr(hf, p->szDevel);
        GetNtstr(hf, p->szPmc);
        p->dwBlds = 0;
        p->dwRoms = 0;
        LlIns(p, g_pPrmHead);
    }
    return TRUE;
}

static inline BOOL LoadBlds(HANDLE hf) {
    GetDword(hf, &g_dwBlds);
    for (DWORD i = 0; i < g_dwBlds; ++i) {
        PBUILDING p = (PBUILDING) MAlloc(sizeof(BUILDING));
        GetDword(hf, &p->dwNum);
        GetNtstr(hf, p->szName);
        GetDword(hf, &p->dwUnits);
        GetDword(hf, &p->dwFloors);
        p->dwRoms = 0;
        DWORD dwPNum;
        GetDword(hf, &dwPNum);
        PPREMISES pp = g_pPrmHead->pNext;
        while (pp != g_pPrmHead && pp->dwNum < dwPNum)
            pp = pp->pNext;
        if (pp != g_pPrmHead && pp->dwNum == dwPNum) {
            ++pp->dwBlds;
            p->pPrm = pp;
        }
        else {
            MFree(p);
            RmvBlds();
            return FALSE;
        }
        LlIns(p, g_pBldHead);
    }
    return TRUE;
}

static inline BOOL LoadRoms(HANDLE hf) {
    GetDword(hf, &g_dwRoms);
    for (DWORD i = 0; i < g_dwRoms; ++i) {
        PROOM p = (PROOM) MAlloc(sizeof(ROOM));
        GetDword(hf, &p->dwNum);
        GetNtstr(hf, p->szType);
        GetNndbl(hf, &p->dArea);
        GetNndbl(hf, &p->dShared);
        GetNndbl(hf, &p->dPrice);
        GetDword(hf, &p->dwUnit);
        GetDword(hf, &p->dwFloor);
        DWORD dwPNum;
        GetDword(hf, &dwPNum);
        PBUILDING pp = g_pBldHead->pNext;
        while (pp != g_pBldHead && pp->dwNum < dwPNum)
            pp = pp->pNext;
        if (pp != g_pBldHead && pp->dwNum == dwPNum) {
            ++pp->dwRoms;
            ++pp->pPrm->dwRoms;
            p->pBld = pp;
        }
        else {
            MFree(p);
            RmvRoms();
            return FALSE;
        }
        LlIns(p, g_pRomHead);
    }
    return TRUE;
}

BOOL ObjLoad(void) {
    HANDLE hFPrm = CreateFileW(IDS_FNAME_PRM,
        GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (!hFPrm)
        goto jFCfPrm;
    HANDLE hFBld = CreateFileW(IDS_FNAME_BLD,
        GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (!hFBld)
        goto jFCfBld;
    HANDLE hFRom = CreateFileW(IDS_FNAME_ROM,
        GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (!hFRom)
        goto jFCfRom;
    RmvPrms();
    RmvBlds();
    RmvRoms();
    if (!LoadPrms(hFPrm))
        goto jFLPrms;
    if (!LoadBlds(hFBld))
        goto jFLBlds;
    if (!LoadRoms(hFRom))
        goto jFLRoms;
    CloseHandle(hFRom);
    CloseHandle(hFBld);
    CloseHandle(hFPrm);
    return TRUE;

jFLRoms:
    RmvBlds();
jFLBlds:
    RmvPrms();
jFLPrms:
    CloseHandle(hFRom);
jFCfRom:
    CloseHandle(hFBld);
jFCfBld:
    CloseHandle(hFPrm);
jFCfPrm:
    return FALSE;
}

static inline void SavePrms(HANDLE hf) {
    PutDword(hf, g_dwPrms);
    PPREMISES p = g_pPrmHead->pNext;
    while (p != g_pPrmHead) {
        PutDword(hf, p->dwNum);
        PutNtstr(hf, p->szName);
        PutNtstr(hf, p->szAddress);
        PutNtstr(hf, p->szDevel);
        PutNtstr(hf, p->szPmc);
        p = p->pNext;
    }
}

static inline void SaveBlds(HANDLE hf) {
    PutDword(hf, g_dwBlds);
    PBUILDING p = g_pBldHead->pNext;
    while (p != g_pBldHead) {
        PutDword(hf, p->dwNum);
        PutNtstr(hf, p->szName);
        PutDword(hf, p->dwUnits);
        PutDword(hf, p->dwFloors);
        PutDword(hf, p->pPrm->dwNum);
        p = p->pNext;
    }
}

static inline void SaveRoms(HANDLE hf) {
    PutDword(hf, g_dwRoms);
    PROOM p = g_pRomHead->pNext;
    while (p != g_pRomHead) {
        PutDword(hf, p->dwNum);
        PutNtstr(hf, p->szType);
        PutNndbl(hf, p->dArea);
        PutNndbl(hf, p->dShared);
        PutNndbl(hf, p->dPrice);
        PutDword(hf, p->dwUnit);
        PutDword(hf, p->dwFloor);
        PutDword(hf, p->pBld->dwNum);
        p = p->pNext;
    }
}

BOOL ObjSave(void) {
    HANDLE hFPrm = CreateFileW(IDS_FNAME_PRM,
        GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (!hFPrm)
        goto jFCfPrm;
    HANDLE hFBld = CreateFileW(IDS_FNAME_BLD,
        GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (!hFBld)
        goto jFCfBld;
    HANDLE hFRom = CreateFileW(IDS_FNAME_ROM,
        GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (!hFRom)
        goto jFCfRom;
    SavePrms(hFPrm);
    SaveBlds(hFBld);
    SaveRoms(hFRom);
    CloseHandle(hFRom);
    CloseHandle(hFBld);
    CloseHandle(hFPrm);
    return TRUE;

jFCfRom:
    CloseHandle(hFBld);
jFCfBld:
    CloseHandle(hFPrm);
jFCfPrm:
    return FALSE;
}

#define DEFFINDHINT(name_, tPtr_) static inline tPtr_               \
    CONCAT(name_, FindHint)(DWORD dwNum) {                          \
        tPtr_ p = CONCAT3(g_p, name_, Head)->pNext;                 \
        while (p != CONCAT3(g_p, name_, Head) && p->dwNum < dwNum)  \
            p = p->pNext;                                           \
        if (p != CONCAT3(g_p, name_, Head) && p->dwNum == dwNum)    \
            return NULL;                                            \
        return p;                                                   \
    }

DEFFINDHINT(Prm, PPREMISES)
DEFFINDHINT(Bld, PBUILDING)
DEFFINDHINT(Rom, PROOM)

static inline void PrmFill(PPREMISES pPrm, DWORD dwNum, PCWSTR pszName,
    PCWSTR pszAddress, PCWSTR pszDevel, PCWSTR pszPmc)
{
    pPrm->dwNum = dwNum;
    StringCchCopyW(pPrm->szName, WSTR_MAXLEN, pszName);
    StringCchCopyW(pPrm->szAddress, WSTR_MAXLEN, pszAddress);
    StringCchCopyW(pPrm->szDevel, WSTR_MAXLEN, pszDevel);
    StringCchCopyW(pPrm->szPmc, WSTR_MAXLEN, pszPmc);
}

static inline void BldFill(PBUILDING pBld, DWORD dwNum, PCWSTR pszName,
    DWORD dwUnits, DWORD dwFloors)
{
    pBld->dwNum = dwNum;
    StringCchCopyW(pBld->szName, WSTR_MAXLEN, pszName);
    pBld->dwUnits = dwUnits;
    pBld->dwFloors = dwFloors;
}

static inline void RomFill(PROOM pRom, DWORD dwNum, PCWSTR pszType,
    double dArea, double dShared, double dPrice,
    DWORD dwUnit, DWORD dwFloor)
{
    pRom->dwNum = dwNum;
    StringCchCopyW(pRom->szType, WSTR_MAXLEN, pszType);
    pRom->dArea = dArea;
    pRom->dShared = dShared;
    pRom->dPrice = dPrice;
    pRom->dwUnit = dwUnit;
    pRom->dwFloor = dwFloor;
}

HRESULT ObjAddPrm(DWORD dwNum, PCWSTR pszName, PCWSTR pszAddress,
    PCWSTR pszDevel, PCWSTR pszPmc)
{
    PPREMISES pHint = PrmFindHint(dwNum);
    if (!pHint)
        return OBJ_E_NUM;
    PPREMISES pPrm = (PPREMISES) MAlloc(sizeof(PREMISES));
    if (!pPrm)
        return OBJ_E_MEM;
    PrmFill(pPrm, dwNum, pszName, pszAddress, pszDevel, pszPmc);
    pPrm->dwBlds = 0;
    pPrm->dwRoms = 0;
    LlIns(pPrm, pHint);
    ++g_dwPrms;
    return S_OK;
}

HRESULT ObjAddBld(DWORD dwNum, PCWSTR pszName, DWORD dwUnits,
    DWORD dwFloors, PPREMISES pPrm)
{
    PBUILDING pHint = BldFindHint(dwNum);
    if (!pHint)
        return OBJ_E_NUM;
    PBUILDING pBld = (PBUILDING) MAlloc(sizeof(BUILDING));
    if (!pBld)
        return OBJ_E_MEM;
    BldFill(pBld, dwNum, pszName, dwUnits, dwFloors);
    pBld->dwRoms = 0;
    pBld->pPrm = pPrm;
    LlIns(pBld, pHint);
    ++pPrm->dwBlds;
    ++g_dwBlds;
    return S_OK;
}

HRESULT ObjAddRom(DWORD dwNum, PCWSTR pszType, double dArea, double dShared,
    double dPrice, DWORD dwUnit, DWORD dwFloor, PBUILDING pBld)
{
    PROOM pHint = RomFindHint(dwNum);
    if (!pHint)
        return OBJ_E_NUM;
    PROOM pRom = (PROOM) MAlloc(sizeof(ROOM));
    if (!pRom)
        return OBJ_E_MEM;
    RomFill(pRom, dwNum, pszType, dArea, dShared, dPrice,
        dwUnit, dwFloor);
    pRom->pBld = pBld;
    LlIns(pRom, pHint);
    ++pBld->dwRoms;
    ++pBld->pPrm->dwRoms;
    ++g_dwRoms;
    return S_OK;
}

void ObjRmvPrm(PPREMISES pPrm) {
    for (PROOM p = g_pRomHead->pNext; p != g_pRomHead; )
        if (p->pBld->pPrm == pPrm) {
            LlRmv(p);
            PVOID pTmp = p;
            p = p->pNext;
            --g_dwRoms;
            MFree(pTmp);
        }
        else
            p = p->pNext;
    for (PBUILDING p = g_pBldHead->pNext; p != g_pBldHead; )
        if (p->pPrm == pPrm) {
            LlRmv(p);
            PVOID pTmp = p;
            p = p->pNext;
            --g_dwBlds;
            MFree(pTmp);
        }
        else
            p = p->pNext;
    LlRmv(pPrm);
    --g_dwPrms;
    MFree(pPrm);
}

void ObjRmvBld(PBUILDING pBld) {
    for (PROOM p = g_pRomHead->pNext; p != g_pRomHead; )
        if (p->pBld == pBld) {
            LlRmv(p);
            PVOID pTmp = p;
            p = p->pNext;
            MFree(pTmp);
            --g_dwRoms;
        }
        else
            p = p->pNext;
    LlRmv(pBld);
    --g_dwBlds;
    --pBld->pPrm->dwBlds;
    MFree(pBld);
}

void ObjRmvRom(PROOM pRom) {
    LlRmv(pRom);
    --g_dwRoms;
    --pRom->pBld->pPrm->dwRoms;
    --pRom->pBld->dwRoms;
    MFree(pRom);
}

HRESULT ObjModPrm(PPREMISES pPrm, DWORD dwNum, PCWSTR pszName,
    PCWSTR pszAddress, PCWSTR pszDevel, PCWSTR pszPmc)
{
    PPREMISES pSaved = pPrm->pNext;
    LlRmv(pPrm);
    PPREMISES pHint = PrmFindHint(dwNum);
    if (!pHint) {
        LlIns(pPrm, pSaved);
        return OBJ_E_NUM;
    }
    PrmFill(pPrm, dwNum, pszName, pszAddress, pszDevel, pszPmc);
    LlIns(pPrm, pHint);
    return S_OK;
}

HRESULT ObjModBld(PBUILDING pBld, DWORD dwNum, PCWSTR pszName,
    DWORD dwUnits, DWORD dwFloors)
{
    PBUILDING pSaved = pBld->pNext;
    LlRmv(pBld);
    PBUILDING pHint = BldFindHint(dwNum);
    if (!pHint) {
        LlIns(pBld, pSaved);
        return OBJ_E_NUM;
    }
    BldFill(pBld, dwNum, pszName, dwUnits, dwFloors);
    LlIns(pBld, pHint);
    return S_OK;
}

HRESULT ObjModRom(PROOM pRom, DWORD dwNum, PCWSTR pszType,
    double dArea, double dShared, double dPrice,
    DWORD dwUnit, DWORD dwFloor)
{
    PROOM pSaved = pRom->pNext;
    LlRmv(pRom);
    PROOM pHint = RomFindHint(dwNum);
    if (!pHint) {
        LlIns(pRom, pSaved);
        return OBJ_E_NUM;
    }
    RomFill(pRom, dwNum, pszType, dArea, dShared, dPrice,
        dwUnit, dwFloor);
    LlIns(pRom, pHint);
    return S_OK;
}
