/***************
	TagFs project
	Tag.c: implementation of tag command in userspace
	File Main Owner: Eugen Hristev
***************/

#include "stdio.h"
#include "string.h"

#include "../include/tag_interface.h"

int main(int argc, char * argv[])
{
	if( argc < 3)
	{
		printf("Usage: tag (-a/-d/-l) filename [tag]\n");
		return 1;
	}
	FILE *infile = fopen(argv[3],"rt");
		
	if(!infile)
	{
		printf("Invalid file !\n");
		return 1;
	}
	if(!strcmp(argv[1],"-l"))
	{
		char tags[MAX_TAG_COUNT][MAX_TAG_LENGTH];
		char **tagsp=(char **)tags;
		int count,i;
		
		printf("Tags :\n");
		tagsp = get_tags(argv[3],&count);
		for(i=0;i<count;i++)
			printf("%s\n",tags[i]);
			
	}
	
	if(!strcmp(argv[1],"-a"))
	{
		if(argc<4)
		{
			printf("No tag specified !\n");
			return 1;
		}
		if( add_tag_to_file(argv[3], argv[4]))
			printf("Successfully added tag to file !\n");
		else 
			printf("Could not add tag to file !\n");
	}
	
	if(!strcmp(argv[1],"-d"))
	{
		if(argc<4)
		{
			printf("No tag specified !\n");
			return 1;
		}
		if( remove_tag_from_file(argv[3], argv[4]))
			printf("Successfully deleted tag from file !\n");
		else 
			printf("Could not deelte tag from file !\n");
	}
	
		


	return 0;
}