// Alexis Giraudet
// june 2014
// gcc -std=c99

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

int width,
    height,
    n_jump,
    x_bat,
    y_bat;

void start(void);
void end(void);
int get_direction(void);
char * _getline(FILE * __stream);
int main(void);

void start(void)
{
    char * line = NULL;
    
    //init destructor
    if(atexit(end) != 0)
    {
        fprintf(stderr, "atexit: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    //read line, init width height
    line = _getline(stdin);
    if(line == NULL)
    {
        fprintf(stderr, "_getline: NULL pointer\n");
        exit(EXIT_FAILURE);
    }
    if(sscanf(line ,"%d %d", &width, &height) != 2)
    {
        fprintf(stderr, "sscanf: unexpected number of items\n");
        exit(EXIT_FAILURE);
    }
    free(line);
    line = NULL;
    
    //read line, init n_jump
    line = _getline(stdin);
    if(line == NULL)
    {
        fprintf(stderr, "_getline: NULL pointer\n");
        exit(EXIT_FAILURE);
    }
    if(sscanf(line ,"%d", &n_jump) != 1)
    {
        fprintf(stderr, "sscanf: unexpected number of items\n");
        exit(EXIT_FAILURE);
    }
    free(line);
    line = NULL;
    
    //read line, init x_bat y_bat
    line = _getline(stdin);
    if(line == NULL)
    {
        fprintf(stderr, "_getline: NULL pointer\n");
        exit(EXIT_FAILURE);
    }
    if(sscanf(line ,"%d %d", &x_bat, &y_bat) != 2)
    {
        fprintf(stderr, "sscanf: unexpected number of items\n");
        exit(EXIT_FAILURE);
    }
    free(line);
    line = NULL;
}

void end(void)
{
    ;
}

char * _getline(FILE * __stream)
{
    char * __lineptr = NULL;
    size_t __n = 0;
    ssize_t __ssize_t = getline(&__lineptr, &__n, __stream);
    if(__ssize_t < 0)
    {
        free(__lineptr);
         __lineptr = NULL;
    }
    return __lineptr;
}

int main(void)
{
    int x_min = 0,
        y_min = 0,
        x_max = 0,
        y_max = 0;
    char * direction = NULL;
    
    start();
    x_max = width-1;
    y_max = height-1;
    
    while(true)
    {
        //read direction
        direction = _getline(stdin);
        if(direction == NULL)
        {
            fprintf(stderr, "_getline: NULL pointer\n");
            exit(EXIT_FAILURE);
        }
        
        if(strlen(direction)>1)
        {
            //switch case 0
            switch(direction[0])
            {
                case 'U':
                    y_max = y_bat;
                    y_bat = y_min+(y_bat-y_min)/2;
                    break;
                case 'D':
                    y_min = y_bat;
                    y_bat = y_bat+(y_max-y_bat)/2;
                    break;
                case 'L':
                    x_max = x_bat;
                    x_bat = x_min+(x_bat-x_min)/2;
                    break;
                case 'R':
                    x_min = x_bat;
                    x_bat = x_bat+(x_max-x_bat)/2;
                    break;
                default:
                    fprintf(stderr,"main: unknow direction\n");
                    return EXIT_FAILURE;
            }
            
            if(strlen(direction)>2)
            {
                //switch case 1
                switch(direction[1])
                {
                    case 'U':
                        y_max = y_bat;
                        y_bat = y_min+(y_bat-y_min)/2;
                        break;
                    case 'D':
                        y_min = y_bat;
                        y_bat = y_bat+(y_max-y_bat)/2;
                        break;
                    case 'L':
                        x_max = x_bat;
                        x_bat = x_min+(x_bat-x_min)/2;
                        break;
                    case 'R':
                        x_min = x_bat;
                        x_bat = x_bat+(x_max-x_bat)/2;
                        break;
                    default:
                        fprintf(stderr,"main: unknow direction\n");
                        return EXIT_FAILURE;
                }
            }
        }
        
        free(direction);
        direction = NULL;
        
        fprintf(stdout, "%d %d\n", x_bat, y_bat);
    }
    return EXIT_SUCCESS;
}
