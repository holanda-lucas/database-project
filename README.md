# Local Database Archive

This is a fast, secure, and easy way to store information locally using a command-line tool.

### Getting Started:

Clone the repository and run `make` to compile it on your system. By default, the data you can store is an employee structure, but you can change it in the **parse.h** header file. Here is a list of flags you can use:

- `-n` Creates a new database file.
- `-f <filename>` Specifies the database path (required).
- `-l` Lists all the registries in the database.
- `-a <information,goes,like,this>` Creates a new registry.
