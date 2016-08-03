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

typedef void fileList; /* TO DO: Write a list for file display */


void format(fsInfo fs);
fileList* listfile(char *dirpath); /*TO DO*/
bool rmfile(char* filepath); /*TO DO*/
bool mkdir(char *dirpath); /*TO DO*/
bool mvfile(char *oldpath, char* newpath); /*TO DO*/
bool mvdir(char *oldpath, char* newpath); /*TO DO*/




#endif
