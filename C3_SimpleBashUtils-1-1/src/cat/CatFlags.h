#ifndef CAT_CATFLAGS_H
#define CAT_CATFLAGS_H

#include "catFlags.c"

void CatSetTable(const char *table[static 256]);
void catSetEndl(const char *table[static 256]);
void catSetNonPrintable(const char *table[static 256]);
void catSetTab(const char *table[static 256]);
#endif