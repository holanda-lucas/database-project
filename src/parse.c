#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/stat.h>

#include "parse.h"
#include "common.h"

int create_db_header(int fd, dbheader_t **headerOut) {
    dbheader_t *header = (dbheader_t*)calloc(1, sizeof(dbheader_t));
    if (header == NULL) {
        printf("calloc failed to create db header\n");
        return STATUS_ERROR;
    }

    header->version = 0x1;
    header->count = 0;
    header->magic = HEADER_MAGIC;
    header->filesize = sizeof(dbheader_t);

    *headerOut = header;

    return STATUS_SUCCESS;
}

int validate_db_header(int fd, dbheader_t **headerOut) {
    if (fd < 0) {
        printf("got a bad FD from user\n");
        return STATUS_ERROR;
    }

    dbheader_t *header = (dbheader_t*)calloc(1, sizeof(dbheader_t));
    if (header == NULL) {
        printf("calloc failed to create a database header\n");
        return STATUS_ERROR;
    }

    if (read(fd, header, sizeof(dbheader_t)) != sizeof(dbheader_t)) {
        perror("read");
        free(header);
        return STATUS_ERROR;
    }

    header->version = ntohs(header->version);
    header->count = ntohs(header->count);
    header->magic = ntohl(header->magic);
    header->filesize = ntohl(header->filesize);

    if (header->magic != HEADER_MAGIC) {
        printf("improper header magic\n");
        free(header);
        return STATUS_ERROR;
    }

    if (header->version != 1) {
        printf("improper header version\n");
        free(header);
        return STATUS_ERROR;
    }

    struct stat dbstat = {0};
    fstat(fd, &dbstat);

    if (header->filesize != dbstat.st_size) {
        printf("corrupted database\n");
        free(header);
        return STATUS_ERROR;
    }

    *headerOut = header;

    return STATUS_SUCCESS;
}

void output_file(int fd, dbheader_t *dbheader, employee_t *employees) {
    if (fd < 0) {
        printf("got a bad FD from user\n");
    }

    int realcount = dbheader->count;

    dbheader->magic = htonl(dbheader->magic);
    dbheader->filesize = htonl(dbheader->filesize);
    dbheader->version = htons(dbheader->version);
    dbheader->count = htons(dbheader->count);

    lseek(fd, 0, SEEK_SET);
    write(fd, dbheader, sizeof(dbheader_t));

    for (int i = 0; i < realcount; i++) {
        employees[i].hours = htonl(employees[i].hours);
        write(fd, &employees[i], sizeof(employee_t));
    }
}

int read_employees(int fd, dbheader_t *dbheader, employee_t **employeesOut) {
    if (fd < 0) {
        printf("got a bad FD from user\n");
        return STATUS_ERROR;
    }

    int count = dbheader->count;

    employee_t *employees = (employee_t*)calloc(count, sizeof(employee_t));
    if (employees == NULL) {
        printf("calloc failed");
        return STATUS_ERROR;
    }

    read(fd, employees, count * sizeof(employee_t));

    for (int i = 0; i < count; i++) {
        employees[i].hours = ntohl(employees[i].hours);
    }

    *employeesOut = employees;
    return STATUS_SUCCESS;
}