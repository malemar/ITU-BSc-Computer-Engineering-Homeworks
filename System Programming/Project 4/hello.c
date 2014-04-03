/*
    FUSE: Filesystem in Userspace
    Copyright (C) 2001-2005  Miklos Szeredi <miklos@szeredi.hu>

    This program can be distributed under the terms of the GNU GPL.
    See the file COPYING.
*/

#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <libgen.h>
#include <stdlib.h>
#include <signal.h>
#include <ctype.h>

static const char *hello_str = "Hello World!\n";
static const char *hello_path = "/hello";
static const char *fatih_path = "/fatih";	

static char cmdline[100] = "";


int isNum(const char* n);

/*char* getParent(char* c)
{
	
	if(atol(c) != 0)
	{
		FILE *fp;
		char path[100] = "/proc/";
		strcat(path,c);
		strcat(path,"/status");
		fp = fopen(path,"r");
		char fileName[100] ="";
		if(fp != NULL)
		{
			char temp[100];
			char parent[10];
			fgets(temp, 99, fp);
			fgets(temp, 99, fp);
			fgets(temp, 99, fp);
			fgets(temp, 99, fp);
			fscanf(fp, "%s", temp);
			fscanf(fp, "%s", parent);
			return parent;
			//printf("parent : %s\n", parent);
		}
	}
}*/


static int hello_getattr(const char *path, struct stat *stbuf)
{
    int res = 0;
	
    memset(stbuf, 0, sizeof(struct stat));
    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    }
	else if(isNum(basename(path)) == 0){
		stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
	}
	
	else 
	{
		stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = 1024;
	}
    /*else
        res = -ENOENT;*/
        
    return res;
}

static int hello_write(const char* path, char *buf, size_t size, off_t offset,
						struct fuse_file_info *fi)
{
	int fd, res;
	fd = open(path, O_WRONLY);
	if(fd == -1)	return -errno;
	
	res =pwrite(fd, buf, size, offset);
	if( res == -1)	res = -errno;
	
	close(fd);
	return res;
}

static int hello_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{
    (void) offset;
    (void) fi;
    FILE *fp;
	
	/* to avoid warnings caused by basename() */
	char *pathh;
	char *dir_name;
	dir_name = (char *)malloc(100);
	pathh = (char *)malloc(100);
	
	strcpy(pathh,path); 
	dir_name = basename(pathh);
				
				
	if (strcmp(path, "/") != 0 && atol(dir_name) != 0)
		filler(buf,"info.txt",NULL,0);
	DIR *dp;
	struct dirent *dirp;
	if((dp  = opendir("/proc")) == NULL) {
		printf("%d\n",errno );
		return errno;
	}
	if(strstr(path,"info.txt") == NULL)
	{
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);	
	}
		
	
	while ((dirp = readdir(dp)) != NULL) {
		if(atol(dirp->d_name) != 0)
		{
			/* get parent id */
			char procPath[100] = "/proc/";
			strcat(procPath,dirp->d_name);
			strcat(procPath,"/status");
			fp = fopen(procPath,"r");
			char temp[100];
			char parent[10];
			if(fp != NULL)
			{
				fgets(temp, 99, fp);
				fgets(temp, 99, fp);
				fgets(temp, 99, fp);
				fgets(temp, 99, fp);
				fscanf(fp, "%s %s", temp, parent);
			}
			else 
			{
				filler(buf, "fp equ NULL", NULL, 0);
				break;
			}
			fclose(fp);
			
			/* get cmdline */
			procPath[0] = '\0';
			strcat(procPath, "/proc/");
			strcat(procPath, dirp->d_name);
			strcat(procPath, "/cmdline");
			fp = fopen(procPath, "r");
			if(fp != NULL){ 
				fgets(cmdline, 99, fp);
			}
			fclose(fp);
			
			/* construct file path */
			pathh[0] = '\0';
			strcpy(pathh, path);
			strcat(pathh, "/info.txt");
			//filler(buf, pathh, NULL, 0);
			
			hello_write(pathh, cmdline, strlen(cmdline), 0, fi);
			
			
			/* add the process folders */
			if (strcmp(path, "/") != 0)
			{
				if(strtol(parent, NULL, 0) != NULL && strtol(parent, NULL, 0) == strtol(dir_name, NULL, 0))
				{
					filler(buf, dirp->d_name , NULL, 0);
				}
			}
			else
			{
				if(strtol(parent, NULL, 0) == 0)
				{
					filler(buf, dirp->d_name , NULL, 0);
				}
			}
			
		}
	}
	return 0;
	
}


static int hello_open(const char *path, struct fuse_file_info *fi)
{
    /*if (strcmp(path, hello_path) != 0 )
    {
		if (strcmp(path, fatih_path) != 0 )
			return -ENOENT;
	}*/

    if ((fi->flags & 3) != O_RDONLY)
        return -EACCES;

    return 0;
}

static int hello_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi)
{
    /*(void) fi;
    if (offset < len) {
		if (offset + size > len)
			size = len - offset;
		memcpy(buf, offset, size);
	} else
		size = 0;
    
    return size;*/
    
    int fd, res;
    
    fd = open(path, O_RDONLY);
    if(fd == -1)	return -errno;
    
    res = pread(fd, buf, size, offset);
    if(res == -1)	res = -errno;
    
    close(fd);
    return res;
}

static int hello_rmdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{	
	char* _path = (char*)malloc(sizeof(char)*strlen(path+1));
	char name[10] = "";
	strcat(name, basename(_path));
	//free(_path);
	return kill(atoi(name), SIGTERM);
}



static struct fuse_operations hello_oper = {
    .getattr	= hello_getattr,
    .readdir	= hello_readdir,
    .open	= hello_open,
    .read	= hello_read,
    .rmdir = hello_rmdir,
    .write = hello_write
};

int main(int argc, char *argv[])
{
    return fuse_main(argc, argv, &hello_oper, NULL);
}

int isNum(const char* n){
	int i = 0;
	while(n[i] != '\0'){
		if(n[i] < '0' || n[i] > '9')
			return -1;
		++i;
	}
	return 0;
}
