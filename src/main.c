#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filesystem.h"

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
		puts("1. format partition \n"
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

