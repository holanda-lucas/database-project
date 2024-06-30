#include <stdio.h>
#include "common.h"

void print_usage(char *argv[]) {
    printf("usage: %s -n -d <database file>\n", argv[0]);
    printf("\t -n - creates a new database file\n");
    printf("\t -f - (required) path to database file");
    printf("\t -l - lists all the registries on the database");
    printf("\t -a - add a new registry with informations divided by comma");
}