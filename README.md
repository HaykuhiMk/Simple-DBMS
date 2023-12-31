# Simple DBMS
This is a simple Database Management System (DBMS) project implemented in C++. The project provides basic functionalities to create
tables, insert data, delete data, and show existing tables.
## How to Use
1. Clone the repository:
#### git clone https://github.com/your-username/dbms-project.git
2. Compile the source code:
#### g++ -o dbms dbms.cpp main.cpp
3. Run the executable:
#### ./dbms

## Project Structure
dbms.h: Header file containing the declaration of the DBMS class.

dbms.cpp: Source file containing the implementation of the DBMS class methods.

main.cpp: Source file containing the main() function to run the DBMS.

## Available Commands
#### create table tableName { columnName1, columnName2, ... }: 
Create a new table with the specified tableName and columns listed within the curly braces.

#### insert into tableName { data1, data2, ... }: 
Insert data into the table with the specified tableName.

#### delete from tableName { condition }: 
Delete data from the table with the specified tableName based on the specified condition.

#### exit: 
Exit the DBMS.

## Important Notes
1. Each table is stored in a text file named tableName.txt, where tableName is the name of the table.

2. The metadata containing the list of tables and their column names is stored in the meta.txt file.

3. The data should be provided within curly braces when using the insert and delete commands.
