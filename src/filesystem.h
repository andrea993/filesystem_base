#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>
#include "type.h"

typedef struct _fsInfo
{
	char devfname[30];
	FILE *fp;
	dword size;
}fsInfo;

void format(fsInfo fs);

#endif
