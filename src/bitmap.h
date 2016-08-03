#ifndef BITMAP_H
#define BITMAP_H


#include "type.h"
#include "filesystem.h"

void writeBitmap(fsInfo fs);
bool isByteFreeAt(dword i,fsInfo fs);
dword getFirstNfree(word n, fsInfo fs);

#endif
