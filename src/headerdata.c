#include "headerdata.h"

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
