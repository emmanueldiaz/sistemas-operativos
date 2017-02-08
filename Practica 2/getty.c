#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>

#define ACCOUNTS 6

// char[] setCredentials(char *user[], char *pass[]);

int main(int argc, char *argv[])
{
    int i, j;

    FILE *f;
    size_t len;
    pid_t pid;
    int status;
    char line[80];

    bool notIdentified = true;
    while (notIdentified)
    {
        // Get user and pass
        char user[50] = {}, pass[50] = {}, credentials[100] = {};
        printf("%s\n", "userid:");
        scanf("%s", user);
        printf("%s\n", "password:");
        scanf("%s", pass);

        // Set credentials in format user:pass to be compared
        // with each row from passwd.txt
        while (user[i] != NULL)
        {
            credentials[i] = user[i];
            i++;
        }

        credentials[i] = ':';
        i++;

        while (pass[j] != NULL)
        {
            credentials[i] = pass[j];
            i++;
            j++;
        }

        /*
        * Read a file line by line.
        * http://rosettacode.org/wiki/Read_a_file_line_by_line
        */

        f = fopen("passwd.txt", "rt");
        while ((fgets(line, 80, f) != NULL))
        {
            i = 0;
            bool valid = true;

            while (line[i] != NULL && credentials[i] != NULL && valid)
            {
                //Compare each char, if diferent, go to next row
                if (credentials[i] != line[i])
                {
                    valid = false;
                }
                i++;
            }
            if (valid)
            {
                notIdentified = false;

                pid = fork();
                if (pid == 0)
                {
                    char command[] = "./sh";
                    execlp("xterm", "xterm", "-e", command, NULL);
                }
                pid = wait(&status);
                if (status == 50)
                {
                    kill(pid, SIGKILL);
                    return 50;
                }
                if (status == 15)
                {
                    kill(pid, SIGKILL);
                    return 50;
                }
                if (status == 10)
                {
                    notIdentified = true;
                    break;
                }
            }
        }
        printf("Invalid Credentials, please try again\n");
        fclose(f);
    }
}