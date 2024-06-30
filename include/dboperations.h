#ifndef DBOPERATIONS_H
#define DBOPERATIONS_H

#include "parse.h"

int add_employee(dbheader_t *dbhdr, employee_t *employees, char *addstring);
void list_employees(dbheader_t *dbhdr, employee_t *employees);

#endif