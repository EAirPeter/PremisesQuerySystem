// Filter.c
// Implementing filter utilities

#include "Common.h"

#include "Filter.h"

typedef enum tagFPART_TYPE_ {
    FPT_NUM,
    FPT_TXT,
} FPART_TYPE;

typedef struct tagFILTER_PART_  FILTER_PART;
typedef FILTER_PART *           PFILTER_PART;
typedef const FILTER_PART *     PCFILTER_PART;

struct tagFILTER_PART_ {
    // Linked list
    PFILTER_PART    pNext;
    // Filter part
    FPART_TYPE      fptType;
    DWORD           dwNum;
    WSTR            szText;
};

static PFILTER_PART s_pFpAnc = NULL;    // Matching the ancient.
static PFILTER_PART s_pFpPar = NULL;    // Matching the parent.
static PFILTER_PART s_pFpObj = NULL;    // Matching the object itself.
static BOOL         s_bValid = FALSE;

// Parsers
PCWCHAR ParseFilterString(PCWCHAR c);
PCWCHAR ParseFilterPart(PCWCHAR c);
PCWCHAR ParseFilterPartSpec(PCWCHAR c, PFILTER_PART **pppFpHead);
PCWCHAR ParseFilterPartString(PCWCHAR c, PFILTER_PART *ppFp);
PCWCHAR ParseCharseqNoQuoteWs(PCWCHAR c, PWCHAR p);
PCWCHAR ParseCharseqNoQuote(PCWCHAR c, PWCHAR p);
PCWCHAR ParseWs(PCWCHAR c);

BOOL FpsAccepted(PFILTER_PART pFp, DWORD dwNum, PCWSTR szText);
#define FpsAcN(fp_, p_) (FpsAccepted((fp_), (p_)->dwNum, (p_)->szName))
#define FpsAcT(fp_, p_) (FpsAccepted((fp_), (p_)->dwNum, (p_)->szType))

void FltClear(void) {
#define FP_FREEALL(p_) while ((p_)) {   \
    PFILTER_PART pTmp = (p_)->pNext;    \
    HeapFree(GetProcessHeap(), 0, (p_));\
    (p_) = pTmp;                        \
}
    FP_FREEALL(s_pFpAnc)
    FP_FREEALL(s_pFpPar)
    FP_FREEALL(s_pFpObj)
#undef FP_FREEALL
}

BOOL FltPrepare(PCWSTR pszFlt) {
    FltClear();
    s_bValid = TRUE;
    ParseFilterString(pszFlt);
    return s_bValid;
}

BOOL FltPrm(PPREMISES pPrm) {
    return FpsAcN(s_pFpObj, pPrm);
}

BOOL FltBld(PBUILDING pBld) {
    return FpsAcN(s_pFpObj, pBld) && FpsAcN(s_pFpPar, pBld->pPrm);
}

BOOL FltRom(PROOM pRom) {
    return FpsAcT(s_pFpObj, pRom) && FpsAcN(s_pFpPar, pRom->pBld) &&
        FpsAcN(s_pFpAnc, pRom->pBld->pPrm);
}

PCWCHAR ParseFilterString(PCWCHAR c) {
    while (*c && (c = ParseFilterPart(c)) && *c)
        c = ParseWs(c);
    return c;
}

PCWCHAR ParseFilterPart(PCWCHAR c) {
    PFILTER_PART *ppFpHead;
    if (!(c = ParseFilterPartSpec(c, &ppFpHead)))
        return NULL;
    PFILTER_PART pFp;
    if (!(c = ParseFilterPartString(c, &pFp)))
        return NULL;
    if (!s_bValid)
        return NULL;
    pFp->pNext = *ppFpHead;
    *ppFpHead = pFp;
    return c;
}

PCWCHAR ParseFilterPartSpec(PCWCHAR c, PFILTER_PART **pppFpHead) {
    if (*c == L'$') {
        *pppFpHead = &s_pFpAnc;
        return c + 1;
    }
    if (*c == L'^') {
        *pppFpHead = &s_pFpPar;
        return c + 1;
    }
    *pppFpHead = &s_pFpObj;
    return c;
}

static inline PCWCHAR ParseFilterPartString_(PCWCHAR c, PFILTER_PART pFp) {
    if (c[0] == L'\"') {
        if (c[1] == L'+' && c[2] == L'&') {
            c = ParseCharseqNoQuote(c + 3, pFp->szText);
            if (!StringToDword(pFp->szText, &pFp->dwNum)) {
                s_bValid = FALSE;
                return NULL;
            }
            pFp->fptType = FPT_NUM;
        }
        else {
            c = ParseCharseqNoQuote(c + 1, pFp->szText);
            pFp->fptType = FPT_TXT;
        }
    }
    else {
        if (c[0] == L'+' && c[1] == L'&') {
            c = ParseCharseqNoQuoteWs(c + 2, pFp->szText);
            if (!StringToDword(pFp->szText, &pFp->dwNum)) {
                s_bValid = FALSE;
                return NULL;
            }
            pFp->fptType = FPT_NUM;
        }
        else {
            c = ParseCharseqNoQuoteWs(c, pFp->szText);
            pFp->fptType = FPT_TXT;
        }
    }
    return c;
}

PCWCHAR ParseFilterPartString(PCWCHAR c, PFILTER_PART *ppFp) {
    PFILTER_PART pFp = (PFILTER_PART)
        HeapAlloc(GetProcessHeap(), 0, sizeof(FILTER_PART));
    c = ParseFilterPartString_(c, pFp);
    if (s_bValid)
        *ppFp = pFp;
    else
        HeapFree(GetProcessHeap(), 0, pFp);
    return c;
}

PCWCHAR ParseCharseqNoQuoteWs(PCWCHAR c, PWCHAR p) {
    if (!*c || *c == L'\"' || *c == L' ' || *c == L'\t') {
        s_bValid = FALSE;
        return NULL;
    }
    while (*c && *c != L'\"' && *c != L' ' && *c != L'\t')
        *p++ = *c++;
    *p = L'\0';
    return c;
}

PCWCHAR ParseCharseqNoQuote(PCWCHAR c, PWCHAR p) {
    if (!*c || *c == L'\"') {
        s_bValid = FALSE;
        return NULL;
    }
    while (*c && *c != L'\"')
        *p++ = *c++;
    *p = L'\0';
    if (*c != L'\"')
        s_bValid = FALSE;
    return c + 1;
}

PCWCHAR ParseWs(PCWCHAR c) {
    while (*c == L' ' || *c == L'\t')
        ++c;
    return c;
}

BOOL FpsAccepted(PFILTER_PART pFp, DWORD dwNum, PCWSTR szText) {
    if (!pFp)
        return TRUE;
    while (pFp) {
        switch (pFp->fptType) {
        case FPT_NUM:
            if (dwNum == pFp->dwNum)
                return TRUE;
            break;
        case FPT_TXT:
            if (szText && StrStrIW(szText, pFp->szText))
                return TRUE;
            break;
        }
        pFp = pFp->pNext;
    }
    return FALSE;
}
