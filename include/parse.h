#ifndef PARSE_H
#define PARSE_H

#define HEADER_MAGIC 0x484f4c

typedef struct {
    unsigned int magic;
    unsigned short version;
    unsigned short count;
    unsigned int filesize;
} dbheader_t;

typedef struct {
    char name[256];
    char address[256];
    unsigned int hours;
} employee_t;


int create_db_header(int fd, dbheader_t **headerOut);
int validate_db_header(int fd, dbheader_t **headerOut);
int read_employees(int fd, dbheader_t *dbheader, employee_t **employeesOut);
void output_file(int fd, dbheader_t *dbheader, employee_t *employees);

#endif