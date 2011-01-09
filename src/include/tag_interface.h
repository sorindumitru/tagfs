/***************
	TagFs project
	Tag.h: implementation of tag command in userspace
	File Main Owner: Eugen Hristev
***************/
#ifndef TAG_INTERFACE_H
#define TAG_INTERFACE_H

#include <stdio.h>
#include <errno.h>


#define MAX_TAG_COUNT 256
#define MAX_TAG_LENGTH 16

#define F_TAG_ADD   17 /*TagFS*/
#define F_TAG_DEL   18
#define F_TAG_LIST  19
#define F_TAG_CREAT_EP 20

struct tagfs_ops{
    unsigned str_len;
    char *str;
};

struct tagfs_list{
    unsigned fname_size;
    char *filename;
    char *tags;
};

/*
Returns an array of tags for a specific file.
Tag_count  parameter is being set to the number of tags by called.
*/
char ** get_tags(char *filename /* directory ? */, int *tag_count)
{
    int fd = open("/tmp/tagfs",O_CREAT);
    struct tagfs_list arg;
    arg.fname_size = strlen(filename);
    arg.filename = calloc(strlen(filename),sizeof(char));
    memcpy(arg.filename,filename,strlen(filename));
    arg.tags = calloc(MAX_TAG_COUNT * MAX_TAG_LENGTH,sizeof(char));
     
    int count = fcntl(fd, F_TAG_LIST,&arg);
    if ( count == -1 ) { /*TODO: see how arg should look like*/
        perror("Error occured at get_tafs");
        close(fd);
        return NULL;
    }
    *tag_count = count;
    close(fd);
    //tags_matrix
    {
        char **matrix = calloc(count, sizeof(char*));
        char *ctag = strtok(arg.tags,":");
        int i=0;
        while (ctag != NULL){
            matrix[i] = calloc(strlen(ctag),sizeof(char));
            memcpy(matrix[i],ctag,strlen(ctag));
            i++;
            ctag = strtok(NULL, ":");
        }
    free(arg.filename);
    free(arg.tags);
    return matrix;
    }
}

/*
Adds the given tag as a tag for specified file
Returns 1 if failure, 0 if success.
*/
int add_tag_to_file(char *str)
{
    int fd = open("/tmp/tagfs",O_CREAT);
    struct tagfs_ops arg;
    arg.str_len = strlen(str);
    arg.str = calloc(strlen(str),sizeof(char));
    memcpy(arg.str,str,strlen(str));
    if ( fcntl(fd, F_TAG_ADD,&arg) == -1 ) {
        close(fd);
        return 1;
    }
    close(fd);
    free(arg.str);
    return 0;
}

/*
Removes the given tag as a tag for specified file
Returns 1 if failure, 0 if success.
*/
int remove_tag_from_file(char *str)
{
    int fd = open("/tmp/tagfs",O_CREAT);
    struct tagfs_ops arg;
    arg.str_len = strlen(str);
    arg.str = calloc(strlen(str),sizeof(char));
    memcpy(arg.str,str,strlen(str));
    if ( fcntl(fd, F_TAG_DEL,&arg) ==-1 ) {
        close(fd);
        return 1;
    }
    close(fd);
    free(arg.str);
    return 0;
}

int create_tag_entry()
{
    int fd = open("/tmp/tagfs",O_CREAT);
    if ( fcntl(fd, F_TAG_CREAT_EP) ==-1 ) { 
        close(fd);
        return 1;
    }
    close(fd);
    return 0;
}
#endif
