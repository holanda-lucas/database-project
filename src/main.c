#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <string.h>

#include "common.h"
#include "file.h"   
#include "parse.h"
#include "dboperations.h"


int main(int argc, char *argv[]) {
    char *filepath = NULL;
    char *addstring = NULL;
    bool newfile = false;
    bool list = false;
    int c;  

    int dbfd = -1;  
    dbheader_t *dbhdr = NULL;

    employee_t *employees = NULL;

    while((c = getopt(argc, argv, "nf:a:l")) != -1) {
        switch (c) {
            case 'n':
                newfile = true;
                break;
            case 'f':
                filepath = optarg;
                break;
            case 'a':
                addstring = optarg;
                break;
            case 'l':
                list = true;
                break;
            case '?':
                printf("unknown option -%c\n", c);
                break;
            default:
                return -1;
        }
    }

    if (filepath == NULL) {
        printf("filepath is a required argument\n");
        print_usage(argv);
        return 0;
    }

    if (newfile) {
        dbfd = create_db_file(filepath);
        if (dbfd == STATUS_ERROR) {
            printf("unable to create database file\n");
            return -1;
        }

        if ((create_db_header(dbfd, &dbhdr)) == STATUS_ERROR) {
            printf("failed to create database header\n");
            return -1;
        }
    } else {
        dbfd = open_db_file(filepath);
        if (dbfd == STATUS_ERROR) {
            printf("unable to open database file\n");
            return -1;
        }

        if ((validate_db_header(dbfd, &dbhdr)) == STATUS_ERROR) {
            printf("failed to validate database header\n");
            return -1;
        }
    }

    if ((read_employees(dbfd, dbhdr, &employees)) != STATUS_SUCCESS) {
        printf("failed to read employees");
        return -1;
    }

    if (addstring) {
        dbhdr->count++;
        dbhdr->filesize = (dbhdr->count * sizeof(employee_t)) + sizeof(dbheader_t);
        employees = realloc(employees, dbhdr->count*sizeof(employee_t));
        add_employee(dbhdr, employees, addstring);
    }

    if (list) {
        list_employees(dbhdr, employees);
    }


    output_file(dbfd, dbhdr, employees); // finish operations and export the new file

    free(dbhdr);
    return 0;
}