// Dialog.c
// Implementing dialog utilities

#include "Common.h"

#include "Dialog.h"
#include "Localization.h"
#include "Resources.h"

static WORD             s_wFPointSize;
static WSTR             s_szFFaceName;

static PVOID            s_pBasePool;
static PVOID            s_pCur;
static LPDLGTEMPLATEW   s_pDtCur;

// Aligns a pointer to DWORD boundary.
#define AlignDword(p_) ((LPVOID) ((((ULONG_PTR) (p_) + 3) >> 2) << 2))

BOOL DlgInitialize(SIZE_T uSize) {
    NONCLIENTMETRICSW ncm;
    ncm.cbSize = sizeof(NONCLIENTMETRICSW);
    if (!SystemParametersInfoW(SPI_GETNONCLIENTMETRICS,
        sizeof(NONCLIENTMETRICSW), &ncm, 0))
    {
        return FALSE;
    }
    HDC hDc = GetDC(NULL);
    if (!hDc)
        return FALSE;
    s_wFPointSize = (WORD) MulDiv(-ncm.lfCaptionFont.lfHeight,
        72, GetDeviceCaps(hDc, LOGPIXELSY));
    ReleaseDC(NULL, hDc);
    if (FAILED(StringCchCopyW(s_szFFaceName, WSTR_MAXLEN,
        ncm.lfCaptionFont.lfFaceName)))
    {
        return FALSE;
    }
    PVOID pMem = MAlloc(uSize);
    if (!pMem)
        return FALSE;
    s_pCur = AlignDword(pMem);
    s_pBasePool = pMem;
    return TRUE;
}

void DlgShutdown(void) {
    MFree(s_pBasePool);
}

void DlgFillDt(PCWSTR pszCap, short x, short y, short cx, short cy) {
    LPDLGTEMPLATEW pDt = (PVOID) s_pCur;
    pDt->style = WS_CAPTION | WS_POPUP | WS_SYSMENU | WS_BORDER |
        DS_MODALFRAME | DS_SETFONT;
    pDt->dwExtendedStyle = 0;
    pDt->cdit = 0;
    pDt->x = x;
    pDt->y = y;
    pDt->cx = cx;
    pDt->cy = cy;
    PWORD pw = (PWORD) (pDt + 1);
    *pw++ = 0x0000;
    *pw++ = 0x0000;
    SIZE_T cch = (SIZE_T) lstrlenW(pszCap) + 1;
    StringCchCopyW(pw, cch, pszCap);
    pw += cch;
    *pw++ = s_wFPointSize;
    cch = (SIZE_T) lstrlenW(s_szFFaceName) + 1;
    StringCchCopyW(pw, cch, s_szFFaceName);
    pw += cch;
    s_pDtCur = pDt;
    s_pCur = AlignDword(pw);
}

void DlgFillDit(WORD wId, PCWSTR pszCap, WORD wCtl, DWORD dwStyle,
    short x, short y, short cx, short cy)
{
    ++s_pDtCur->cdit;
    LPDLGITEMTEMPLATEW pDit = (LPDLGITEMTEMPLATEW) s_pCur;
    pDit->style = WS_CHILD | WS_VISIBLE | dwStyle;
    pDit->dwExtendedStyle = 0;
    pDit->x = x;
    pDit->y = y;
    pDit->cx = cx;
    pDit->cy = cy;
    pDit->id = wId;
    PWORD pw = (PWORD) (pDit + 1);
    *pw++ = 0xffff;
    *pw++ = wCtl;
    SIZE_T cch = (SIZE_T) lstrlenW(pszCap) + 1;
    StringCchCopyW(pw, cch, pszCap);
    pw += cch;
    *pw++ = 0x0000;
    s_pCur = AlignDword(pw);
}

LPDLGTEMPLATEW DlgFinalize(void) {
    LPDLGTEMPLATEW pDt = s_pDtCur;
    s_pDtCur = s_pCur;
    return pDt;
}

BOOL DlgCreateDtFlt(LPDLGTEMPLATEW *ppDt) {
    DlgFillDt(IDS_DLGFLT_CAP, 0, 0, 174, 55);
    DlgStc(IDC_DLGFLT_SFLT, IDS_DLGFLT_FLT, 7, 7, 160, 8);
    DlgEdt(IDC_DLGFLT_EFLT, L"", 7, 19, 160, 8);
    DlgBtn(IDOK, IDS_DLG_BOK, TRUE, 27, 34, 44, 14);
    DlgBtn(IDCANCEL, IDS_DLG_BCA, FALSE, 75, 34, 44, 14);
    DlgBtn(IDABORT, IDS_DLG_BCL, FALSE, 123, 34, 44, 14);
    *ppDt = DlgFinalize();
    return TRUE;
}

BOOL DlgCreateDtPrm(LPDLGTEMPLATEW *ppDt) {
    DlgFillDt(L"", 0, 0, 134, 163);
    DlgStc(IDC_DLGPRM_SNUM, IDS_DLGPRM_NUM, 7, 7, 120, 8);
    DlgEdt(IDC_DLGPRM_ENUM, L"", 7, 19, 120, 8);
    DlgStc(IDC_DLGPRM_SNAM, IDS_DLGPRM_NAM, 7, 34, 120, 8);
    DlgEdt(IDC_DLGPRM_ENAM, L"", 7, 46, 120, 8);
    DlgStc(IDC_DLGPRM_SADD, IDS_DLGPRM_ADD, 7, 61, 120, 8);
    DlgEdt(IDC_DLGPRM_EADD, L"", 7, 73, 120, 8);
    DlgStc(IDC_DLGPRM_SDEV, IDS_DLGPRM_DEV, 7, 88, 120, 8);
    DlgEdt(IDC_DLGPRM_EDEV, L"", 7, 100, 120, 8);
    DlgStc(IDC_DLGPRM_SPMC, IDS_DLGPRM_PMC, 7, 115, 120, 8);
    DlgEdt(IDC_DLGPRM_EPMC, L"", 7, 127, 120, 8);
    DlgBtn(IDOK, IDS_DLG_BOK, TRUE, 35, 142, 44, 14);
    DlgBtn(IDCANCEL, IDS_DLG_BCA, FALSE, 83, 142, 44, 14);
    *ppDt = DlgFinalize();
    return TRUE;
}

BOOL DlgCreateDtBld(LPDLGTEMPLATEW *ppDt) {
    DlgFillDt(L"", 0, 0, 134, 136);
    DlgStc(IDC_DLGBLD_SNUM, IDS_DLGBLD_NUM, 7, 7, 120, 8);
    DlgEdt(IDC_DLGBLD_ENUM, L"", 7, 19, 120, 8);
    DlgStc(IDC_DLGBLD_SNAM, IDS_DLGBLD_NAM, 7, 34, 120, 8);
    DlgEdt(IDC_DLGBLD_ENAM, L"", 7, 46, 120, 8);
    DlgStc(IDC_DLGBLD_SUNT, IDS_DLGBLD_UNT, 7, 61, 120, 8);
    DlgEdt(IDC_DLGBLD_EUNT, L"", 7, 73, 120, 8);
    DlgStc(IDC_DLGBLD_SFLR, IDS_DLGBLD_FLR, 7, 88, 120, 8);
    DlgEdt(IDC_DLGBLD_EFLR, L"", 7, 100, 120, 8);
    DlgBtn(IDOK, IDS_DLG_BOK, TRUE, 35, 115, 44, 14);
    DlgBtn(IDCANCEL, IDS_DLG_BCA, FALSE, 83, 115, 44, 14);
    *ppDt = DlgFinalize();
    return TRUE;
}

BOOL DlgCreateDtRom(LPDLGTEMPLATEW *ppDt) {
    DlgFillDt(L"", 0, 0, 134, 136);
    DlgStc(IDC_DLGROM_SNUM, IDS_DLGROM_NUM, 7, 7, 120, 8);
    DlgEdt(IDC_DLGROM_ENUM, L"", 7, 19, 120, 8);
    DlgStc(IDC_DLGROM_STYP, IDS_DLGROM_TYP, 7, 34, 120, 8);
    DlgEdt(IDC_DLGROM_ETYP, L"", 7, 46, 120, 8);
    DlgStc(IDC_DLGROM_SARE, IDS_DLGROM_ARE, 7, 61, 120, 8);
    DlgEdt(IDC_DLGROM_EARE, L"", 7, 73, 120, 8);
    DlgStc(IDC_DLGROM_SPRI, IDS_DLGROM_PRI, 7, 88, 120, 8);
    DlgEdt(IDC_DLGROM_EPRI, L"", 7, 100, 120, 8);
    DlgBtn(IDOK, IDS_DLG_BOK, TRUE, 35, 115, 44, 14);
    DlgBtn(IDCANCEL, IDS_DLG_BCA, FALSE, 83, 115, 44, 14);
    *ppDt = DlgFinalize();
    return TRUE;
}
