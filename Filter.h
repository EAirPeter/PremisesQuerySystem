// Filter.h
// Filter utilities
//
// filter-string:
// - filter-part
// - filter-string ws filter-part
// filter-part:
// - [filter-part-spec] filter-part-string
// filter-part-spec:
// - `$`    => The ancient
// - `^`    => The parent
// filter-part-string:
// - [`+&`] charseq-no-quote-ws
// - `"` [`+&`] charseq-no-quote `"`
// ws:
// - (` ` | `\t`)+

#ifndef PQS_FILTER_H_
#define PQS_FILTER_H_

#include "Common.h"

#include "Object.h"

// Cleans up the resources used to build a filter.
void FltClear(void);

// Parses a filter string and prepares for filtering.
// - Invokes FltClear.
BOOL FltPrepare(PCWSTR pszFlt);

// Decides whether the current filter accepts the premises.
BOOL FltPrm(PPREMISES pPrm);

// Decides whether the current filter accepts the building.
BOOL FltBld(PBUILDING pBld);

// Decides whether the current filter accepts the room.
BOOL FltRom(PROOM pRom);

#endif // ifndef PQS_FILTER_H_
