// Gui.c
// Implementing GUI operations.

#include "Common.h"

#include "Dialog.h"
#include "Filter.h"
#include "Gui.h"
#include "Localization.h"
#include "Object.h"
#include "Resources.h"

BOOL CreateMainMenu(HMENU *pMenu) {
#define MAddStr(wId_, pszDesc_) do {                            \
    if (!AppendMenuW(hMenuSub, MF_STRING, (wId_), (pszDesc_)))  \
        goto jFAppMenu;                                         \
} while (FALSE)
    HMENU hMenu, hMenuSub;
    if (!(hMenu = CreateMenu()))
        return FALSE;
    if (!(hMenuSub = CreateMenu()))
        goto jFCreateMenu;
    if (!AppendMenuW(hMenu, MF_POPUP, (UINT_PTR) hMenuSub, IDS_MENU_FILE))
        goto jFAppMainMenu;
    MAddStr(IDM_FILE_LOAD, IDS_MENU_FILE_LOAD);
    MAddStr(IDM_FILE_SAVE, IDS_MENU_FILE_SAVE);
    if (!AppendMenuW(hMenuSub, MF_SEPARATOR, 0, NULL))
        goto jFAppMenu;
    MAddStr(IDM_FILE_ABOUT, IDS_MENU_FILE_ABOUT);
    if (!AppendMenuW(hMenuSub, MF_SEPARATOR, 0, NULL))
        goto jFAppMenu;
    MAddStr(IDM_FILE_EXIT, IDS_MENU_FILE_EXIT);
    if (!(hMenuSub = CreateMenu()))
        goto jFCreateMenu;
    if (!AppendMenuW(hMenu, MF_POPUP, (UINT_PTR) hMenuSub, IDS_MENU_VIEW))
        goto jFAppMainMenu;
    MAddStr(IDM_VIEW_PRM, IDS_MENU_VIEW_PRM);
    MAddStr(IDM_VIEW_BLD, IDS_MENU_VIEW_BLD);
    MAddStr(IDM_VIEW_ROM, IDS_MENU_VIEW_ROM);
    MAddStr(IDM_VIEW_PARENT, IDS_MENU_VIEW_PARENT);
    if (!AppendMenuW(hMenuSub, MF_SEPARATOR, 0, NULL))
        goto jFAppMenu;
    MAddStr(IDM_VIEW_FILTER, IDS_MENU_VIEW_FILTER);
    *pMenu = hMenu;
    return TRUE;
#undef MAddStr
jFAppMainMenu:
    DestroyMenu(hMenuSub);
jFAppMenu:
jFCreateMenu:
    DestroyMenu(hMenu);
    return FALSE;
}

typedef void (*READDFUNC)(void);

typedef struct tagGUI_TYPE_ GUI_TYPE;
struct tagGUI_TYPE_ {
    GUI_STATE       gs;
    HWND            hWndLv;
    HMENU           hMenu;
    int             idxLastSort;
    READDFUNC       pfnReAdd;
    LPDLGTEMPLATEW  pDtInf;
};

static HWND             s_hWndSb;

static GUI_TYPE         s_gts[4];
static GUI_TYPE * const s_gtPrm = &s_gts[GS_PRM];
static GUI_TYPE * const s_gtBld = &s_gts[GS_BLD];
static GUI_TYPE * const s_gtRom = &s_gts[GS_ROM];
static GUI_TYPE *       s_gtCur;

static LPDLGTEMPLATEW   s_pDtFlt;
static WSTR             s_szFlt;

// Sets a timer to indicates when to set ready.
void ReadyTimer(HWND hWnd);

// Creates a list view.
BOOL CreateListView(HWND hWnd, HWND *pWndLv);

// Creates main status bar.
BOOL CreateStatusBar(HWND hWnd);

// Updates the status bar.
void UpdateSb(BOOL bFltVld);

// Initializations
BOOL InitPrm(HWND hWnd);
BOOL InitBld(HWND hWnd);
BOOL InitRom(HWND hWnd);

// Uninitializations
void UninitPrm(void);
void UninitBld(void);
void UninitRom(void);

// Creates the popup menu.
BOOL CreatePopupMenuPrm(void);
BOOL CreatePopupMenuBld(void);
BOOL CreatePopupMenuRom(void);

// Creates the list view.
BOOL CreateListViewPrm(HWND hWnd);
BOOL CreateListViewBld(HWND hWnd);
BOOL CreateListViewRom(HWND hWnd);

// Adds information to list view.
void AddPrms(void);
void AddBlds(void);
void AddRoms(void);

// Formats and returns the string with given arguments.
static inline PWSTR StrFmt(PCWSTR pszFmt, ...) {
    static WSTR szBuf;
    va_list vArgs;
    va_start(vArgs, pszFmt);
    StringCchVPrintfW(szBuf, WSTR_MAXLEN, pszFmt, vArgs);
    va_end(vArgs);
    return szBuf;
}

static VOID CALLBACK ReadyTimerProc(HWND hWnd, UINT msg,
    UINT_PTR id, DWORD dwTime)
{
    UNREFERENCED_PARAMETER(msg);
    UNREFERENCED_PARAMETER(id);
    UNREFERENCED_PARAMETER(dwTime);
    GuiReady(hWnd);
}

void SetReadyTimer(HWND hWnd) {
    SetTimer(hWnd, IDT_TOREADY, 2000, ReadyTimerProc);
}

BOOL CreateListView(HWND hWnd, HWND *pWndLv) {
    INITCOMMONCONTROLSEX iccx;
    iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
    iccx.dwICC = ICC_LISTVIEW_CLASSES;
    if (!InitCommonControlsEx(&iccx))
        return FALSE;
    RECT rcClient;
    GetClientRect(hWnd, &rcClient);
    HWND hWndLv = CreateWindowExW(
        0,
        WC_LISTVIEWW,
        NULL,
        WS_CHILD | WS_BORDER | WS_HSCROLL | WS_VSCROLL |
            LVS_REPORT | LVS_SINGLESEL,
        0, 0, 0, 0,
        hWnd,
        NULL,
        g_hInst,
        NULL
    );
    DWORD dwStyle = ListView_GetExtendedListViewStyle(hWndLv);
    ListView_SetExtendedListViewStyle(hWndLv, dwStyle |
        LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    if (!hWndLv)
        return FALSE;
    *pWndLv = hWndLv;
    return TRUE;
}

BOOL CreateStatusBar(HWND hWnd) {
    INITCOMMONCONTROLSEX iccx;
    iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
    iccx.dwICC = ICC_BAR_CLASSES;
    if (!InitCommonControlsEx(&iccx))
        return FALSE;
    HWND hWndStatus = CreateWindowExW(
        0,
        STATUSCLASSNAMEW,
        NULL,
        WS_CHILD | WS_BORDER | WS_VISIBLE,
        0, 0, 0, 0,
        hWnd,
        NULL,
        g_hInst,
        NULL
    );
    if (!hWndStatus)
        return FALSE;
    int pos[N_STATUSBAR] = {120, 480, 600, 720, 840};
    SendMessageW(hWndStatus, SB_SETPARTS, (WPARAM) N_STATUSBAR, (LPARAM) &pos);
    s_hWndSb = hWndStatus;
    return TRUE;
}

#define SbSetText(part_, text_) ((BOOL) SendMessageW(s_hWndSb, \
    SB_SETTEXTW, MAKEWORD((part_), 0), (LPARAM) (text_)))

void UpdateSb(BOOL bFltVld) {
    ULONG c = (ULONG) ListView_GetItemCount(s_gtCur->hWndLv);
    if (!bFltVld)
        SbSetText(IDX_SB_FLT, IDS_SB_FLT_INV);
    else if (*s_szFlt)
        SbSetText(IDX_SB_FLT, StrFmt(IDS_SB_FLT_FLT, c, s_szFlt));
    else
        SbSetText(IDX_SB_FLT, StrFmt(IDS_SB_FLT_ALL, c));
    SbSetText(IDX_SB_PRM, StrFmt(IDS_SB_PRM, (ULONG) g_dwPrms));
    SbSetText(IDX_SB_BLD, StrFmt(IDS_SB_BLD, (ULONG) g_dwBlds));
    SbSetText(IDX_SB_ROM, StrFmt(IDS_SB_ROM, (ULONG) g_dwRoms));
}

void GuiLoad(HWND hWnd) {
    if (ObjLoad()) {
        SbSetText(IDX_SB_STA, IDS_SB_STA_LOADED);
        SetReadyTimer(hWnd);
        s_gtCur->pfnReAdd();
    }
    else {
        GuiReady(hWnd);
        MbError(IDS_MSGE_LOAD, hWnd);
    }
 }

void GuiSave(HWND hWnd) {
    if (ObjSave()) {
        SbSetText(IDX_SB_STA, IDS_SB_STA_SAVED);
        SetReadyTimer(hWnd);
    }
    else {
        GuiReady(hWnd);
        MbError(IDS_MSGE_SAVE, hWnd);
    }
}

void GuiReady(HWND hWnd) {
    KillTimer(hWnd, IDT_TOREADY);
    SbSetText(IDX_SB_STA, IDS_SB_STA_READY);
}

#define DEFINIT(name_, nameu_) BOOL CONCAT(Init, name_)(HWND hWnd) {    \
    CONCAT(s_gt, name_)->gs = CONCAT(GS_, nameu_);                      \
    if (!CONCAT(CreateListView, name_)(hWnd))                           \
        goto jFCLv;                                                     \
    if (!CONCAT(CreatePopupMenu, name_)())                              \
        goto jFCM;                                                      \
    CONCAT(s_gt, name_)->idxLastSort = 1;                               \
    CONCAT(s_gt, name_)->pfnReAdd = CONCAT3(Add, name_, s);             \
    if (!CONCAT(DlgCreateDt, name_)(&CONCAT(s_gt, name_)->pDtInf))      \
        goto jFCDt;                                                     \
    return TRUE;                                                        \
jFCDt:                                                                  \
    DestroyMenu(CONCAT(s_gt, name_)->hMenu);                            \
jFCM:                                                                   \
    DestroyWindow(CONCAT(s_gt, name_)->hWndLv);                         \
jFCLv:                                                                  \
    return FALSE;                                                       \
}
DEFINIT(Prm, PRM)
DEFINIT(Bld, BLD)
DEFINIT(Rom, ROM)
#undef DEFINIT

#define DEFUNINIT(name_) void CONCAT(Uninit, name_)(void) { \
    DestroyMenu(CONCAT(s_gt, name_)->hMenu);                \
    DestroyWindow(CONCAT(s_gt, name_)->hWndLv);             \
}
DEFUNINIT(Prm)
DEFUNINIT(Bld)
DEFUNINIT(Rom)
#undef DEFUNINIT

#define MAddStr(wId_, pszDesc_)                             \
    if (!AppendMenuW(hMenu, MF_STRING, (wId_), (pszDesc_))) \
        goto jFAppMenu;

BOOL CreatePopupMenuPrm(void) {
    HMENU hMenu = CreatePopupMenu();
    if (!hMenu)
        return FALSE;
    MAddStr(IDM_POP_ADD, IDS_MPRM_ADD)
    MAddStr(IDM_POP_MOD, IDS_MPRM_MOD)
    MAddStr(IDM_POP_RMV, IDS_MPRM_RMV)
    MAddStr(IDM_POP_SUB, IDS_MPRM_SUB)
    if (!AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL))
        goto jFAppMenu;
    MAddStr(IDM_POP_NEW, IDS_MPRM_NEW)
    s_gtPrm->hMenu = hMenu;
    return TRUE;

jFAppMenu:
    DestroyMenu(hMenu);
    return FALSE;
}

BOOL CreatePopupMenuBld(void) {
    HMENU hMenu = CreatePopupMenu();
    if (!hMenu)
        return FALSE;
    MAddStr(IDM_POP_ADD, IDS_MBLD_ADD)
    MAddStr(IDM_POP_MOD, IDS_MBLD_MOD)
    MAddStr(IDM_POP_RMV, IDS_MBLD_RMV)
    MAddStr(IDM_POP_SUB, IDS_MBLD_SUB)
    if (!AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL))
        goto jFAppMenu;
    MAddStr(IDM_POP_PAR, IDS_MBLD_PAR)
    s_gtBld->hMenu = hMenu;
    return TRUE;

jFAppMenu:
    DestroyMenu(hMenu);
    return FALSE;
}

BOOL CreatePopupMenuRom(void) {
    HMENU hMenu = CreatePopupMenu();
    if (!hMenu)
        return FALSE;
    MAddStr(IDM_POP_MOD, IDS_MROM_MOD)
    MAddStr(IDM_POP_RMV, IDS_MROM_RMV)
    if (!AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL))
        goto jFAppMenu;
    MAddStr(IDM_POP_PAR, IDS_MROM_PAR)
    MAddStr(IDM_POP_ANC, IDS_MROM_ANC)
    s_gtRom->hMenu = hMenu;
    return TRUE;

jFAppMenu:
    DestroyMenu(hMenu);
    return FALSE;
}

#define LvInsCol(iCol_, cx_, pszText_) do {                     \
    if (FAILED(StringCchCopyW(szText, WSTR_MAXLEN, (pszText_))))\
        goto jFInsCol;                                          \
    lvc.cx = (cx_);                                             \
    lvc.iSubItem = (iCol_);                                     \
    if (ListView_InsertColumn(hWndLv, (iCol_), (&lvc)) == -1)   \
        goto jFInsCol;                                          \
} while (FALSE)

BOOL CreateListViewPrm(HWND hWnd) {
    HWND hWndLv;
    if (!CreateListView(hWnd, &hWndLv))
        return FALSE;
    WSTR szText;
    LVCOLUMNW lvc;
    lvc.mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH;
    lvc.pszText = szText;
    LvInsCol(0, 80, IDS_LVPRM_NUM);
    LvInsCol(1, 240, IDS_LVPRM_NAM);
    LvInsCol(2, 240, IDS_LVPRM_ADD);
    LvInsCol(3, 240, IDS_LVPRM_DEV);
    LvInsCol(4, 240, IDS_LVPRM_PMC);
    LvInsCol(5, 80, IDS_LVPRM_BLD);
    LvInsCol(6, 80, IDS_LVPRM_ROM);
    s_gtPrm->hWndLv = hWndLv;
    return TRUE;

jFInsCol:
    DestroyWindow(hWndLv);
    return FALSE;
}

BOOL CreateListViewBld(HWND hWnd) {
    HWND hWndLv;
    if (!CreateListView(hWnd, &hWndLv))
        return FALSE;
    WSTR szText;
    LVCOLUMNW lvc;
    lvc.mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH;;
    lvc.pszText = szText;
    LvInsCol(0, 80, IDS_LVBLD_NUM);
    LvInsCol(1, 240, IDS_LVBLD_NAM);
    LvInsCol(2, 80, IDS_LVBLD_UNT);
    LvInsCol(3, 80, IDS_LVBLD_FLR);
    LvInsCol(4, 80, IDS_LVBLD_ROM);
    LvInsCol(5, 240, IDS_LVBLD_PRM);
    s_gtBld->hWndLv = hWndLv;
    return TRUE;

jFInsCol:
    DestroyWindow(hWndLv);
    return FALSE;
}

BOOL CreateListViewRom(HWND hWnd) {
    HWND hWndLv;
    if (!CreateListView(hWnd, &hWndLv))
        return FALSE;
    WSTR szText;
    LVCOLUMNW lvc;
    lvc.mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH;;
    lvc.pszText = szText;
    LvInsCol(0, 80, IDS_LVROM_NUM);
    LvInsCol(1, 80, IDS_LVROM_TYP);
    LvInsCol(2, 80, IDS_LVROM_ARE);
    LvInsCol(3, 80, IDS_LVROM_SHR);
    LvInsCol(4, 80, IDS_LVROM_PRI);
    LvInsCol(5, 80, IDS_LVROM_UNT);
    LvInsCol(6, 80, IDS_LVROM_FLR);
    LvInsCol(7, 240, IDS_LVROM_BLD);
    LvInsCol(8, 240, IDS_LVROM_PRM);
    s_gtRom->hWndLv = hWndLv;
    return TRUE;

jFInsCol:
    DestroyWindow(hWndLv);
    return FALSE;
}

#undef LvInsCol

void AddPrms(void) {
    HWND hWnd = s_gtPrm->hWndLv;
    ListView_DeleteAllItems(hWnd);
    StrTrimW(s_szFlt, L" \t");
    if (!FltPrepare(s_szFlt)) {
        UpdateSb(FALSE);
        return;
    }
    LVITEMW lvi;
    lvi.mask = LVIF_PARAM;
    lvi.iSubItem = 0;
    int cItem = 0;
    for (PPREMISES p = g_pPrmHead->pNext; p != g_pPrmHead; p = p->pNext) {
        if (!FltPrm(p))
            continue;
        lvi.iItem = cItem;
        lvi.lParam = (LPARAM) p;
        ListView_InsertItem(hWnd, &lvi);
        ListView_SetItemText(hWnd, cItem, 0,
            StrFmt(L"%lu", (ULONG) p->dwNum));
        ListView_SetItemText(hWnd, cItem, 1, p->szName);
        ListView_SetItemText(hWnd, cItem, 2, p->szAddress);
        ListView_SetItemText(hWnd, cItem, 3, p->szDevel);
        ListView_SetItemText(hWnd, cItem, 4, p->szPmc);
        ListView_SetItemText(hWnd, cItem, 5,
            StrFmt(L"%lu", (ULONG) p->dwBlds));
        ListView_SetItemText(hWnd, cItem, 6,
            StrFmt(L"%lu", (ULONG) p->dwRoms));
        ++cItem;
    }
    UpdateSb(TRUE);
}

void AddBlds(void) {
    HWND hWnd = s_gtBld->hWndLv;
    ListView_DeleteAllItems(hWnd);
    StrTrimW(s_szFlt, L" \t");
    if (!FltPrepare(s_szFlt)) {
        UpdateSb(FALSE);
        return;
    }
    LVITEMW lvi;
    lvi.mask = LVIF_PARAM;
    lvi.iSubItem = 0;
    int cItem = 0;
    for (PBUILDING p = g_pBldHead->pNext; p != g_pBldHead; p = p->pNext) {
        if (!FltBld(p))
            continue;
        lvi.iItem = cItem;
        lvi.lParam = (LPARAM) p;
        ListView_InsertItem(hWnd, &lvi);
        ListView_SetItemText(hWnd, cItem, 0,
            StrFmt(L"%lu", (ULONG) p->dwNum));
        ListView_SetItemText(hWnd, cItem, 1, p->szName);
        ListView_SetItemText(hWnd, cItem, 2,
            StrFmt(L"%lu", (ULONG) p->dwUnits));
        ListView_SetItemText(hWnd, cItem, 3,
            StrFmt(L"%lu", (ULONG) p->dwFloors));
        ListView_SetItemText(hWnd, cItem, 4,
            StrFmt(L"%lu", (ULONG) p->dwRoms));
        ListView_SetItemText(hWnd, cItem, 5,
            StrFmt(L"(%lu)%s", (ULONG) p->pPrm->dwNum, p->pPrm->szName));
        ++cItem;
    }
    UpdateSb(TRUE);
}

void AddRoms(void) {
    HWND hWnd = s_gtRom->hWndLv;
    ListView_DeleteAllItems(hWnd);
    StrTrimW(s_szFlt, L" \t");
    if (!FltPrepare(s_szFlt)) {
        UpdateSb(FALSE);
        return;
    }
    LVITEMW lvi;
    lvi.mask = LVIF_PARAM;
    lvi.iSubItem = 0;
    int cItem = 0;
    for (PROOM p = g_pRomHead->pNext; p != g_pRomHead; p = p->pNext) {
        if (!FltRom(p))
            continue;
        lvi.iItem = cItem;
        lvi.lParam = (LPARAM) p;
        ListView_InsertItem(hWnd, &lvi);
        ListView_SetItemText(hWnd, cItem, 0,
            StrFmt(L"%lu", (ULONG) p->dwNum));
        ListView_SetItemText(hWnd, cItem, 1, p->szType);
        ListView_SetItemText(hWnd, cItem, 2,
            StrFmt(L"%.2f", p->dArea));
        ListView_SetItemText(hWnd, cItem, 3,
            StrFmt(L"%.2f", p->dShared));
        ListView_SetItemText(hWnd, cItem, 4,
            StrFmt(L"%.2f", p->dPrice));
        ListView_SetItemText(hWnd, cItem, 5,
            StrFmt(L"%lu", (ULONG) p->dwUnit));
        ListView_SetItemText(hWnd, cItem, 6,
            StrFmt(L"%lu", (ULONG) p->dwFloor));
        ListView_SetItemText(hWnd, cItem, 7, StrFmt(L"(%lu)%s",
            (ULONG) p->pBld->dwNum, p->pBld->szName));
        ListView_SetItemText(hWnd, cItem, 8, StrFmt(L"(%lu)%s",
            (ULONG) p->pBld->pPrm->dwNum, p->pBld->pPrm->szName));
        ++cItem;
    }
    UpdateSb(TRUE);
}

BOOL GuiInitialize(HWND hWnd) {
    if (!CreateStatusBar(hWnd))
        goto jFCSb;
    if (!ObjInitialize())
        goto jFInitObj;
    if (!DlgInitialize(2048))
        goto jFInitDlg;
    if (!DlgCreateDtFlt(&s_pDtFlt))
        goto jFCDtFlt;
    if (!InitPrm(hWnd))
        goto jFInitPrm;
    if (!InitBld(hWnd))
        goto jFInitBld;
    if (!InitRom(hWnd))
        goto jFInitRom;
    s_gtCur = &s_gts[GS_INV];
    GuiSwitch(hWnd, GS_PRM, NULL);
    return TRUE;

jFInitRom:
    UninitBld();
jFInitBld:
    UninitPrm();
jFInitPrm:
jFCDtFlt:
    DlgShutdown();
jFInitDlg:
    ObjShutdown();
jFInitObj:
    DestroyWindow(s_hWndSb);
jFCSb:
    return FALSE;
}

void GuiShutdown(void) {
    UninitRom();
    UninitBld();
    UninitPrm();
    DlgShutdown();
    ObjShutdown();
    FltClear();
}

void GuiSwitch(HWND hWnd, GUI_STATE gs, PCWSTR pszFlt) {
    if (gs == GS_INV || &s_gts[gs] == s_gtCur)
        return;
    switch (gs) {
    case GS_PRM:
        CheckMenuRadioItem(GetMenu(hWnd),
            IDM_VIEW_PRM, IDM_VIEW_ROM, IDM_VIEW_PRM, MF_BYCOMMAND);
        ShowWindow(s_gtPrm->hWndLv, SW_SHOW);
        ShowWindow(s_gtBld->hWndLv, SW_HIDE);
        ShowWindow(s_gtRom->hWndLv, SW_HIDE);
        break;
    case GS_BLD:
        CheckMenuRadioItem(GetMenu(hWnd),
            IDM_VIEW_PRM, IDM_VIEW_ROM, IDM_VIEW_BLD, MF_BYCOMMAND);
        ShowWindow(s_gtPrm->hWndLv, SW_HIDE);
        ShowWindow(s_gtBld->hWndLv, SW_SHOW);
        ShowWindow(s_gtRom->hWndLv, SW_HIDE);
        break;
    case GS_ROM:
        CheckMenuRadioItem(GetMenu(hWnd),
            IDM_VIEW_PRM, IDM_VIEW_ROM, IDM_VIEW_ROM, MF_BYCOMMAND);
        ShowWindow(s_gtPrm->hWndLv, SW_HIDE);
        ShowWindow(s_gtBld->hWndLv, SW_HIDE);
        ShowWindow(s_gtRom->hWndLv, SW_SHOW);
        break;
    }
    s_gtCur = &s_gts[gs];
    StringCchCopyW(s_szFlt, WSTR_MAXLEN, pszFlt ? pszFlt : L"");
    s_gtCur->pfnReAdd();
}

static int s_idx;

static inline LPARAM CurItem(void) {
    LVITEMW lvi;
    lvi.mask = LVIF_PARAM;
    lvi.iItem = s_idx;
    lvi.iSubItem = 0;
    ListView_GetItem(s_gtCur->hWndLv, &lvi);
    return lvi.lParam;
}

void GuiOnSize(HWND hWnd) {
    SendMessageW(s_hWndSb, WM_SIZE, 0, 0);
    RECT rcMain, rcStatus;
    GetClientRect(hWnd, &rcMain);
    GetWindowRect(s_hWndSb, &rcStatus);
    int w = rcMain.right - rcMain.left - 8;
    int h = (rcMain.bottom - rcMain.top) -
        (rcStatus.bottom - rcStatus.top) - 16;
    MoveWindow(s_gtPrm->hWndLv, 4, 8, w, h, TRUE);
    MoveWindow(s_gtBld->hWndLv, 4, 8, w, h, TRUE);
    MoveWindow(s_gtRom->hWndLv, 4, 8, w, h, TRUE);
}

// On notify: LVN_COLUMNCLICK - Sorts the items.
void OnNmLvColumnClick(HWND hWnd, LPNMHDR pHdr);

// On notify: NM_RCLICK - Shows the popup menu.
void OnNmRclick(HWND hWnd, LPNMHDR pHdr);

// On notify: NM_DBLCLK - Calls OnCmdPmSublist.
void OnNmDblclk(HWND hWnd, LPNMHDR pHdr);

// On commands
void OnCmdViewParent(HWND hWnd);
void OnCmdViewFilter(HWND hWnd);
void OnCmdPmAdd(HWND hWnd);
void OnCmdPmModify(HWND hWnd);
void OnCmdPmRemove(HWND hWnd);
void OnCmdPmSublist(HWND hWnd);
void OnCmdPmParent(HWND hWnd);
void OnCmdPmAncient(HWND hWnd);
void OnCmdPmNew(HWND hWnd);

// Dilag procedures
INT_PTR CALLBACK DlgFilterProc(HWND hWndDlg, UINT msg,
    WPARAM wpar, LPARAM lpar);
INT_PTR CALLBACK DlgPrmProc(HWND hWndDlg, UINT msg,
    WPARAM wpar, LPARAM lpar);
INT_PTR CALLBACK DlgBldProc(HWND hWndDlg, UINT msg,
    WPARAM wpar, LPARAM lpar);
INT_PTR CALLBACK DlgRomProc(HWND hWndDlg, UINT msg,
    WPARAM wpar, LPARAM lpar);

typedef struct tagGUIDLG_STATE_ GUIDLG_STATE;
typedef GUIDLG_STATE *          PGUIDLG_STATE;

// Provides the dialog type and the related LPARAM.
struct tagGUIDLG_STATE_ {
    BOOL        bIsAdd;
    LPARAM      lpar;
};

void GuiOnNotify(HWND hWnd, LPNMHDR pHdr) {
    if (pHdr->hwndFrom != s_gtCur->hWndLv)
        return;
    switch (pHdr->code) {
    case LVN_COLUMNCLICK:
        OnNmLvColumnClick(hWnd, pHdr);
        break;
    case NM_RCLICK:
        OnNmRclick(hWnd, pHdr);
        break;
    case NM_DBLCLK:
        OnNmDblclk(hWnd, pHdr);
        break;
    }
}

#define LvSort(cmp_, struct_, member_) do {                             \
    ci.uOffset = offsetof(struct_, member_);                            \
    ListView_SortItems(s_gtCur->hWndLv, (cmp_), (LPARAM) &ci);          \
} while (FALSE);

void OnNmLvColumnClick(HWND hWnd, LPNMHDR pHdr) {
    UNREFERENCED_PARAMETER(hWnd);
    int idx = ((LPNMLISTVIEW) pHdr)->iSubItem + 1;
    CMPINFO ci = {1, 0};
    if (idx == s_gtCur->idxLastSort) {
        s_gtCur->idxLastSort = -idx;
        ci.nReverse = -1;
    }
    else
        s_gtCur->idxLastSort = idx;
    switch (s_gtCur->gs) {
    case GS_PRM:
        switch (idx) {
        case 1: // dwNum
            LvSort(ObjCmpDword, PREMISES, dwNum);
            break;
        case 2: // szName
            LvSort(ObjCmpNtstr, PREMISES, szName);
            break;
        case 3: // szAddress
            LvSort(ObjCmpNtstr, PREMISES, szAddress);
            break;
        case 4: // szDevel
            LvSort(ObjCmpNtstr, PREMISES, szDevel);
            break;
        case 5: // szPmc
            LvSort(ObjCmpNtstr, PREMISES, szPmc);
            break;
        case 6: // dwBlds
            LvSort(ObjCmpDword, PREMISES, dwBlds);
            break;
        case 7: // dwRoms
            LvSort(ObjCmpDword, PREMISES, dwRoms);
            break;
        }
        break;
    case GS_BLD:
        switch (idx) {
        case 1: // dwNum
            LvSort(ObjCmpDword, BUILDING, dwNum);
            break;
        case 2: // szName
            LvSort(ObjCmpNtstr, BUILDING, szName);
            break;
        case 3: // dwUnits
            LvSort(ObjCmpDword, BUILDING, dwUnits);
            break;
        case 4: // dwFloors
            LvSort(ObjCmpDword, BUILDING, dwFloors);
            break;
        case 5: // dwRoms
            LvSort(ObjCmpDword, BUILDING, dwRoms);
            break;
        case 6: // pPrm->dwNum
            ci.uOffset2 = offsetof(PREMISES, dwNum);
            LvSort(ObjCmpDwLv2, BUILDING, pPrm);
            break;
        }
        break;
    case GS_ROM:
        switch (idx) {
        case 1: // dwNum
            LvSort(ObjCmpDword, ROOM, dwNum);
            break;
        case 2: // szType
            LvSort(ObjCmpNtstr, ROOM, szType);
            break;
        case 3: // dArea
            LvSort(ObjCmpNndbl, ROOM, dArea);
            break;
        case 4: // dShared
            LvSort(ObjCmpNndbl, ROOM, dShared);
            break;
        case 5: // dPrice
            LvSort(ObjCmpNndbl, ROOM, dPrice);
            break;
        case 6: // dwUnit
            LvSort(ObjCmpDword, ROOM, dwUnit);
            break;
        case 7: // dwFloor
            LvSort(ObjCmpDword, ROOM, dwFloor);
            break;
        case 8: // pBld->dwNum
            ci.uOffset2 = offsetof(BUILDING, dwNum);
            LvSort(ObjCmpDwLv2, ROOM, pBld);
            break;
        case 9: // pBld->pPrm->dwNum
            ci.uOffset3 = offsetof(PREMISES, dwNum);
            ci.uOffset2 = offsetof(BUILDING, pPrm);
            LvSort(ObjCmpDwLv3, ROOM, pBld);
            break;
        }
        break;
    }
}

static inline BOOL HitItem(LPNMITEMACTIVATE pNmia) {
    HWND hWndLv = s_gtCur->hWndLv;
    LVHITTESTINFO lvhti;
    lvhti.pt = pNmia->ptAction;
    ListView_SubItemHitTest(hWndLv, &lvhti);
    if (lvhti.flags & LVHT_ONITEM) {
        s_idx = lvhti.iItem;
        return TRUE;
    }
    return FALSE;
}

void OnNmRclick(HWND hWnd, LPNMHDR pHdr) {
    HWND hWndLv = s_gtCur->hWndLv;
    LPNMITEMACTIVATE pNmia = (LPNMITEMACTIVATE) pHdr;
    UINT uEnable = MF_BYCOMMAND | (HitItem(pNmia) ? MF_ENABLED : MF_DISABLED);
    HMENU hMenu = s_gtCur->hMenu;
    EnableMenuItem(hMenu, IDM_POP_ADD, uEnable);
    EnableMenuItem(hMenu, IDM_POP_MOD, uEnable);
    EnableMenuItem(hMenu, IDM_POP_RMV, uEnable);
    EnableMenuItem(hMenu, IDM_POP_SUB, uEnable);
    EnableMenuItem(hMenu, IDM_POP_PAR, uEnable);
    EnableMenuItem(hMenu, IDM_POP_ANC, uEnable);
    ClientToScreen(hWndLv, &pNmia->ptAction);
    TrackPopupMenuEx(
        hMenu,
        TPM_LEFTALIGN | TPM_TOPALIGN,
        pNmia->ptAction.x,
        pNmia->ptAction.y,
        hWnd,
        NULL
    );
}

void OnNmDblclk(HWND hWnd, LPNMHDR pHdr) {
    if (HitItem((LPNMITEMACTIVATE) pHdr))
        OnCmdPmSublist(hWnd);
}

BOOL GuiOnCommand(HWND hWnd, WORD wId) {
    switch (wId) {
    case IDM_VIEW_PRM:
        GuiSwitch(hWnd, GS_PRM, NULL);
        break;
    case IDM_VIEW_BLD:
        GuiSwitch(hWnd, GS_BLD, NULL);
        break;
    case IDM_VIEW_ROM:
        GuiSwitch(hWnd, GS_ROM, NULL);
        break;
    case IDM_VIEW_PARENT:
        OnCmdViewParent(hWnd);
        return TRUE;
    case IDM_VIEW_FILTER:
        OnCmdViewFilter(hWnd);
        return TRUE;
    case IDM_POP_ADD:
        OnCmdPmAdd(hWnd);
        return TRUE;
    case IDM_POP_MOD:
        OnCmdPmModify(hWnd);
        return TRUE;
    case IDM_POP_RMV:
        OnCmdPmRemove(hWnd);
        return TRUE;
    case IDM_POP_SUB:
        OnCmdPmSublist(hWnd);
        return TRUE;
    case IDM_POP_PAR:
        OnCmdPmParent(hWnd);
        return TRUE;
    case IDM_POP_ANC:
        OnCmdPmAncient(hWnd);
        return TRUE;
    case IDM_POP_NEW:
        OnCmdPmNew(hWnd);
        return TRUE;
    }
    return FALSE;
}

void OnCmdViewParent(HWND hWnd) {
    switch (s_gtCur->gs) {
    case GS_PRM:
        return;
    case GS_BLD:
        GuiSwitch(hWnd, GS_PRM, NULL);
        break;
    case GS_ROM:
        GuiSwitch(hWnd, GS_BLD, NULL);
        break;
    }
}

void OnCmdViewFilter(HWND hWnd) {
    DlgShow(s_pDtFlt, DlgFilterProc, NULL);
}

void OnCmdPmAdd(HWND hWnd) {
    GUIDLG_STATE gds;
    gds.bIsAdd = TRUE;
    switch (s_gtCur->gs) {
    case GS_PRM:
        gds.lpar = CurItem();
        DlgShow(s_gtBld->pDtInf, DlgBldProc, &gds);
        break;
    case GS_BLD:
        gds.lpar = CurItem();
        DlgShow(s_gtRom->pDtInf, DlgRomProc, &gds);
        break;
    case GS_ROM:
        return;
    }
}

void OnCmdPmModify(HWND hWnd) {
    GUIDLG_STATE gds;
    gds.bIsAdd = FALSE;
    gds.lpar = CurItem();
    switch (s_gtCur->gs) {
    case GS_PRM:
        DlgShow(s_gtPrm->pDtInf, DlgPrmProc, &gds);
        break;
    case GS_BLD:
        DlgShow(s_gtBld->pDtInf, DlgBldProc, &gds);
        break;
    case GS_ROM:
        DlgShow(s_gtRom->pDtInf, DlgRomProc, &gds);
        break;
    }
}

void OnCmdPmRemove(HWND hWnd) {
    PCWSTR pszQuestion = NULL;
    switch (s_gtCur->gs) {
    case GS_PRM:
        pszQuestion = IDS_MSGQ_PRMRMV;
        break;
    case GS_BLD:
        pszQuestion = IDS_MSGQ_BLDRMV;
        break;
    case GS_ROM:
        pszQuestion = IDS_MSGQ_ROMRMV;
        break;
    }
    if (MbQuestion(pszQuestion, hWnd) != IDYES)
        return;
    switch (s_gtCur->gs) {
    case GS_PRM:
        ObjRmvPrm((PPREMISES) CurItem());
        AddPrms();
        break;
    case GS_BLD:
        ObjRmvBld((PBUILDING) CurItem());
        AddBlds();
        break;
    case GS_ROM:
        ObjRmvRom((PROOM) CurItem());
        AddRoms();
        break;
    }
}

void OnCmdPmSublist(HWND hWnd) {
    LPARAM lpar = CurItem();
    ULONG id;
    switch (s_gtCur->gs) {
    case GS_PRM:
        id = (ULONG) ((PPREMISES) lpar)->dwNum;
        GuiSwitch(hWnd, GS_BLD, StrFmt(L"^\"+&%lu\"", id));
        break;
    case GS_BLD:
        id = (ULONG) ((PBUILDING) lpar)->dwNum;
        GuiSwitch(hWnd, GS_ROM, StrFmt(L"^\"+&%lu\"", id));
        break;
    case GS_ROM:
        return;
    }
}

void OnCmdPmParent(HWND hWnd) {
    LPARAM lpar = CurItem();
    LVFINDINFOW lvfi;
    lvfi.flags = LVFI_PARAM;
    switch (s_gtCur->gs) {
    case GS_PRM:
        return;
    case GS_BLD:
        GuiSwitch(hWnd, GS_PRM, NULL);
        lvfi.lParam = (LPARAM) ((PBUILDING) lpar)->pPrm;
        break;
    case GS_ROM:
        GuiSwitch(hWnd, GS_BLD, NULL);
        lvfi.lParam = (LPARAM) ((PROOM) lpar)->pBld;
        break;
    }
    int n = ListView_FindItem(s_gtCur->hWndLv, -1, &lvfi);
    if (n != -1) {
        ListView_EnsureVisible(s_gtCur->hWndLv, n, FALSE);
        ListView_SetItemState(s_gtCur->hWndLv, n,
            LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
    }
}

void OnCmdPmAncient(HWND hWnd) {
    LPARAM lpar = CurItem();
    LVFINDINFOW lvfi;
    lvfi.flags = LVFI_PARAM;
    switch (s_gtCur->gs) {
    case GS_PRM:
        return;
    case GS_BLD:
        return;
    case GS_ROM:
        GuiSwitch(hWnd, GS_PRM, NULL);
        lvfi.lParam = (LPARAM) ((PROOM) lpar)->pBld->pPrm;
        break;
    }
    int n = ListView_FindItem(s_gtCur->hWndLv, -1, &lvfi);
    if (n != -1) {
        ListView_EnsureVisible(s_gtCur->hWndLv, n, FALSE);
        ListView_SetItemState(s_gtCur->hWndLv, n,
            LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
    }
}

void OnCmdPmNew(HWND hWnd) {
    GUIDLG_STATE gds;
    gds.bIsAdd = TRUE;
    switch (s_gtCur->gs) {
    case GS_PRM:
        gds.lpar = CurItem();
        DlgShow(s_gtPrm->pDtInf, DlgPrmProc, &gds);
        break;
    case GS_BLD:
        return;
    case GS_ROM:
        return;
    }
}

#define DlgTextSet(id_, str_)   \
    (SetWindowTextW(GetDlgItem(hWndDlg, (id_)), (str_)))
#define DlgTextGet(id_, buf_)   \
    (GetWindowTextW(GetDlgItem(hWndDlg, (id_)), (buf_), WSTR_MAXLEN))
#define DlgTextLen(id_)         \
    (GetWindowTextLengthW(GetDlgItem(hWndDlg, (id_))))

#define DiAnyStr(id_, what_, buf_) do {             \
    if (DlgTextLen((id_)) >= WSTR_MAXLEN) {         \
        MbError(LSG_MSGE_STRLEN(what_), hWndDlg);   \
        return TRUE;                                \
    }                                               \
    DlgTextGet((id_), (buf_));                      \
} while (FALSE)
#define DiString(id_, what_, buf_) do {             \
    if (!DlgTextLen((id_))) {                       \
        MbError(LSG_MSGE_EMPTY(what_), hWndDlg);    \
        return TRUE;                                \
    }                                               \
    if (DlgTextLen((id_)) >= WSTR_MAXLEN) {         \
        MbError(LSG_MSGE_STRLEN(what_), hWndDlg);   \
        return TRUE;                                \
    }                                               \
    DlgTextGet((id_), (buf_));                      \
} while (FALSE)
#define DiDword(id_, what_, dw_) do {               \
    DiString((id_), what_, szTmp);                  \
    if (!StringToDword(szTmp, &(dw_))) {            \
        MbError(LSG_MSGE_DWORD(what_), hWndDlg);    \
        return TRUE;                                \
    }                                               \
} while (FALSE)
#define DiNndbl(id_, what_, d_) do {                \
    DiString((id_), what_, szTmp);                  \
    if (!StringToNndbl(szTmp, &(d_))) {             \
        MbError(LSG_MSGE_NNDBL(what_), hWndDlg);    \
        return TRUE;                                \
    }                                               \
} while (FALSE)

INT_PTR CALLBACK DlgFilterProc(HWND hWndDlg, UINT msg,
    WPARAM wpar, LPARAM lpar)
{
    UNREFERENCED_PARAMETER(lpar);
    switch (msg) {
    case WM_INITDIALOG:
        DlgTextSet(IDC_DLGFLT_EFLT, s_szFlt);
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wpar)) {
        case IDOK:
            DiAnyStr(IDC_DLGFLT_EFLT, IDS_DLGFLT_TFLT, s_szFlt);
            s_gtCur->pfnReAdd();
            EndDialog(hWndDlg, IDOK);
            return TRUE;
        case IDCANCEL:
            EndDialog(hWndDlg, IDCANCEL);
            return TRUE;
        case IDABORT:
            StringClear(s_szFlt);
            s_gtCur->pfnReAdd();
            EndDialog(hWndDlg, IDABORT);
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

INT_PTR CALLBACK DlgPrmProc(HWND hWndDlg, UINT msg,
    WPARAM wpar, LPARAM lpar)
{
    static PGUIDLG_STATE pGds = NULL;
    WSTR szTmp;
    DWORD dwNum;
    WSTR szName;
    WSTR szAddress;
    WSTR szDevel;
    WSTR szPmc;
    HRESULT hr;
    switch (msg) {
    case WM_INITDIALOG:
        pGds = (PGUIDLG_STATE) lpar;
        if (pGds->bIsAdd)
            SetWindowTextW(hWndDlg, IDS_DLGPRM_CAPADD);
        else {
            SetWindowTextW(hWndDlg, IDS_DLGPRM_CAPMOD);
            PPREMISES pPrm = (PPREMISES) pGds->lpar;
            DlgTextSet(IDC_DLGPRM_ENUM, StrFmt(L"%lu", (ULONG) pPrm->dwNum));
            DlgTextSet(IDC_DLGPRM_ENAM, pPrm->szName);
            DlgTextSet(IDC_DLGPRM_EADD, pPrm->szAddress);
            DlgTextSet(IDC_DLGPRM_EDEV, pPrm->szDevel);
            DlgTextSet(IDC_DLGPRM_EPMC, pPrm->szPmc);
        }
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wpar)) {
        case IDOK:
            DiDword(IDC_DLGPRM_ENUM, IDS_DLGPRM_TNUM, dwNum);
            DiString(IDC_DLGPRM_ENAM, IDS_DLGPRM_TNAM, szName);
            DiString(IDC_DLGPRM_EADD, IDS_DLGPRM_TADD, szAddress);
            DiString(IDC_DLGPRM_EDEV, IDS_DLGPRM_TDEV, szDevel);
            DiString(IDC_DLGPRM_EPMC, IDS_DLGPRM_TPMC, szPmc);
            hr = pGds->bIsAdd ?
                ObjAddPrm(dwNum, szName, szAddress, szDevel, szPmc) :
                ObjModPrm((PPREMISES) pGds->lpar, dwNum, szName, szAddress,
                szDevel, szPmc);
            switch (hr) {
            case S_OK:
                s_gtCur->pfnReAdd();
                EndDialog(hWndDlg, IDOK);
                break;
            case OBJ_E_NUM:
                MbError(LSG_MSGE_UNIQUE(IDS_DLGPRM_TNUM), hWndDlg);
                break;
            case OBJ_E_MEM:
                MbError(IDS_MSGE_MEMORY, hWndDlg);
                break;
            }
            return TRUE;
        case IDCANCEL:
            EndDialog(hWndDlg, IDCANCEL);
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

INT_PTR CALLBACK DlgBldProc(HWND hWndDlg, UINT msg,
    WPARAM wpar, LPARAM lpar)
{
    static PGUIDLG_STATE pGds = NULL;
    WSTR szTmp;
    DWORD dwNum;
    WSTR szName;
    DWORD dwUnits;
    DWORD dwFloors;
    HRESULT hr;
    switch (msg) {
    case WM_INITDIALOG:
        pGds = (PGUIDLG_STATE) lpar;
        if (pGds->bIsAdd)
            SetWindowTextW(hWndDlg, IDS_DLGBLD_CAPADD);
        else {
            SetWindowTextW(hWndDlg, IDS_DLGBLD_CAPMOD);
            PBUILDING pBld = (PBUILDING) pGds->lpar;
            DlgTextSet(IDC_DLGBLD_ENUM, StrFmt(L"%lu", (ULONG) pBld->dwNum));
            DlgTextSet(IDC_DLGBLD_ENAM, pBld->szName);
            DlgTextSet(IDC_DLGBLD_EUNT, StrFmt(L"%lu", (ULONG) pBld->dwUnits));
            DlgTextSet(IDC_DLGBLD_EFLR, StrFmt(L"%lu", (ULONG) pBld->dwFloors));
        }
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wpar)) {
        case IDOK:
            DiDword(IDC_DLGBLD_ENUM, IDS_DLGBLD_TNUM, dwNum);
            DiString(IDC_DLGBLD_ENAM, IDS_DLGBLD_TNAM, szName);
            DiDword(IDC_DLGBLD_EUNT, IDS_DLGBLD_TUNT, dwUnits);
            DiDword(IDC_DLGBLD_EFLR, IDS_DLGBLD_TFLR, dwFloors);
            hr = pGds->bIsAdd ? ObjAddBld(dwNum, szName, dwUnits, dwFloors,
                (PPREMISES) pGds->lpar) : ObjModBld((PBUILDING) pGds->lpar,
                dwNum, szName, dwUnits, dwFloors);
            switch (hr) {
            case S_OK:
                s_gtCur->pfnReAdd();
                EndDialog(hWndDlg, IDOK);
                break;
            case OBJ_E_NUM:
                MbError(LSG_MSGE_UNIQUE(IDS_DLGBLD_TNUM), hWndDlg);
                break;
            case OBJ_E_MEM:
                MbError(IDS_MSGE_MEMORY, hWndDlg);
                break;
            }
            return TRUE;
        case IDCANCEL:
            EndDialog(hWndDlg, IDCANCEL);
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

INT_PTR CALLBACK DlgRomProc(HWND hWndDlg, UINT msg,
    WPARAM wpar, LPARAM lpar)
{
    static PGUIDLG_STATE pGds = NULL;
    WSTR szTmp;
    DWORD dwNum;
    WSTR szType;
    double dArea;
    double dShared;
    double dPrice;
    DWORD dwUnit;
    DWORD dwFloor;
    HRESULT hr;
    switch (msg) {
    case WM_INITDIALOG:
        pGds = (PGUIDLG_STATE) lpar;
        if (pGds->bIsAdd)
            SetWindowTextW(hWndDlg, IDS_DLGROM_CAPADD);
        else {
            SetWindowTextW(hWndDlg, IDS_DLGROM_CAPMOD);
            PROOM pRom = (PROOM) pGds->lpar;
            DlgTextSet(IDC_DLGROM_ENUM, StrFmt(L"%lu", (ULONG) pRom->dwNum));
            DlgTextSet(IDC_DLGROM_ETYP, pRom->szType);
            DlgTextSet(IDC_DLGROM_EARE, StrFmt(L"%.2f", pRom->dArea));
            DlgTextSet(IDC_DLGROM_ESHR, StrFmt(L"%.2f", pRom->dShared));
            DlgTextSet(IDC_DLGROM_EPRI, StrFmt(L"%.2f", pRom->dPrice));
            DlgTextSet(IDC_DLGROM_EUNT, StrFmt(L"%lu", (ULONG) pRom->dwUnit));
            DlgTextSet(IDC_DLGROM_EFLR, StrFmt(L"%lu", (ULONG) pRom->dwFloor));
        }
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wpar)) {
        case IDOK:
            DiDword(IDC_DLGROM_ENUM, IDS_DLGROM_TNUM, dwNum);
            DiString(IDC_DLGROM_ETYP, IDS_DLGROM_TTYP, szType);
            DiNndbl(IDC_DLGROM_EARE, IDS_DLGROM_TARE, dArea);
            DiNndbl(IDC_DLGROM_ESHR, IDS_DLGROM_TSHR, dShared);
            DiNndbl(IDC_DLGROM_EPRI, IDS_DLGROM_TPRI, dPrice);
            DiDword(IDC_DLGROM_EUNT, IDS_DLGROM_TUNT, dwUnit);
            DiDword(IDC_DLGROM_EFLR, IDS_DLGROM_TFLR, dwFloor);
            hr = pGds->bIsAdd ?
                ObjAddRom(dwNum, szType, dArea, dShared, dPrice,
                dwUnit, dwFloor, (PBUILDING) pGds->lpar) :
                ObjModRom((PROOM) pGds->lpar, dwNum, szType, dArea, dShared,
                dPrice, dwUnit, dwFloor);

            switch (hr) {
            case S_OK:
                s_gtCur->pfnReAdd();
                EndDialog(hWndDlg, IDOK);
                break;
            case OBJ_E_NUM:
                MbError(LSG_MSGE_UNIQUE(IDS_DLGROM_TNUM), hWndDlg);
                break;
            case OBJ_E_MEM:
                MbError(IDS_MSGE_MEMORY, hWndDlg);
                break;
            }
            break;
        case IDCANCEL:
            EndDialog(hWndDlg, IDCANCEL);
            break;
        }
        return TRUE;
    }
    return FALSE;
}
