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
bool rmdir(char* filepath); /*TO DO*/
bool mkdir(char *dirpath); /*TO DO*/
bool mvfile(char *oldpath, char* newpath); /*TO DO*/
bool mvdir(char *oldpath, char* newpath); /*TO DO*/


/* Fuse driver needs to implement all thats operations: (TO DO LATER)
 *
 struct fuse_operations
 {
    int (*getattr) (const char *, struct stat *);
    int (*readlink) (const char *, char *, size_t);
    int (*getdir) (const char *, fuse_dirh_t, fuse_dirfil_t);
    int (*mknod) (const char *, mode_t, dev_t);
    int (*mkdir) (const char *, mode_t);
    int (*unlink) (const char *);
    int (*rmdir) (const char *);
    int (*symlink) (const char *, const char *);
    int (*rename) (const char *, const char *);
    int (*link) (const char *, const char *);
    int (*chmod) (const char *, mode_t);
    int (*chown) (const char *, uid_t, gid_t);
    int (*truncate) (const char *, off_t);
    int (*utime) (const char *, struct utimbuf *);
    int (*open) (const char *, struct fuse_file_info *);
    int (*read) (const char *, char *, size_t, off_t, struct fuse_file_info *);
    int (*write) (const char *, const char *, size_t, off_t,struct fuse_file_info *);
    int (*statfs) (const char *, struct statfs *);
    int (*flush) (const char *, struct fuse_file_info *);
    int (*release) (const char *, struct fuse_file_info *);
    int (*fsync) (const char *, int, struct fuse_file_info *);
    int (*setxattr) (const char *, const char *, const char *, size_t, int);
    int (*getxattr) (const char *, const char *, char *, size_t);
    int (*listxattr) (const char *, char *, size_t);
    int (*removexattr) (const char *, const char *);
};*/




#endif
