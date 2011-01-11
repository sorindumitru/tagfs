/***************
	TagFs project
	Tag.c: implementation of tag command in userspace
	File Main Owner: Eugen Hristev
***************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../include/tag_interface.h"

//new commands
#define COMMANDS    4
char *args[] = {"-c","-a","-d","-l"}; 

int invalid_arg( char *cmd )
{
    int i;
    for ( i=0; i < COMMANDS; i++)
        if( !strcmp(cmd,args[i]) )
            return 0;
    printf("invalid arg=%s\n",cmd);
    return 1;
}


int main(int argc, char * argv[])
{
	if( argc < 2 || argc > 3 || invalid_arg(argv[1]) )
	{
		printf("Usage: tag (-a/-d/-l) filename[:tag1:...:tagn]\nOR\ntag -c\n");
		printf("argc = %d\n",argc);
        return 1;
	}

    if ( argc == 3 && strcmp(argv[1],"-c") )
	{
        
        //filename existence check
        char *arg = calloc(strlen(argv[2]), sizeof(char));
        memcpy(arg,argv[2],strlen(argv[2]));
        char *filename = strtok(arg,":");
        FILE *infile = fopen(filename,"rt");
                
        if(!infile)
        {
            printf("Invalid file !\n");
            return 1;
        }
        free(arg);

        if(!strcmp(argv[1],"-l"))
        {
            if ( strchr(argv[2],':') != NULL ){
                printf("invalid argument at %s ,not expected <:>",argv[1]);
                return 1;
            }

            char **tags;
            int count,i;
            
            printf("Tags :\n");
            tags = get_tags(argv[2],&count);
            if (tags == NULL){
                printf("Could not get taglist");
                return 1;
            }
            for(i=0;i<count;i++)
                printf("%s\n",tags[i]);
                
            for(i=0;i<count;i++)
                free(tags[i]);
            free(tags);
        }
        
        if(!strcmp(argv[1],"-a"))
        {
            if( strchr(argv[2],':') == NULL )
            {
                printf("No tag specified !\n");
                return 1;
            }
            if( !add_tag_to_file(argv[2]) )
                printf("Successfully added tag to file !\n");
            else 
                printf("Could not add tag to file !\n");
        }
        
        if(!strcmp(argv[1],"-d"))
        {
            if( strchr(argv[2],':') == NULL )
            {
                printf("No tag specified !\n");
                return 1;
            }

            if( !remove_tag_from_file(argv[2]))
                printf("Successfully deleted tag from file !\n");
            else 
                printf("Could not delete tag from file !\n");
        }
	}
	else if ( strcmp(argv[1],"-c") )
	{
		printf("Usage: tag (-a/-d/-l) filename[:tag1:...:tagn]\n OR\n tag -c\n");
		return 1;
    }
	else
	{
	
        //create start point
        if ( !create_tag_entry() )
           printf("Created new entry point for TagFS\n");
        else
           printf("Could not create new entry point for TagFS\n"); 
    }


	return 0;
}
