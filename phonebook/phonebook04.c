#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CAPACITY 100
#define BUFFER_LENTH 100

typedef struct person
{
    char *name;
    char *number;
    char *email;
    char *group;
} Person;

Person directory[CAPACITY];
int n = 0;

void print_person();
void handle_add();
void delete();
void status();
void load();
void save();
void find();
void add();
int read_line();
int compose_name();
int search();

int read_line(FILE *fp, char str[], int n) {
    int ch, i = 0;
    while ((ch = fgetc(fp)) != '\n' && ch != EOF)
        if (i < n)
            str[i++] = ch;
    str[i] = '\0';
    return i;
}

int main() {
    char command_line[BUFFER_LENTH];
    char *command, *argument;
    char name_str[BUFFER_LENTH];

    while (1) {
        printf("$ ");
        if (read_line(stdin, command_line, BUFFER_LENTH) <= 0)
            continue;

        command = strtok(command_line, " ");

        if (strcmp(command, "read") == 0) {
            argument = strtok(NULL, " ");
            if (argument == NULL) {
                printf("File name required.\n");
                continue;
            }
            load(argument);
        }
        else if (strcmp(command, "add") == 0) {
            if (compose_name(name_str, BUFFER_LENTH) <= 0) {
                printf("Name required.\n");
                continue;
            }
            handle_add(name_str);
        }
        else if (strcmp(command, "find") == 0) {
            if (compose_name(name_str, BUFFER_LENTH) <= 0) {
                printf("Name required.\n");
                continue;
            }
            find(name_str);
        }
        else if (strcmp(command, "status") == 0) {
            status();
        }
        else if (strcmp(command, "delete") == 0) {
            if (compose_name(name_str, BUFFER_LENTH) <= 0) {
                printf("Invalid name.\n");
                continue;
            }
            delete(name_str);
        }
        else if (strcmp(command, "save") == 0) {
            argument = strtok(NULL, " ");
            if (strcmp("as", argument) != 0) {
                printf("Invalid command format.\n");
                continue;
            }
            save(argument);
        }
        else if (strcmp(command, "exit") == 0) 
            break;
    }
    return 0;
}

int compose_name(char str[], int limit) {
    char *ptr;
    int length = 0;

    ptr = strtok(NULL, " ");
    if (ptr == NULL)
        return 0;

    strcpy(str, ptr);
    length += strlen(ptr);

    while ((ptr = strtok(NULL, " ")) != NULL) {
        if ((length + strlen(ptr) + 1) < limit + 1) {
            str[length++] = ' ';
            str[length] = '\0';
            strcat(str, ptr);
            length += strlen(ptr);
        }
    }
    return length;
}

void load(char *fileName) {
    char buffer[BUFFER_LENTH];
    char *name, *number, *email, *group;

    FILE *fp = fopen(fileName, "r");
    if (fp==NULL) {
        printf("File open failed.\n");
        return;
    }
    while (1) {
        if (read_line(fp, buffer, BUFFER_LENTH) <= 0)
            break;
        name = strtok(buffer, "#");
        number = strtok(buffer, "#");
        email = strtok(buffer, "#");
        group = strtok(buffer, "#");
        add(name, number, email, group);
    }
    fclose(fp);
}

void save(char *fileName) {
    int i;
    FILE *fp = fopen(fileName, "w");
    if (fp==NULL) {
        printf("Open failed.\n");
        return;
    }
    for (i=0; i<n; i++) {
        fprintf(fp, "%s#", directory[i].name);
        fprintf(fp, "%s#", directory[i].number);
        fprintf(fp, "%s#", directory[i].email);
        fprintf(fp, "%s#\n", directory[i].group);

    }
    fclose(fp);
}

int search(char *name) {
    int i;
    for (i=0; i<n; i++) {
        if (strcmp(name, directory[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

void print_person(Person p) {
    printf("%s: \n", p.name);
    printf("     Phone: %s\n", p.number);
    printf("     Email: %s\n", p.email);
    printf("     Group: %s\n", p.group);
}

void delete(char *name) {
    int index = search(name);
    if (index == -1) {
        printf("No person named '%s' exists.\n", name);
        return;
    }

    int j = index;
    for (; j<n-1; j++) {
        directory[j] = directory[j+1];
    }
    n--;
    printf("'%s' was deleted successfully.\n", name);
}

void status() {
    int i;
    for (i=0; i<n; i++)
        print_person(directory[i]);
    printf("Total %d people.\n", n);
}

void find(char *name) {
    int index = search(name);
    if (index==-1) 
        printf("No person named '%s' exists.\n", name);
    else
        print_person(directory[index]);
}

void handle_add(char *name) {
    char number[BUFFER_LENTH], email[BUFFER_LENTH], group[BUFFER_LENTH];
    char empty[] = " ";

    printf("    Phone:    ");
    read_line(stdin, number, BUFFER_LENTH);

    printf("    Email:    ");
    read_line(stdin, email, BUFFER_LENTH);

    printf("    Group:    ");
    read_line(stdin, group, BUFFER_LENTH);

    add(name, (char*) (strlen(number) > 0 ? number : empty),
              (char*) (strlen(email) > 0 ? email : empty),
              (char*) (strlen(group) > 0 ? group : empty));
}

void add(char *name, char *number, char *email, char *group) {
    int i = n-1;
    while (i >= 0 && strcmp(directory[i].name, name) > 0) {
        directory[i+1] = directory[i];
        i--;
    }
    directory[i+1].name = strdup(name);
    directory[i+1].number = strdup(number);
    directory[i+1].email = strdup(email);
    directory[i+1].group = strdup(group);
    n++;
}