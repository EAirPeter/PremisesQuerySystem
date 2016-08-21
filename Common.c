// Common.c
// Implementing common utilites

#include "Common.h"

static inline PCWCHAR CmParseDword(PCWCHAR pw, DWORD *pdw) {
    DWORD dw = 0;
    while (iswdigit(*pw)) {
        DWORD dw2 = dw + dw;
        if (dw2 < dw) {
            *pdw = dw;
            return pw;
        }
        DWORD dw4 = dw2 + dw2;
        if (dw4 < dw2) {
            *pdw = dw;
            return pw;
        }
        DWORD dw8 = dw4 + dw4;
        if (dw8 < dw4) {
            *pdw = dw;
            return pw;
        }
        DWORD dw10 = dw8 + dw2;
        if (dw10 < dw8) {
            *pdw = dw;
            return pw;
        }
        dw = dw10 + (DWORD) (*pw++ & 0x0f);
        if (dw < dw10) {
            *pdw = dw;
            return pw - 1;
        }
    }
    *pdw = dw;
    return pw;
}

BOOL StringToDword(PCWSTR psz, DWORD *pdw) {
    PCWCHAR pw = CmParseDword(psz, pdw);
    if (*pw)
        return FALSE;
    return TRUE;
}

BOOL StringToNndbl(PCWSTR psz, double *pd) {
    DWORD dwInt;
    PCWCHAR pw = CmParseDword(psz, &dwInt);
    if (!*pw) {
        *pd = (double) dwInt;
        return TRUE;
    }
    if (*pw != L'.')
        return FALSE;
    if (!*++pw) {
        *pd = (double) dwInt;
        return TRUE;
    }
    if (!iswxdigit(*pw))
        return FALSE;
    double dFlt = (double) (*pw++ & 0x0f) / 10.0;
    if (!*pw) {
        *pd = dFlt + dwInt;
        return TRUE;
    }
    if (!iswxdigit(*pw))
        return FALSE;
    dFlt += (double) (*pw++ & 0x0f) / 100.0;
    while (*pw && iswxdigit(*pw))
        ++pw;
    if (*pw)
        return FALSE;
    *pd = dFlt + dwInt;
    return TRUE;
}
