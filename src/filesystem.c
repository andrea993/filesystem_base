#include "filesystem.h"
#include "bitmap.h"

void format(fsInfo fs)
{
	writeBitmap(fs);
}

