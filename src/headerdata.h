#ifndef HEADERDATA_H
#define HEADERDATA_H

#include "type.h"
#include <memory.h>

/*
 --Entry Header--
 |Timestamp|inUse|dir_file|rw_ro|exec|hidden|length|nextPtr|name|
 |33       |1    |1       |1    |1   |1     |7     |19     |128 |	=192bit=24byte

 --Next Header--
 |length|nextPtr|
 |13     |19	 | =32bit =4byte

 --Bitmap Header--
 |length|
 |32    |

 */


#define EHDRLEN 24
typedef struct _entryHeader
{
	sgnddword timestmp;
	bool inUse;
	bool dir_file;
	bool rw_ro;
	bool exec;
	bool hidden;
	byte length;
	dword nextPtr;
	byte name[17]; /* 17 null terminator */

}entryHeader;


#define NHDRLEN 4
typedef struct _nextHeader
{
	word length; /* following page can be 2^16byte length */
	dword nextPtr;
}nextHeader;

#define BHDRLEN 4
typedef struct _bitmapHeader
{
	dword length;
}bitmapHeader;



entryHeader readEntryHeader(const byte *buffer);
void writeEntryHeader(byte* buffer,const entryHeader header);
nextHeader readNextHeader(const byte *buffer);
void writeNextHeader(byte* buffer,const nextHeader header);

#endif
