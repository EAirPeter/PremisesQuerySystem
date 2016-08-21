// Resources.h
// Resource ids

#ifndef PQS_RESOURCES_H_
#define PQS_RESOURCES_H_

// Strings
#define IDS_FNAME_PRM   L"PqsPremises.dat"
#define IDS_FNAME_BLD   L"PqsBuildings.dat"
#define IDS_FNAME_ROM   L"PqsRooms.dat"

// IDs
enum IDS_ {
    ID_PQS_START    = 0100000,
    // Main menu - File
    IDM_FILE,
    IDM_FILE_LOAD,
    IDM_FILE_SAVE,
    IDM_FILE_ABOUT,
    IDM_FILE_EXIT,
    // Main menu - View
    IDM_VIEW,
    IDM_VIEW_PRM,
    IDM_VIEW_BLD,
    IDM_VIEW_ROM,
    IDM_VIEW_PARENT,
    IDM_VIEW_FILTER,
    // Popup menu
    IDM_POP_ADD,
    IDM_POP_MOD,
    IDM_POP_RMV,
    IDM_POP_SUB,
    IDM_POP_PAR,
    IDM_POP_ANC,
    IDM_POP_NEW,
    // Timer
    IDT_TOREADY,
    // Controls - Filter dialog
    IDC_DLGFLT_SFLT,
    IDC_DLGFLT_EFLT,
    // Controls - Premises information dialog
    IDC_DLGPRM_SNUM,
    IDC_DLGPRM_ENUM,
    IDC_DLGPRM_SNAM,
    IDC_DLGPRM_ENAM,
    IDC_DLGPRM_SADD,
    IDC_DLGPRM_EADD,
    IDC_DLGPRM_SDEV,
    IDC_DLGPRM_EDEV,
    IDC_DLGPRM_SPMC,
    IDC_DLGPRM_EPMC,
    // Controls - Building information dialog
    IDC_DLGBLD_SNUM,
    IDC_DLGBLD_ENUM,
    IDC_DLGBLD_SNAM,
    IDC_DLGBLD_ENAM,
    IDC_DLGBLD_SUNT,
    IDC_DLGBLD_EUNT,
    IDC_DLGBLD_SFLR,
    IDC_DLGBLD_EFLR,
    // Controls - Room information dialog
    IDC_DLGROM_SNUM,
    IDC_DLGROM_ENUM,
    IDC_DLGROM_STYP,
    IDC_DLGROM_ETYP,
    IDC_DLGROM_SARE,
    IDC_DLGROM_EARE,
    IDC_DLGROM_SSHR,
    IDC_DLGROM_ESHR,
    IDC_DLGROM_SPRI,
    IDC_DLGROM_EPRI,
    IDC_DLGROM_SUNT,
    IDC_DLGROM_EUNT,
    IDC_DLGROM_SFLR,
    IDC_DLGROM_EFLR,
};

// Status bar indecies
enum IDX_STATUS_BAR_ {
    IDX_SB_STA,
    IDX_SB_FLT,
    IDX_SB_PRM,
    IDX_SB_BLD,
    IDX_SB_ROM,
    // Status bar part count
    N_STATUSBAR,
};

#endif // ifndef PQS_RESOURCES_H_
