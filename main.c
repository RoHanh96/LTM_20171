// gcc -o main main.c `mysql_config --cflags --libs`
// mysql -u root -p
// LOAD DATA LOCAL INFILE '/home/tmd/Desktop/TEST/users.csv' INTO TABLE users FIELDS TERMINATED by ','  LINES TERMINATED BY 'n';

#include <mysql/my_global.h>
#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "database.h"

#define DB_NAME "ltm"
#define USERNAME "root"
#define PASSWORD "a9291698"
#define USER_ID_INDEX 0
#define USER_USERNAME_INDEX 1
#define USER_PASSWORD_INDEX 2
#define USER_ROLE_INDEX 3

int login(MYSQL *con);
void logged(MYSQL *con, MYSQL_ROW current_user);
void menu(char *username, char *password);
void connect_database(MYSQL **con);
char *concat_query(int arg_count, ...);
MYSQL_RES *select_query(MYSQL *con, char *query);
void showSelectQuery(MYSQL_RES *result);
void print_column_name(MYSQL_RES *result);
MYSQL_ROW get_row(MYSQL_RES *result);
void print_row(MYSQL_ROW row, int num_fields);
void finish_with_error(MYSQL *con);

void sign_up(MYSQL *con){
	char username[30];
	char password[30];
	char query[1024];
	menu(username, password);
	sprintf(query, "INSERT INTO user (username, password, role) VALUES (\'%s\',\'%s\', \'student\')", username, password);
	if (mysql_query(con, query))
	{  
		finish_with_error(con);
	}
	printf("Student: \"%s\" created\n", username);
}

int main(int argc, char **argv)
{
	MYSQL *con;
	connect_database(&con); 
	if (login(con) == FALSE)
	{
		printf("%s\n", "Sign up");
		sign_up(con);
	}
	mysql_close(con);
	exit(0);
}

int login(MYSQL *con) {
	char username[30];
	char password[30];
	char query[1024];
	menu(username, password);
	sprintf(query, "SELECT * FROM user WHERE username = \'%s\' AND password = \'%s\'", username, password);
	MYSQL_RES *result = select_query(con, query);
	if (mysql_num_rows(result) == 1)
	{
		// showSelectQuery(result);
		mysql_free_result(result);
		logged(con, get_row(result));
		return TRUE;
	}
	mysql_free_result(result);
	return FALSE;
}

void logged(MYSQL *con, MYSQL_ROW current_user) {
	printf("%s\n", current_user[USER_USERNAME_INDEX]);
	printf("%s\n", current_user[USER_PASSWORD_INDEX]);
	if(strcmp(current_user[USER_ROLE_INDEX], "student"))
	{
			//
	}
	else
	{
			//
	}
}


void menu(char *username, char *password){
	printf("\n%s\n", "******************************************************");
	printf("%s", "Username: ");
	fgets(username, 30, stdin);
	username[strlen(username) - 1] = '\0';
	printf("%s", "Password: ");
	fgets(password, 30, stdin);
	password[strlen(password) - 1] = '\0';
	printf("%s\n", "******************************************************");
	
}

void connect_database(MYSQL **con){
	*con  = mysql_init(NULL);

	if (*con == NULL) 
	{
		fprintf(stderr, "%s\n", mysql_error(*con));
		exit(1);
	}  

	if (mysql_real_connect(*con, "localhost", USERNAME, PASSWORD, DB_NAME, 0, NULL, 0) == NULL) 
	{
		finish_with_error(*con);
	}    
}

char *concat_query(int arg_count, ...){
/*
	thua, dung sprint nhanh hon
	MYSQL_RES *result = select_query(con, concat_query(5, "SELECT * FROM user WHERE username = \'", username, "\' AND password = \'", password, "\'"));
*/
	char *concat_query = (char *) malloc(sizeof(char) *1024);
	concat_query[0] = '\0';
	va_list list;
	va_start(list, arg_count); 
	for (int i = 0; i < arg_count; ++i)
	{
		strcat(concat_query, va_arg(list, char *));
	}
	va_end(list);
	// printf("%s\n", concat_query); 
	return concat_query;
}

MYSQL_RES *select_query(MYSQL *con, char *query){
	if (mysql_query(con, query))
	{  
		finish_with_error(con);
	}
	MYSQL_RES *result = mysql_store_result(con);
	if (result == NULL) 
	{
		finish_with_error(con);
	}  
	return result;
}

void print_column_name(MYSQL_RES *result){
	MYSQL_FIELD *field;
	while(field = mysql_fetch_field(result)) 
	{
		printf("%s ", field->name);
	}
	printf("\n"); 
}

MYSQL_ROW get_row(MYSQL_RES *result){
	return mysql_fetch_row(result);
}

void print_row(MYSQL_ROW row, int num_fields){
	for(int i = 0; i < num_fields; i++) 
	{ 
		printf("%s  ", row[i] ? row[i] : "NULL"); 
	} 
	printf("\n");  
}

void showSelectQuery(MYSQL_RES *result){
	MYSQL_ROW row;
	if (mysql_num_rows(result) > 0)
	{
		print_column_name(result);
	}
	int num_fields = mysql_num_fields(result);
	for (int i = 0; row = mysql_fetch_row(result); i++)
	{ 
		print_row(row, num_fields);
	}
}

void finish_with_error(MYSQL *con)
{
	fprintf(stderr, "%s\n", mysql_error(con));
	mysql_close(con);
	exit(1);        
}