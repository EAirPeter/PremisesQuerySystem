// Dialog.h
// Dialog utilities

#ifndef PQS_DIALOG_H_
#define PQS_DIALOG_H_

#include "Common.h"

// Initialization
BOOL DlgInitialize(SIZE_T uSize);

// Uninitialization
void DlgShutdown(void);

// Fills the current dialog template.
void DlgFillDt(PCWSTR pszCap, short x, short y, short cx, short cy);

// Fills a dialog item template and adds to the current dialog template.
void DlgFillDit(WORD wId, PCWSTR pszCap, WORD wCtl, DWORD dwStyle,
    short x, short y, short cx, short cy);

// Gets the current dialog template.
LPDLGTEMPLATEW DlgFinalize(void);

// Adds a button to the current dialog template.
#define DlgBtn(wId_, pszCap_, bDefault_, x_, y_, cx_, cy_)              \
    (DlgFillDit((wId_), (pszCap_), 0x0080, WS_TABSTOP | ((bDefault_) ?  \
    BS_DEFPUSHBUTTON : BS_PUSHBUTTON), (x_), (y_), (cx_), (cy_)))

// Adds an edit to the current dialog template.
#define DlgEdt(wId_, pszCap_, x_, y_, cx_, cy_)                         \
    (DlgFillDit((wId_), (pszCap_), 0x0081, WS_TABSTOP | ES_AUTOHSCROLL |\
    ES_LEFT, (x_), (y_), (cx_), (cy_)))

// Adds a static to the current dialog template.
#define DlgStc(wId_, pszCap_, x_, y_, cx_, cy_)                         \
    (DlgFillDit((wId_), (pszCap_), 0x0082, SS_LEFT, (x_), (y_), (cx_), (cy_)))

// Shows a modal dialog.
#define DlgShow(pDt_, pfnDlgProc_, p_) (DialogBoxIndirectParamW(g_hInst,\
    (pDt_), hWnd, (pfnDlgProc_), (LPARAM) (p_)))

// Creates dialog templates.
BOOL DlgCreateDtFlt(LPDLGTEMPLATEW *ppDt);
BOOL DlgCreateDtPrm(LPDLGTEMPLATEW *ppDt);
BOOL DlgCreateDtBld(LPDLGTEMPLATEW *ppDt);
BOOL DlgCreateDtRom(LPDLGTEMPLATEW *ppDt);

#endif // ifndef PQS_DIALOG_H_
