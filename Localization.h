// Resources.h
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
#define IDS_LVROM_ARE           L"Area"
#define IDS_LVROM_SHR           L"Shared Area"
#define IDS_LVROM_PRI           L"Price"
#define IDS_LVROM_UNT           L"Unit"
#define IDS_LVROM_FLR           L"Floor"
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
#define IDS_DLGROM_ARE          L"Area:"
#define IDS_DLGROM_SHR          L"Shared Area:"
#define IDS_DLGROM_PRI          L"Price:"
#define IDS_DLGROM_UNT          L"Unit:"
#define IDS_DLGROM_FLR          L"Floor:"
#define IDS_DLGROM_TNUM         L"The ID"
#define IDS_DLGROM_TTYP         L"The type"
#define IDS_DLGROM_TARE         L"The area"
#define IDS_DLGROM_TSHR         L"The shared area"
#define IDS_DLGROM_TPRI         L"The price"
#define IDS_DLGROM_TUNT         L"The unit"
#define IDS_DLGROM_TFLR         L"The floor"

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

#define IDS_PQS                 L"楼盘查询系统"

#define IDS_ABOUT               L"一个使用C编写的简易楼盘查询系统。\n" \
                                L"提供了数据项的添加、修改、删除、排序、过滤功能。\n" \
                                L"此程序是C语言程序与设计的课程设计。\n"

#define IDS_MSGE_MEMORY         L"内存分配失败。"

#define IDS_MSGF_REGISTERCLASS  L"严重错误：无法注册类。"
#define IDS_MSGF_CREATEMENU     L"严重错误：无法创建菜单。"
#define IDS_MSGF_CREATEWINDOW   L"严重错误：无法创建窗口。"
#define IDS_MSGF_CREATEACCELTAB L"严重错误：无法创建快捷键表。"
#define IDS_MSGF_UPDATEWINDOW   L"严重错误：无法更新窗口"

#define IDS_MENU_FILE           L"文件(&F)"
#define IDS_MENU_FILE_LOAD      L"从磁盘载入(&L)\tCtrl+L"
#define IDS_MENU_FILE_SAVE      L"保存到磁盘(&S)\tCtrl+S"
#define IDS_MENU_FILE_ABOUT     L"关于(&A)\tF1"
#define IDS_MENU_FILE_EXIT      L"退出(&X)\tAlt+F4"
#define IDS_MENU_VIEW           L"视图(&V)"
#define IDS_MENU_VIEW_PRM       L"楼盘视图(&P)\tF2"
#define IDS_MENU_VIEW_BLD       L"楼栋视图(&B)\tF3"
#define IDS_MENU_VIEW_ROM       L"房屋视图(&R)\tF4"
#define IDS_MENU_VIEW_PARENT    L"返回上级视图(&U)\tBackspace"
#define IDS_MENU_VIEW_FILTER    L"过滤器(&F)...\tCtrl+F"

#define IDS_MSGQ_LOAD           L"确定要从磁盘载入所有数据？"
#define IDS_MSGQ_SAVE           L"确定要保存所有数据到磁盘？"
#define IDS_MSGQ_EXIT           L"确定要退出此程序？"
#define IDS_MSGE_LOAD           L"载入数据失败。"
#define IDS_MSGE_SAVE           L"保存数据失败。"

#define IDS_MPRM_ADD            L"添加一个楼栋(&A)..."
#define IDS_MPRM_MOD            L"修改(&M)..."
#define IDS_MPRM_RMV            L"移除(&R)"
#define IDS_MPRM_SUB            L"查看所有楼栋(&V)"
#define IDS_MPRM_NEW            L"新增一个楼盘(&N)..."

#define IDS_MBLD_ADD            L"添加一个房屋(&A)..."
#define IDS_MBLD_MOD            L"修改(&M)..."
#define IDS_MBLD_RMV            L"移除(&R)"
#define IDS_MBLD_SUB            L"查看所有房间(&V)"
#define IDS_MBLD_PAR            L"查看所属楼盘(&I)"

#define IDS_MROM_MOD            L"修改(&M)..."
#define IDS_MROM_RMV            L"移除(&R)"
#define IDS_MROM_PAR            L"查看所属楼栋(&I)"
#define IDS_MROM_ANC            L"查看所属楼盘(&E)"

#define IDS_LVPRM_NUM           L"编号"
#define IDS_LVPRM_NAM           L"名称"
#define IDS_LVPRM_ADD           L"地址"
#define IDS_LVPRM_DEV           L"开发商"
#define IDS_LVPRM_PMC           L"物业公司"
#define IDS_LVPRM_BLD           L"楼栋数"
#define IDS_LVPRM_ROM           L"房屋数"
#define IDS_MSGQ_PRMRMV         L"确定要移除此楼盘及其所有楼栋和房屋？"

#define IDS_LVBLD_NUM           L"编号"
#define IDS_LVBLD_NAM           L"名称"
#define IDS_LVBLD_UNT           L"单元数"
#define IDS_LVBLD_FLR           L"楼层数"
#define IDS_LVBLD_ROM           L"房屋数"
#define IDS_LVBLD_PRM           L"所属楼盘"
#define IDS_MSGQ_BLDRMV         L"确定要移除此楼栋及其所有房屋？"

#define IDS_LVROM_NUM           L"编号"
#define IDS_LVROM_TYP           L"户型"
#define IDS_LVROM_ARE           L"面积"
#define IDS_LVROM_SHR           L"公摊面积"
#define IDS_LVROM_PRI           L"价格"
#define IDS_LVROM_UNT           L"单元"
#define IDS_LVROM_FLR           L"楼层"
#define IDS_LVROM_BLD           L"所属楼栋"
#define IDS_LVROM_PRM           L"所属楼盘"
#define IDS_MSGQ_ROMRMV         L"确定要移除此房屋？"

#define LSG_MSGE_EMPTY(what_)   what_ L"不应为空。"
#define LSG_MSGE_STRLEN(what_)  what_ L"应短于" STR(WSTR_MAXLEN) L"个字符。"
#define LSG_MSGE_DWORD(what_)   what_ L"应为一个小于4294967296的非负整数。"
#define LSG_MSGE_NNDBL(what_)   what_ L"应为一个小于4294967296的非负实数。"
#define LSG_MSGE_UNIQUE(what_)  what_ L"不应与其他项重复。"

#define IDS_DLG_BOK             L"确定"
#define IDS_DLG_BCA             L"取消"
#define IDS_DLG_BCL             L"清除"

#define IDS_DLGFLT_CAP          L"过滤器"
#define IDS_DLGFLT_FLT          L"过滤器:"
#define IDS_DLGFLT_TFLT         L"过滤器"

#define IDS_DLGPRM_CAPADD       L"新增楼盘"
#define IDS_DLGPRM_CAPMOD       L"修改楼盘"
#define IDS_DLGPRM_NUM          L"编号："
#define IDS_DLGPRM_NAM          L"名称："
#define IDS_DLGPRM_ADD          L"地址："
#define IDS_DLGPRM_DEV          L"开发商："
#define IDS_DLGPRM_PMC          L"物业公司："
#define IDS_DLGPRM_TNUM         L"编号"
#define IDS_DLGPRM_TNAM         L"名称"
#define IDS_DLGPRM_TADD         L"地址"
#define IDS_DLGPRM_TDEV         L"开发商"
#define IDS_DLGPRM_TPMC         L"物业公司"

#define IDS_DLGBLD_CAPADD       L"新增楼栋"
#define IDS_DLGBLD_CAPMOD       L"修改楼栋"
#define IDS_DLGBLD_NUM          L"编号："
#define IDS_DLGBLD_NAM          L"名称："
#define IDS_DLGBLD_UNT          L"单元数："
#define IDS_DLGBLD_FLR          L"楼层数："
#define IDS_DLGBLD_TNUM         L"编号"
#define IDS_DLGBLD_TNAM         L"名称"
#define IDS_DLGBLD_TUNT         L"单元数"
#define IDS_DLGBLD_TFLR         L"楼层数"

#define IDS_DLGROM_CAPADD       L"新增房屋"
#define IDS_DLGROM_CAPMOD       L"修改房屋"
#define IDS_DLGROM_NUM          L"编号："
#define IDS_DLGROM_TYP          L"户型："
#define IDS_DLGROM_ARE          L"面积："
#define IDS_DLGROM_SHR          L"公摊面积："
#define IDS_DLGROM_PRI          L"价格："
#define IDS_DLGROM_UNT          L"单元："
#define IDS_DLGROM_FLR          L"楼层："
#define IDS_DLGROM_TNUM         L"编号"
#define IDS_DLGROM_TTYP         L"户型"
#define IDS_DLGROM_TARE         L"面积"
#define IDS_DLGROM_TSHR         L"公摊面积"
#define IDS_DLGROM_TPRI         L"价格"
#define IDS_DLGROM_TUNT         L"单元"
#define IDS_DLGROM_TFLR         L"楼层"

#define IDS_SB_STA_READY        L"就绪"
#define IDS_SB_STA_LOADED       L"载入成功"
#define IDS_SB_STA_SAVED        L"保存成功"
#define IDS_SB_FLT_ALL          L"显示所有的%lu个结果"
#define IDS_SB_FLT_FLT          L"显示%lu个结果，过滤器：%s"
#define IDS_SB_FLT_INV          L"无效过滤器"
#define IDS_SB_PRM              L"楼盘总数：%lu"
#define IDS_SB_BLD              L"楼栋总数：%lu"
#define IDS_SB_ROM              L"房屋总数：%lu"


#else // ifdef PQS_LOC_ZH

#error No localization specified.

#endif // if !(defined PQS_LOC_EN) && !(defined PQS_LOC_ZH)

#endif // ifndef PQS_LOCALIZATION_H_
