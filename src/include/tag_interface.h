/***************
	TagFs project
	Tag.h: implementation of tag command in userspace
	File Main Owner: Eugen Hristev
***************/
#ifndef TAG_INTERFACE_H
#define TAG_INTERFACE_H


#define MAX_TAG_COUNT 256
#define MAX_TAG_LENGTH 245
/*
Returns an array of tags for a specific file.
Tag_count  parameter is being set to the number of tags by called.
*/
char ** get_tags(char *filename /* directory ? */, int *tag_count);
/*
Adds the given tag as a tag for specified file
Returns 1 if failure, 0 if success.
*/
int add_tag_to_file(char *filename /*directory?*/, char * tag);
/*
Removes the given tag as a tag for specified file
Returns 1 if failure, 0 if success.
*/
int remove_tag_from_file(char *filename /*directory?*/, char * tag);

#endif