#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define _little_endian_

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;
typedef char bool;
typedef long long sgnddword;
typedef unsigned long long ddword;

#define TRUE 1
#define FALSE 0

//header bytes
#define EHDRLEN 24
#define NHDRLEN 4

/*
 --Entry Header--
 |Timestamp|inUse|dir_file|rw_ro|exec|hidden|length|nextPtr|name|
 |33       |1    |1       |1    |1   |1     |7     |19     |128 |	=192bit=24byte

 --Next Header--
 |length|nextPtr|
 |13     |19	 | =32bit =4byte


 */



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

typedef struct _nextHeader
{
	word length; /* following page can be 2^16byte length */
	dword nextPtr;
}nextHeader;

typedef struct _bitmapHeader
{
	dword length;
}bitmapHeader;

typedef struct _fsInfo
{
	char devfname[30];
	FILE *fp;
	dword size;
}fsInfo;




entryHeader readEntryHeader(const byte *buffer);
void writeEntryHeader(byte* buffer,const entryHeader header);
nextHeader readNextHeader(const byte *buffer);
void writeNextHeader(byte* buffer,const nextHeader header);


entryHeader readEntryHeader(const byte *buffer)
{
	entryHeader header;

	header.timestmp=((sgnddword)(buffer[0]))<<25	| ((sgnddword)(buffer[1]))<<17	| ((sgnddword)(buffer[2]))<<9		| ((sgnddword)(buffer[3]))<<1 | ((sgnddword)(buffer[4] & 0b10000000))>>7;
	header.inUse=(buffer[4]								& 0b01000000)>>6;
	header.dir_file=(buffer[4]							& 0b00100000)>5;
	header.rw_ro=(buffer[4]								& 0b00010000)>>4;
	header.exec=(buffer[4]								& 0b00001000)>>3;
	header.hidden=(buffer[5]							& 0b00000100)>>2;
	header.length=(buffer[4] 							& 0b00000011)<<5	 					| (buffer[5] & 0b11111000)>>3;
	header.nextPtr=((dword)(buffer[5] 				& 0b00000111))<<16	 				| ((dword)(buffer[6]))<<8			| ((dword)(buffer[7]));
	memcpy(header.name,buffer+8,16);
	header.name[16]='\0';

	return header;
}

void writeEntryHeader(byte* buffer,const entryHeader header)
{
	buffer[0]=((byte)(header.timestmp>>25));
	buffer[1]=((byte)((header.timestmp & (ddword)0xffffffffff800000) >>17));
	buffer[2]=((byte)((header.timestmp & (ddword)0xffffffffffff8000) >>9));
	buffer[3]=((byte)((header.timestmp & (ddword)0xffffffffffffffff)>>1));
	buffer[4]=((byte)((header.timestmp & (ddword)0x0000000000000001)<<8));
	buffer[4]|=header.inUse<<6;
	buffer[4]|=header.dir_file<<5;
	buffer[4]|=header.rw_ro<<4;
	buffer[4]|=header.exec<<3;
	buffer[4]|=header.hidden<<2;
	buffer[4]|=header.length>>5;
	buffer[5]=(header.length & 0b00011111)<<3;
	buffer[5]|=((byte)(header.nextPtr >> 16));
	buffer[6]=((byte)((header.nextPtr & (word)0x0000ffff) >>8));
	buffer[7]=((byte)((header.nextPtr & (word)0x000000ff)));
	memcpy(buffer+8,header.name,16);
}

nextHeader readNextHeader(const byte *buffer)
{
	nextHeader header;
	header.length=((word)(buffer[1])>>3) | ((word)(buffer[0])<<5);
	header.nextPtr=((dword)(buffer[3])) | ((dword)(buffer[2])<<8) | ((dword)(buffer[1] & 0b00000111)<<16);

	return header;
}

void writeNextHeader(byte* buffer,const nextHeader header)
{
	buffer[0]=(byte)(header.length >>5);
	buffer[1]=(byte)((header.length & 0x1f)<<3);
	buffer[1]|=(byte)(header.nextPtr >> 16);
	buffer[2]=(byte)((header.nextPtr & 0x70000)>>8);
	buffer[3]=(byte)(header.nextPtr & 0xff);
}

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

void format(fsInfo fs)
{
	writeBitmap(fs);
}

dword getFileSize(char *fname)
{
	FILE *fp = fopen(fname, "rb");
	long size;

	fseek(fp, 0, SEEK_END);
	size=ftell(fp);

	fclose(fp);
	return size;
}







int main(int argc, char **argv)
{
	fsInfo fs;
	strncpy(fs.devfname,argv[1],30);
	fs.size=getFileSize(fs.devfname);

	fs.fp=fopen(fs.devfname,"r+b");

	char c=0;
	while (c!='q')
	{
		puts(	"1. format disk \n"
				"2. ls \n"
				"3. cd"
				"4. new file\n"
				"5. rm file\n");

		switch(c=getchar())
		{
			case '1':
					format(fs);
					break;
			case '2':
					break;


		}
	}

	fclose(fs.fp);


	return EXIT_SUCCESS;
}

