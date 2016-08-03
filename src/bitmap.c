#include "bitmap.h"
#include <math.h>
#include "headerdata.h"

void writeBitmap(fsInfo fs)
{
	bitmapHeader header={ceil(fs.size/8.0)};
	unsigned i,bmpspace,nmissing;
	rewind(fs.fp);
	fwrite(&header,sizeof(word),1,fs.fp);

	bmpspace=ceil(header.length/8.0);

	for(i=0;i<bmpspace-1;i++)
		fputc(0xff,fs.fp);

	//the missing bytes are set to used
	nmissing=bmpspace%8;
	fputc(0xff>>(8-nmissing),fs.fp);

	for(;i<header.length-1;i++)
		fputc(0,fs.fp);

	//the missing bytes are set to used
	nmissing=fs.size%8;
	fputc(0xff>>(8-nmissing),fs.fp);
}

bool isByteFreeAt(dword i,fsInfo fs)
{
	byte b;
	fseek(fs.fp,sizeof(dword)+i/8,SEEK_SET);
	fread(&b,1,1,fs.fp);

	return (b & 1<<(7-(i%8)))>0;

}

dword getFirstNfree(word n, fsInfo fs)
{
	word count=0;
	dword pos=0;
	byte b;
	int i;
	fseek(fs.fp,sizeof(dword),SEEK_SET);

	while(fread(&b, 1, 1, fs.fp) && count<n)
	{
		for(i=0;i<8 && count<n;i++)
		{
			if((b & (0x80>>i))>0)
				count++;
			else
				count=0;
		}
		pos++;
	}

	if(count<n)
		return 0; //error occurred or not found

	return pos;

}

