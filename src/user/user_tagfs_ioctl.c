/*
 * SO2 Lab - Linux device drivers (#4)
 * User-space test file
 */

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <sys/ioctl.h>
#include "tagfs_ioctl.h"
//#include "../include/so2_cdev.h"

#define DEVICE_PATH	"/dev/tagfs_ioctl"

/*
 * prints error message and exits
 */

static void error(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

/*
 * print use case
 */

static void usage(const char *argv0)
{
	printf("Usage: %s command\n commands:\n"
			"\t touch <filename> [<tag1/tag2.../tagn>]\n"
			"\t add-tag <filename> <tag1/tag2.../tagn>\n"
			"\t remove-tag <filename> <tag1/tag2.../tagn>\n"
			"\t find <tag1/tag2.../tagn>\n"
			"\t rm <filename> \n", argv0);
	exit(EXIT_FAILURE);
}

void data_to_be_sent(char *argv[], struct ioctl_data *args, int what)
{
    memset(args,0,sizeof(struct ioctl_data));
    
    switch(what){
        case (ALL):
            args->s_filename = strlen(argv[2]);
            args->file_name = malloc(args->s_filename * sizeof(char));
            memcpy(args->file_name,argv[2],args->s_filename * sizeof(char));
            if( argv[3] == NULL && strcmp(argv[1],"touch")==0 ){
                args->s_tags = 0;
            }else{
                args->s_tags = strlen(argv[3]);
                args->tags = malloc(args->s_tags * sizeof(char));
                memcpy(args->tags,argv[3],args->s_tags * sizeof(char));
            }
            
            break;
        
        case(TAGS):
            args->s_tags = strlen(argv[2]);
            args->tags = malloc(args->s_tags * sizeof(char));
            memcpy(args->tags,argv[2],args->s_tags * sizeof(char));

            break;
        
        case (FILENAME):
            args->s_filename = strlen(argv[2]);
            args->file_name = malloc(args->s_filename * sizeof(char));
            memcpy(args->file_name,argv[2],args->s_filename * sizeof(char));

            break;
    }
}

int main(int argc, char** argv)
{
	int fd;

	if (argc < 2)
		usage(argv[0]);

	fd = open(DEVICE_PATH, O_RDONLY);
	if (fd < 0) {
		perror("open");
		exit(EXIT_FAILURE);
	}
    
    struct ioctl_data args;

    if ((strcmp(argv[1],"touch") == 0) && (argc == 4 || argc == 3) ){
        data_to_be_sent(argv,&args,ALL);
        printf("Touch\n");
        print_data(&args,ALL);
        
        if (ioctl(fd, TAGFS_TOUCH, &args) < 0) {
			perror("ioctl");
			exit(EXIT_FAILURE);
		}

    } else if ( strcmp(argv[1],"add-tag") == 0 && (argc == 4) ) {
        data_to_be_sent(argv,&args,ALL);
        printf("add-tag\n");
        print_data(&args,ALL);
        
        if (ioctl(fd, TAGFS_ADD_TAG, &args) < 0) {
			perror("ioctl");
			exit(EXIT_FAILURE);
		}
    } else if (strcmp(argv[1],"remove-tag") == 0 && (argc == 4) ) {
        data_to_be_sent(argv,&args,ALL);
        printf("remove-tag\n");
        print_data(&args,ALL);
        
        if (ioctl(fd, TAGFS_REMOVE_TAG, &args) < 0) {
			perror("ioctl");
			exit(EXIT_FAILURE);
		}

    } else if (strcmp(argv[1],"find") == 0 && (argc == 3) ) {
        data_to_be_sent(argv,&args,TAGS);
        printf("find\n");
        print_data(&args,TAGS);
        
        if (ioctl(fd, TAGFS_FIND_TAGS, &args) < 0) {
			perror("ioctl");
			exit(EXIT_FAILURE);
		}

    } else if (strcmp(argv[1],"rm") == 0 && (argc == 3) ) {
        data_to_be_sent(argv,&args,FILENAME);
        printf("rm\n");
        print_data(&args,FILENAME);
        
        if (ioctl(fd, TAGFS_RM, &args) < 0) {
			perror("ioctl");
			exit(EXIT_FAILURE);
		}
    } else{
        printf("unknown\n");
        usage(argv[0]);
    }

		close(fd);

	return 0;
}
