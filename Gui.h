// Gui.h
// Gui operations

#ifndef PQS_GUI_H_
#define PQS_GUI_H_

#include "Common.h"

// Message box wrappers
#define MbFatal(text_)              \
    (MessageBoxW(NULL, (text_), IDS_PQS, MB_ICONERROR | MB_OK))
#define MbError(text_, hwnd_)       \
    (MessageBoxW((hwnd_), (text_), IDS_PQS, MB_ICONERROR | MB_OK))
#define MbWarning(text_, hwnd_)     \
    (MessageBoxW((hwnd_), (text_), IDS_PQS, MB_ICONWARNING | MB_OK))
#define MbQuestion(text_, hwnd_)    \
    (MessageBoxW((hwnd_), (text_), IDS_PQS, MB_ICONQUESTION | MB_YESNO))
#define MbInfo(text_, hwnd_)        \
    (MessageBoxW((hwnd_), (text_), IDS_PQS, MB_OK))

// Creates the main menu.
BOOL CreateMainMenu(HMENU *pMenu);

typedef enum tagGUI_STATE_ {
    GS_INV,
    GS_PRM,
    GS_BLD,
    GS_ROM,
} GUI_STATE;

// Loads all data from disk and updates status bar.
void GuiLoad(HWND hWnd);

// Saves all data to disk and updates status bar.
void GuiSave(HWND hWnd);

// Sets status bar to ready.
void GuiReady(HWND hWnd);

// Initialization
BOOL GuiInitialize(HWND hWnd);

// Uninitialization
void GuiShutdown(void);

// Switches the GUI state.
void GuiSwitch(HWND hWnd, GUI_STATE gs, PCWSTR pszFlt);

// On message: WM_SIZE
void GuiOnSize(HWND hWnd);

// On message: WM_NOTIFY
void GuiOnNotify(HWND hWnd, LPNMHDR pHdr);

// On message: WM_COMMAND
BOOL GuiOnCommand(HWND hWnd, WORD wId);

#endif // ifndef PQS_GUI_H_
