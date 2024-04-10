#pragma once

#ifdef _DEBUG
#define DEBUGPRINT(text) printf(text);
#else
#define DEBUGPRINT(text) {}
#endif
