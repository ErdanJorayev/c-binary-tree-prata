#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define SLEN 20
#include "tree.h"

// Prototypes for local functions
char menu(void);
void addpet(Tree * pt);
void droppet(Tree * pt);
void showpets(const Tree * pt);
void findpet(const Tree * pt);
void printitem(Item item);
void uppercase(char * str);
char * s_gets(char * st, int n);

int main(void)
{
    Tree pets;
    char choice;

    InitializeTree(&pets);

    // Main program loop
    while ((choice = menu()) != 'q')
    {
        switch(choice)
        {
            case 'a' : addpet(&pets);
                break;
            case 'l' : showpets(&pets);
                break;
            case 'f' : findpet(&pets);
                break;
            case 'n' : printf("%d pets in the club\n", TreeItemCount(&pets));
                break;
            case 'd' : droppet(&pets);
                break;
            default  : puts("Switch error");
        }
    }

    DeleteAll(&pets);
    puts("Program completed.");

    return 0;
}

// Display menu and get user choice
char menu(void)
{
    int ch;

    puts("Nerfville Pet Club Membership Program");
    puts("Enter the letter corresponding to your choice:");
    puts("a) Add a pet          l) Show list of pets");
    puts("n) Number of pets     f) Find a pet");
    puts("d) Delete a pet       q) Quit");

    while ((ch = getchar()) != EOF)
    {
        while (getchar() != '\n') // Discard rest of line
            continue;
        ch = tolower(ch);
        if (strchr("alrfndq", ch) == NULL)
            puts("Please enter a, l, f, n, d, or q:");
        else
            break;
    }
    if (ch == EOF) // Exit on EOF
        ch = 'q';
    return ch;
}

// Add a new pet to the tree
void addpet(Tree * pt)
{
    Item temp;
    if (TreeIsFull(pt))
        puts("The club is full!");
    else
    {
        puts("Enter pet name:");
        s_gets(temp.petname, SLEN);
        puts("Enter pet kind:");
        s_gets(temp.petkind, SLEN);
        uppercase(temp.petname);
        uppercase(temp.petkind);
        AddItem(&temp, pt);
    }
}

// Display all pets in the tree
void showpets(const Tree * pt)
{
    if (TreeIsEmpty(pt))
        puts("No records found!");
    else
        Traverse(pt, printitem);
}

// Print formatted pet info
void printitem(Item item)
{
    printf("Pet: %-19s Kind: %-19s\n", item.petname, item.petkind);
}

// Search for a specific pet
void findpet(const Tree * pt)
{
    Item temp;
    if (TreeIsEmpty(pt))
    {
        puts("No records found!");
        return;
    }

    puts("Enter name of the pet to find:");
    s_gets(temp.petname, SLEN);
    puts("Enter pet kind:");
    s_gets(temp.petkind, SLEN);
    uppercase(temp.petname);
    uppercase(temp.petkind);

    printf("%s named %s ", temp.petkind, temp.petname);
    if (InTree(&temp, pt))
        printf("is a member.\n");
    else
        printf("is not a member.\n");
}

// Remove a pet from the club
void droppet(Tree * pt)
{
    Item temp;
    if (TreeIsEmpty(pt))
    {
        puts("No records found!");
        return;
    }

    puts("Enter name of the pet to exclude:");
    s_gets(temp.petname, SLEN);
    puts("Enter pet kind:");
    s_gets(temp.petkind, SLEN);
    uppercase(temp.petname);
    uppercase(temp.petkind);

    printf("%s named %s ", temp.petkind, temp.petname);
    if (DeleteItem(&temp, pt))
        printf("has been excluded from the club.\n");
    else
        printf("is not a member.\n");
}

// Convert string to uppercase
void uppercase(char * str)
{
    while (*str)
    {
        *str = toupper(*str);
        str++;
    }
}

// Custom input function to handle newlines
char * s_gets(char * st, int n)
{
    char * ret_val;
    char * find;

    ret_val = fgets(st, n, stdin);
    if (ret_val)
    {
        find = strchr(st, '\n'); // Look for newline
        if (find)                // If found,
            *find = '\0';        // replace with null terminator
        else
            while (getchar() != '\n')
                continue;        // Discard rest of line
    }
    return ret_val;
}
