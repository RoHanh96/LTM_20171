// gcc -o main main.c `mysql_config --cflags --libs`
// mysql -u root -p
// LOAD DATA LOCAL INFILE '/home/tmd/Desktop/TEST/users.csv' INTO TABLE users FIELDS TERMINATED by ','  LINES TERMINATED BY 'n';

#include <mysql/my_global.h>
#include <mysql/mysql.h>
#include <stdio.h>
#include "database.h"

#define DB_NAME "ltm"
#define USERNAME "root"
#define PASSWORD "a9291698"

void connect_database(MYSQL **con);
MYSQL_RES *select_query(MYSQL *con, char *query);
void finish_with_error(MYSQL *con);


void menu(){
	char username[30];
	char password[30];
	printf("\n%s\n", "******************************************************");
	printf("%s", "Username: ");
	fgets(username, 30, stdin);
	username[strlen(username) - 1] = '\0';
	printf("%s", "Password: ");
	fgets(password, 30, stdin);
	password[strlen(password) - 1] = '\0';
	printf("%s\n", "******************************************************");
	printf("%s %s\n", username, password);
}

int main(int argc, char **argv)
{
	// menu();
	MYSQL *con;
	connect_database(&con);

	MYSQL_RES *result = select_query(con, "SELECT * FROM user");

	int num_fields = mysql_num_fields(result);

	MYSQL_ROW row;
	MYSQL_FIELD *field;

	while ((row = mysql_fetch_row(result))) 
	{ 
		for(int i = 0; i < num_fields; i++) 
		{ 
			if (i == 0) 
			{              
				while(field = mysql_fetch_field(result)) 
				{
					printf("%s ", field->name);
				}

				printf("\n");           
			}

			printf("%s  ", row[i] ? row[i] : "NULL"); 
		} 
	}

	printf("\n");

	mysql_free_result(result);
	mysql_close(con);

	exit(0);
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

void finish_with_error(MYSQL *con)
{
	fprintf(stderr, "%s\n", mysql_error(con));
	mysql_close(con);
	exit(1);        
}