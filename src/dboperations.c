#include "dboperations.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"

int add_employee(dbheader_t *dbhdr, employee_t *employees, char *addstring) {
    char *name = strtok(addstring, ",");
    char *address = strtok(NULL, ",");
    char *hours = strtok(NULL, ",");

    strncpy(employees[dbhdr->count-1].name, name, sizeof(employees[dbhdr->count-1].name));
    strncpy(employees[dbhdr->count-1].address, address, sizeof(employees[dbhdr->count-1].address));
    employees[dbhdr->count-1].hours = atoi(hours);

    return STATUS_SUCCESS;
}

void list_employees(dbheader_t *dbhdr, employee_t *employees) {
    for (int i = 0; i < dbhdr->count; i++) {
        printf("Employee %d\n", i+1);
        printf("\t Name: %s\n", employees[i].name);
        printf("\t Address: %s\n", employees[i].address);
        printf("\t Hours: %d\n", employees[i].hours);
    }
}