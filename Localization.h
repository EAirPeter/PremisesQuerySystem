// Localization.h
// Strings displayed on GUIs

#ifndef PQS_LOCALIZATION_H_
#define PQS_LOCALIZATION_H_

#define PQS_LOC_ZH

#if defined PQS_LOC_EN

#define IDS_PQS                 L"Premises Query System"

#define IDS_ABOUT               L"A simple premises query system implemented in C.\n" \
                                L"This program is used as a curriculum design.\n"

#define IDS_MSGE_MEMORY         L"Failed to allocate memory."

#define IDS_MSGF_REGISTERCLASS  L"FATAL: Failed to register class."
#define IDS_MSGF_CREATEMENU     L"FATAL: Failed to create menu."
#define IDS_MSGF_CREATEWINDOW   L"FATAL: Failed to create window."
#define IDS_MSGF_CREATEACCELTAB L"FATAL: Failed to create accelerator table."
#define IDS_MSGF_UPDATEWINDOW   L"FATAL: Failed to update window."

#define IDS_MENU_FILE           L"&File"
#define IDS_MENU_FILE_LOAD      L"&Load from disk\tCtrl+L"
#define IDS_MENU_FILE_SAVE      L"&Save to disk\tCtrl+S"
#define IDS_MENU_FILE_ABOUT     L"&About\tF1"
#define IDS_MENU_FILE_EXIT      L"E&xit\tAlt+F4"
#define IDS_MENU_VIEW           L"&View"
#define IDS_MENU_VIEW_PRM       L"&Premises view\tF2"
#define IDS_MENU_VIEW_BLD       L"&Buildings view\tF3"
#define IDS_MENU_VIEW_ROM       L"&Rooms view\tF4"
#define IDS_MENU_VIEW_PARENT    L"Ret&urn to the parent view\tBackspace"
#define IDS_MENU_VIEW_FILTER    L"&Filter...\tCtrl+F"

#define IDS_MSGQ_LOAD           L"Are you sure to load all data from disk?"
#define IDS_MSGQ_SAVE           L"Are you sure to save all data to disk?"
#define IDS_MSGQ_EXIT           L"Are you sure to exit this program?"
#define IDS_MSGE_LOAD           L"Failed to load data."
#define IDS_MSGE_SAVE           L"Failed to save data."

#define IDS_MPRM_ADD            L"&Add a building..."
#define IDS_MPRM_MOD            L"&Modify..."
#define IDS_MPRM_RMV            L"&Remove"
#define IDS_MPRM_SUB            L"&View all buildings"
#define IDS_MPRM_NEW            L"&New premises..."

#define IDS_MBLD_ADD            L"&Add a room..."
#define IDS_MBLD_MOD            L"&Modify..."
#define IDS_MBLD_RMV            L"&Remove"
#define IDS_MBLD_SUB            L"&View all rooms"
#define IDS_MBLD_PAR            L"V&iew the premises"

#define IDS_MROM_MOD            L"&Modify..."
#define IDS_MROM_RMV            L"&Remove"
#define IDS_MROM_PAR            L"V&iew the building"
#define IDS_MROM_ANC            L"Vi&ew the premises"

#define IDS_LVPRM_NUM           L"ID"
#define IDS_LVPRM_NAM           L"Name"
#define IDS_LVPRM_ADD           L"Address"
#define IDS_LVPRM_DEV           L"Developer"
#define IDS_LVPRM_PMC           L"Property Management"
#define IDS_LVPRM_BLD           L"Buildings"
#define IDS_LVPRM_ROM           L"Rooms"
#define IDS_MSGQ_PRMRMV         L"Are you sure to remove this premises and " \
    L"all its buildings and rooms?"

#define IDS_LVBLD_NUM           L"ID"
#define IDS_LVBLD_NAM           L"Name"
#define IDS_LVBLD_UNT           L"Units"
#define IDS_LVBLD_FLR           L"Floors"
#define IDS_LVBLD_ROM           L"Rooms"
#define IDS_LVBLD_PRM           L"Premises"
#define IDS_MSGQ_BLDRMV         L"Are you sure to remove this building and " \
    L"all its rooms?"

#define IDS_LVROM_NUM           L"ID"
#define IDS_LVROM_TYP           L"Type"
#define IDS_LVROM_ARE           L"Area(m^2)"
#define IDS_LVROM_PRI           L"Price(RMB/m^2)"
#define IDS_LVROM_BLD           L"Building"
#define IDS_LVROM_PRM           L"Premises"
#define IDS_MSGQ_ROMRMV         L"Are you sure to remove this room?"

#define LSG_MSGE_EMPTY(what_)   what_ L" should not be empty."
#define LSG_MSGE_STRLEN(what_)  what_ L" should be shorter than " \
    STR(WSTR_MAXLEN) L" chars."
#define LSG_MSGE_DWORD(what_)   what_ L" should be a non-negative " \
    L"integer below 4294967296."
#define LSG_MSGE_NNDBL(what_)   what_ L" should be a non-negative " \
    L"real number below 4294967296."
#define LSG_MSGE_UNIQUE(what_)  what_ L" should be unique."

#define IDS_DLG_BOK             L"OK"
#define IDS_DLG_BCA             L"Cancel"
#define IDS_DLG_BCL             L"Clear"

#define IDS_DLGFLT_CAP          L"Filter"
#define IDS_DLGFLT_FLT          L"Filter:"
#define IDS_DLGFLT_TFLT         L"The filter"

#define IDS_DLGPRM_CAPADD       L"Add a premises"
#define IDS_DLGPRM_CAPMOD       L"Modify the premises"
#define IDS_DLGPRM_NUM          L"ID:"
#define IDS_DLGPRM_NAM          L"Name:"
#define IDS_DLGPRM_ADD          L"Address:"
#define IDS_DLGPRM_DEV          L"Developer:"
#define IDS_DLGPRM_PMC          L"Property Management:"
#define IDS_DLGPRM_TNUM         L"The ID"
#define IDS_DLGPRM_TNAM         L"The name"
#define IDS_DLGPRM_TADD         L"The address"
#define IDS_DLGPRM_TDEV         L"The developer"
#define IDS_DLGPRM_TPMC         L"The property management"

#define IDS_DLGBLD_CAPADD       L"Add a building"
#define IDS_DLGBLD_CAPMOD       L"Modify the building"
#define IDS_DLGBLD_NUM          L"ID:"
#define IDS_DLGBLD_NAM          L"Name:"
#define IDS_DLGBLD_UNT          L"Units:"
#define IDS_DLGBLD_FLR          L"Floors:"
#define IDS_DLGBLD_TNUM         L"The ID"
#define IDS_DLGBLD_TNAM         L"The name"
#define IDS_DLGBLD_TUNT         L"The units"
#define IDS_DLGBLD_TFLR         L"The floors"

#define IDS_DLGROM_CAPADD       L"Add a room"
#define IDS_DLGROM_CAPMOD       L"Modify the room"
#define IDS_DLGROM_NUM          L"ID:"
#define IDS_DLGROM_TYP          L"Type:"
#define IDS_DLGROM_ARE          L"Area(m^2):"
#define IDS_DLGROM_PRI          L"Price(RMB/m^2):"
#define IDS_DLGROM_TNUM         L"The ID"
#define IDS_DLGROM_TTYP         L"The type"
#define IDS_DLGROM_TARE         L"The area"
#define IDS_DLGROM_TPRI         L"The price"

#define IDS_SB_STA_READY        L"Ready"
#define IDS_SB_STA_LOADED       L"Loaded successfully"
#define IDS_SB_STA_SAVED        L"Saved successfully"
#define IDS_SB_FLT_ALL          L"Showing all %lu results"
#define IDS_SB_FLT_FLT          L"Showing %lu results of filter: %s"
#define IDS_SB_FLT_INV          L"Invalid filter"
#define IDS_SB_PRM              L"Total premises: %lu"
#define IDS_SB_BLD              L"Total buildings: %lu"
#define IDS_SB_ROM              L"Total rooms: %lu"

#elif defined PQS_LOC_ZH // ifdef PQS_LOC_EN

#define IDS_PQS                 L"¥�̲�ѯϵͳ"

#define IDS_ABOUT               L"һ��ʹ��C��д�ļ���¥�̲�ѯϵͳ��\n" \
                                L"�ṩ�����������ӡ��޸ġ�ɾ�������򡢹��˹��ܡ�\n" \
                                L"�˳����ǡ�C���Գ�����ơ��Ŀγ���ơ�\n"

#define IDS_MSGE_MEMORY         L"�ڴ����ʧ�ܡ�"

#define IDS_MSGF_REGISTERCLASS  L"���ش����޷�ע���ࡣ"
#define IDS_MSGF_CREATEMENU     L"���ش����޷������˵���"
#define IDS_MSGF_CREATEWINDOW   L"���ش����޷��������ڡ�"
#define IDS_MSGF_CREATEACCELTAB L"���ش����޷�������ݼ���"
#define IDS_MSGF_UPDATEWINDOW   L"���ش����޷����´���"

#define IDS_MENU_FILE           L"�ļ�(&F)"
#define IDS_MENU_FILE_LOAD      L"�Ӵ�������(&L)\tCtrl+L"
#define IDS_MENU_FILE_SAVE      L"���浽����(&S)\tCtrl+S"
#define IDS_MENU_FILE_ABOUT     L"����(&A)\tF1"
#define IDS_MENU_FILE_EXIT      L"�˳�(&X)\tAlt+F4"
#define IDS_MENU_VIEW           L"��ͼ(&V)"
#define IDS_MENU_VIEW_PRM       L"¥����ͼ(&P)\tF2"
#define IDS_MENU_VIEW_BLD       L"¥����ͼ(&B)\tF3"
#define IDS_MENU_VIEW_ROM       L"������ͼ(&R)\tF4"
#define IDS_MENU_VIEW_PARENT    L"�����ϼ���ͼ(&U)\tBackspace"
#define IDS_MENU_VIEW_FILTER    L"������(&F)...\tCtrl+F"

#define IDS_MSGQ_LOAD           L"ȷ��Ҫ�Ӵ��������������ݣ�"
#define IDS_MSGQ_SAVE           L"ȷ��Ҫ�����������ݵ����̣�"
#define IDS_MSGQ_EXIT           L"ȷ��Ҫ�˳��˳���"
#define IDS_MSGE_LOAD           L"��������ʧ�ܡ�"
#define IDS_MSGE_SAVE           L"��������ʧ�ܡ�"

#define IDS_MPRM_ADD            L"���һ��¥��(&A)..."
#define IDS_MPRM_MOD            L"�޸�(&M)..."
#define IDS_MPRM_RMV            L"�Ƴ�(&R)"
#define IDS_MPRM_SUB            L"�鿴����¥��(&V)"
#define IDS_MPRM_NEW            L"����һ��¥��(&N)..."

#define IDS_MBLD_ADD            L"���һ������(&A)..."
#define IDS_MBLD_MOD            L"�޸�(&M)..."
#define IDS_MBLD_RMV            L"�Ƴ�(&R)"
#define IDS_MBLD_SUB            L"�鿴���з���(&V)"
#define IDS_MBLD_PAR            L"�鿴����¥��(&I)"

#define IDS_MROM_MOD            L"�޸�(&M)..."
#define IDS_MROM_RMV            L"�Ƴ�(&R)"
#define IDS_MROM_PAR            L"�鿴����¥��(&I)"
#define IDS_MROM_ANC            L"�鿴����¥��(&E)"

#define IDS_LVPRM_NUM           L"���"
#define IDS_LVPRM_NAM           L"����"
#define IDS_LVPRM_ADD           L"��ַ"
#define IDS_LVPRM_DEV           L"������"
#define IDS_LVPRM_PMC           L"��ҵ��˾"
#define IDS_LVPRM_BLD           L"¥����"
#define IDS_LVPRM_ROM           L"��������"
#define IDS_MSGQ_PRMRMV         L"ȷ��Ҫ�Ƴ���¥�̼�������¥���ͷ��ݣ�"

#define IDS_LVBLD_NUM           L"���"
#define IDS_LVBLD_NAM           L"����"
#define IDS_LVBLD_UNT           L"��Ԫ��"
#define IDS_LVBLD_FLR           L"¥����"
#define IDS_LVBLD_ROM           L"��������"
#define IDS_LVBLD_PRM           L"����¥��"
#define IDS_MSGQ_BLDRMV         L"ȷ��Ҫ�Ƴ���¥���������з��ݣ�"

#define IDS_LVROM_NUM           L"���"
#define IDS_LVROM_TYP           L"����"
#define IDS_LVROM_ARE           L"���(ƽ����)"
#define IDS_LVROM_PRI           L"�۸�(Ԫ/ƽ����)"
#define IDS_LVROM_BLD           L"����¥��"
#define IDS_LVROM_PRM           L"����¥��"
#define IDS_MSGQ_ROMRMV         L"ȷ��Ҫ�Ƴ��˷��ݣ�"

#define LSG_MSGE_EMPTY(what_)   what_ L"��ӦΪ�ա�"
#define LSG_MSGE_STRLEN(what_)  what_ L"Ӧ����" STR(WSTR_MAXLEN) L"���ַ���"
#define LSG_MSGE_DWORD(what_)   what_ L"ӦΪһ��С��4294967296�ķǸ�������"
#define LSG_MSGE_NNDBL(what_)   what_ L"ӦΪһ��С��4294967296�ķǸ�ʵ����"
#define LSG_MSGE_UNIQUE(what_)  what_ L"��Ӧ���������ظ���"

#define IDS_DLG_BOK             L"ȷ��"
#define IDS_DLG_BCA             L"ȡ��"
#define IDS_DLG_BCL             L"���"

#define IDS_DLGFLT_CAP          L"������"
#define IDS_DLGFLT_FLT          L"������:"
#define IDS_DLGFLT_TFLT         L"������"

#define IDS_DLGPRM_CAPADD       L"����¥��"
#define IDS_DLGPRM_CAPMOD       L"�޸�¥��"
#define IDS_DLGPRM_NUM          L"��ţ�"
#define IDS_DLGPRM_NAM          L"���ƣ�"
#define IDS_DLGPRM_ADD          L"��ַ��"
#define IDS_DLGPRM_DEV          L"�����̣�"
#define IDS_DLGPRM_PMC          L"��ҵ��˾��"
#define IDS_DLGPRM_TNUM         L"���"
#define IDS_DLGPRM_TNAM         L"����"
#define IDS_DLGPRM_TADD         L"��ַ"
#define IDS_DLGPRM_TDEV         L"������"
#define IDS_DLGPRM_TPMC         L"��ҵ��˾"

#define IDS_DLGBLD_CAPADD       L"����¥��"
#define IDS_DLGBLD_CAPMOD       L"�޸�¥��"
#define IDS_DLGBLD_NUM          L"��ţ�"
#define IDS_DLGBLD_NAM          L"���ƣ�"
#define IDS_DLGBLD_UNT          L"��Ԫ����"
#define IDS_DLGBLD_FLR          L"¥������"
#define IDS_DLGBLD_TNUM         L"���"
#define IDS_DLGBLD_TNAM         L"����"
#define IDS_DLGBLD_TUNT         L"��Ԫ��"
#define IDS_DLGBLD_TFLR         L"¥����"

#define IDS_DLGROM_CAPADD       L"��������"
#define IDS_DLGROM_CAPMOD       L"�޸ķ���"
#define IDS_DLGROM_NUM          L"��ţ�"
#define IDS_DLGROM_TYP          L"���ͣ�"
#define IDS_DLGROM_ARE          L"���(ƽ����)��"
#define IDS_DLGROM_PRI          L"�۸�(Ԫ/ƽ����)��"
#define IDS_DLGROM_TNUM         L"���"
#define IDS_DLGROM_TTYP         L"����"
#define IDS_DLGROM_TARE         L"���"
#define IDS_DLGROM_TPRI         L"�۸�"

#define IDS_SB_STA_READY        L"����"
#define IDS_SB_STA_LOADED       L"����ɹ�"
#define IDS_SB_STA_SAVED        L"����ɹ�"
#define IDS_SB_FLT_ALL          L"��ʾ���е�%lu�����"
#define IDS_SB_FLT_FLT          L"��ʾ%lu���������������%s"
#define IDS_SB_FLT_INV          L"��Ч������"
#define IDS_SB_PRM              L"¥��������%lu"
#define IDS_SB_BLD              L"¥��������%lu"
#define IDS_SB_ROM              L"����������%lu"

#else // ifdef PQS_LOC_ZH

#error No localization specified.

#endif // if !(defined PQS_LOC_EN) && !(defined PQS_LOC_ZH)

#endif // ifndef PQS_LOCALIZATION_H_
