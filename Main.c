// Main.c
// Providing application entry point.
// Implementing GUI operations.

#include "Common.h"

#include "Gui.h"
#include "Localization.h"
#include "Resources.h"

HINSTANCE g_hInst = NULL;
const WCHAR g_szClsName[] = L"PqsMain";

// Window procedure
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wpar, LPARAM lpar);

// Registers class.
BOOL MainRegisterClass();

// Creates window.
BOOL MainCreateWindow(HMENU hMenu, HWND *pWnd);

// Creates accelerator table.
BOOL MainCreateAccelTab(HACCEL *pAccelTab);

int APIENTRY wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR pszCmdLine,
    int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInst);
    UNREFERENCED_PARAMETER(pszCmdLine);
    g_hInst = hInst;

    if (!MainRegisterClass()) {
        MbFatal(IDS_MSGF_REGISTERCLASS);
        goto jFRegisterClass;
    }

    HMENU hMenu;
    if (!CreateMainMenu(&hMenu)) {
        MbFatal(IDS_MSGF_CREATEMENU);
        goto jFCreateMenu;
    }

    HWND hWnd;
    if (!MainCreateWindow(hMenu, &hWnd)) {
        MbFatal(IDS_MSGF_CREATEWINDOW);
        goto jFCreateWindow;
    }

    HACCEL hAccelTab;
    if (!MainCreateAccelTab(&hAccelTab)) {
        MbFatal(IDS_MSGF_CREATEACCELTAB);
        goto jFCreateAccelTab;
    }

    ShowWindow(hWnd, nCmdShow);
    if (!UpdateWindow(hWnd)) {
        MbFatal(IDS_MSGF_UPDATEWINDOW);
        goto jFUpdateWindow;
    }

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        if (!TranslateAcceleratorW(hWnd, hAccelTab, &msg)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    DestroyAcceleratorTable(hAccelTab);
    GuiShutdown();

    return (int) msg.wParam;

jFUpdateWindow:
    DestroyAcceleratorTable(hAccelTab);
jFCreateAccelTab:
    DestroyWindow(hWnd);
jFCreateWindow:
    DestroyMenu(hMenu);
jFCreateMenu:
jFRegisterClass:
    return -1;
}

BOOL MainRegisterClass() {
    WNDCLASSEXW wcx;
    wcx.cbSize          = sizeof(WNDCLASSEXW);
    wcx.style           = CS_HREDRAW | CS_VREDRAW;
    wcx.lpfnWndProc     = MainWndProc;
    wcx.cbClsExtra      = 0;
    wcx.cbWndExtra      = 0;
    wcx.hInstance       = g_hInst;
    wcx.hIcon           = LoadIconW(g_hInst, IDI_APPLICATION);
    wcx.hCursor         = LoadCursorW(g_hInst, IDC_ARROW);
    wcx.hbrBackground   = (HBRUSH) COLOR_BACKGROUND;
    wcx.lpszMenuName    = NULL;
    wcx.lpszClassName   = g_szClsName;
    wcx.hIconSm         = LoadIconW(g_hInst, IDI_APPLICATION);
    return !!RegisterClassExW(&wcx);
}

BOOL MainCreateWindow(HMENU hMenu, HWND *pWnd) {
    HWND hWnd = CreateWindowExW(
        0,
        g_szClsName,
        IDS_PQS,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        hMenu,
        g_hInst,
        NULL
    );
    if (!hWnd)
        return FALSE;
    *pWnd = hWnd;
    return TRUE;
}

BOOL MainCreateAccelTab(HACCEL *pAccelTab) {
#define N_ACCELS 9
    ACCEL accels[N_ACCELS] = {
        {FCONTROL | FVIRTKEY, 'L',     IDM_FILE_LOAD},
        {FCONTROL | FVIRTKEY, 'S',     IDM_FILE_SAVE},
        {           FVIRTKEY, VK_F1,   IDM_FILE_ABOUT},
        {FALT     | FVIRTKEY, VK_F4,   IDM_FILE_EXIT},
        {           FVIRTKEY, VK_F2,   IDM_VIEW_PRM},
        {           FVIRTKEY, VK_F3,   IDM_VIEW_BLD},
        {           FVIRTKEY, VK_F4,   IDM_VIEW_ROM},
        {FCONTROL | FVIRTKEY, 'F',     IDM_VIEW_FILTER},
        {           FVIRTKEY, VK_BACK, IDM_VIEW_PARENT},
    };
    HACCEL hAccelTab = CreateAcceleratorTableW(accels, N_ACCELS);
#undef N_ACCELS
    if (!hAccelTab)
        return FALSE;
    *pAccelTab = hAccelTab;
    return TRUE;
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wpar, LPARAM lpar) {
    switch (msg) {
    case WM_CREATE:
        if (!GuiInitialize(hWnd))
            return -1;
        GuiLoad(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_SIZE:
        GuiOnSize(hWnd);
        break;
    case WM_CLOSE:
        if (MbQuestion(IDS_MSGQ_EXIT, hWnd) == IDYES)
            DestroyWindow(hWnd);
        break;
    case WM_NOTIFY:
        GuiOnNotify(hWnd, (LPNMHDR) lpar);
        break;
    case WM_COMMAND:
        switch (LOWORD(wpar)) {
        case IDM_FILE_LOAD:
            if (MbQuestion(IDS_MSGQ_LOAD, hWnd) == IDYES)
                GuiLoad(hWnd);
            else
                GuiReady(hWnd);
            break;
        case IDM_FILE_SAVE:
            if (MbQuestion(IDS_MSGQ_SAVE, hWnd) == IDYES)
                GuiSave(hWnd);
            else
                GuiReady(hWnd);
            break;
        case IDM_FILE_ABOUT:
            MbInfo(IDS_ABOUT, hWnd);
            break;
        case IDM_FILE_EXIT:
            SendMessageW(hWnd, WM_CLOSE, 0, 0);
            break;
        default:
            if (GuiOnCommand(hWnd, (WORD) wpar))
                break;
            return DefWindowProcW(hWnd, msg, wpar, lpar);
        }
        break;
    default:
        return DefWindowProcW(hWnd, msg, wpar, lpar);
    }
    return 0;
}
