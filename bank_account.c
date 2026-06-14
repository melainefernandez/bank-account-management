// Name: Melaine Fernandez Sarduy 
// NetID: melainefernandez 
// Program Description: This program 
//      can create and manage checking 
//      and saving bank accounts. 

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>

#define NAME_LEN 30
#define MAX_PARTS 10

// define the structure of 
//  the acount
struct part {
    int number;
    char type;
    char name[NAME_LEN + 1];
    char date[11];
    double balance;
};

// Declare functions to be used
int find_part(int number, struct part *acc, int np);
void create(struct part **acc, int *np, int *capacity);
void deposit(struct part *acc, int np);
void withdraw(struct part *acc, int np);
void sort(struct part *acc, int np);
int read_line(char str[], int n);
int compare(const void *a, const void *b);

// Global variable for sorting field
char sort_field;

// Print main menu in a separate function 
//      to avoid unnecesay repetition of 
//      the menu into main function.
void print_menu() {
    printf("******* Main menu *******\n");
    printf("c(reate an account)\n");
    printf("d(eposit money)\n");
    printf("w(ithdraw money)\n");
    printf("s(sort and print accounts)\n");
    printf("q(uit program)\n");
    printf("*************************\n");
}

int main(void)
{
    char code;
    struct part *account = malloc(MAX_PARTS * sizeof(struct part));
    if (account == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    int num_parts = 0;
    int capacity = MAX_PARTS;

    print_menu();

    for (;;) {
        printf("Enter operation code (c, d, w, s, q): ");
        scanf(" %c", &code);
        while (getchar() != '\n');  /* skips to end of line */

        switch (code) {
            case 'c': create(&account, &num_parts, &capacity);
                      break;
            case 'd': deposit(account, num_parts);
                      break;
            case 'w': withdraw(account, num_parts);
                      break;
            case 's': sort(account, num_parts);
                      break;
            case 'q': free(account);
                      return 0;
            default:  printf("Illegal code\n");
        }
        printf("\n");
        print_menu();
    }
}

// function to search an account by its number 
//      where number is the account number, 
//      struct part *acc is a pointer to the array 
//      of parts (or accounts) and np is  
//      the number of parts stored in the array 

int find_part(int number, struct part *acc, int np)
{
    int i;
    for (i = 0; i < np; i++)
    // // Check if the current part's number 
    //      matches the input number
        if (acc[i].number == number)
            return i;
    return -1;
}

// Function to create the account, the double pointer
//      allows the function to modify the array itself,
//      np will be updated when a new account is added 
//      and int *capacity is a pointer to the current 
//      capacity array

void create(struct part **acc, int *np, int *capacity)
{
    if (*np >= *capacity) {
        *capacity *= 2;
        // ussing realoc to alllocate more memory
        *acc = realloc(*acc, *capacity * sizeof(struct part));
        if (*acc == NULL) {
            return;
        }
    }

    int number;
    char type;

number:
    printf("Enter account number (4 digits): ");
    scanf("%d", &number);
    // check if the number is less than 1000 or 
    //      bigger than 9999
    if (number < 1000 || number > 9999) {
        printf("Enter a number between 1000 and 9999.\n");
        goto number;
    }
    // Checking if the account number is 
    //      already used by another account
    if (find_part(number, *acc, *np) >= 0) {
        printf("Account already exists.\n");
        return;
    }

account_type:
    // Enter the account type and check if is 'c' or 's'. If it's 
    //     lowercase change to upper case 
    printf("Enter account type: c(hecking), s(avings)\nc or s: ");
    scanf(" %c", &type);
    if (type == 'c'){
        type = 'C';
    } else if (type == 's'){
        type = 'S';
    //  Checking if the user does not enter 'c' or 's'
    } else if (type != 'c' && type != 'C' && type != 'S' && type != 's') {
        printf("Wrong account type. Enter c or s.\n");
        goto account_type;
    }

    (*acc)[*np].number = number;
    (*acc)[*np].type = type;

    printf("Enter account holder's name: ");
    read_line((*acc)[*np].name, NAME_LEN);

    int year, month, day;

opendate:
    printf("Enter open date (YYYY-MM-DD): ");

    // Read the input as a string 
    char input[50];
    fgets(input, sizeof(input), stdin);

    // Remove the newline character 
    input[strcspn(input, "\n")] = '\0'; 

    // Ensures the format strictly follows "YYYY-MM-DD".
    int result = sscanf(input, "%4d-%2d-%2d", &year, &month, &day);

    // Check if the input has 3 inters and  
    //      it is 10 characters long
    if (result == 3 && strlen(input) == 10) {
        // When the user does not the right number corresponding to a year, month or day 
        if (year < 1000 || year > 9999 || month < 1 || month > 12 || day < 1 || day > 31) {
            printf("%s Wrong date format.\n", input); 
            goto opendate;
        }
        snprintf((*acc)[*np].date, 11, "%04d-%02d-%02d", year, month, day);
    } else {
        // Display error message when the input 
        //      is not 10 or sscanf fails to 
        //      parse 3 integers 
        printf("%s Wrong date format.\n", input); 
        goto opendate;
    }
    // ask the user to input the balance and 
    //      strore this result 
    printf("Enter balance (number only): $ ");
    scanf("%lf", &(*acc)[*np].balance);
    // When the account is succesfully created 
    //      and also increase the number of 
    //      acounts (*np)
    printf("Account %d is created.\n", number);
    (*np)++;
}

// Function that allows the usser to 
//      deposit money into the account 
void deposit(struct part *acc, int np)
{
    int number, index;
    double amount;
    printf("Enter account number: ");
    scanf("%d", &number);
    // find the account information 
    index = find_part(number, acc, np);

deposit:
    // Checking that deposit is bigger than 0
    printf("Enter amount (> 0, number only): $ ");
    scanf("%lf", &amount);
    if (amount <= 0) {
        printf("Amount must be greater than 0.\n");
        goto deposit;
    }
    acc[index].balance += amount;
    printf("Remaining balance: $ %.2f\n", acc[index].balance);
}

// withdrawing money from an account
void withdraw(struct part *acc, int np)
{
    int number, index;
    double amount;
    printf("Enter account number: ");
    scanf("%d", &number);
    index = find_part(number, acc, np);

withdraw:
    // Checking that amount is bigger than 0
    printf("Enter amount (> 0, number only): $ ");
    scanf("%lf", &amount);

    if (amount <= 0) {
        printf("Amount must be greater than 0.\n");
        goto withdraw;
    }
    // checking if the account has enough 
    //      funds. If not, choose another 
    //      option. 
    else if (amount > acc[index].balance) {
        printf("Insufficient balance.\n");
        return;
    }
    acc[index].balance -= amount;
    printf("Remaining balance: $ %.2f\n", acc[index].balance);
}
// Function to sort an array of struct part elements representing accounts based on a 
//         specified field (account number, holder name, open date, or balance).
void sort(struct part *acc, int np)
{
    printf("Enter the sorting field: a(ccount number), h(older name), o(pen date), b(alance)\na, h, o or b: ");
    scanf(" %c", &sort_field);

    qsort(acc, np, sizeof(struct part), compare);

    printf("#### Type Holder name                    Open date          Balance\n");
    printf("--------------------------------------------------------------------\n");
    for (int i = 0; i < np; i++) {
        printf("%4d  %c   %-30s %s   %10.2f\n", acc[i].number, acc[i].type, acc[i].name, acc[i].date, acc[i].balance);
    }
    printf("--------------------------------------------------------------------\n");
}
// Function to sort the array of struct part elements 
int compare(const void *a, const void *b)
{
    struct part *part_a = (struct part *)a;
    struct part *part_b = (struct part *)b;
    // switch statement to determine how to compare part a 
    //      and part b
    switch (sort_field) {
        case 'a':
            return part_a->number - part_b->number;
        case 'h':
            return strcasecmp(part_a->name, part_b->name); 
        case 'o':
            return strcmp(part_a->date, part_b->date);
        case 'b':
            if (part_a->balance < part_b->balance) return -1;
            if (part_a->balance > part_b->balance) return 1;
            return 0;
        default:
            return 0;
    }
}
// fucntion to read the name 
int read_line(char *str, int n)
{
    int ch;
    char *p = str; // Pointer to traverse the string
    while (isspace(ch = getchar())); 
    // Store the first non-whitespace character
    if (ch != '\n' && ch != EOF)
        *p++ = ch; 
    while ((ch = getchar()) != '\n' && ch != EOF) {
        if (p - str < n - 1)
            *p++ = ch; 
    }
    // add the null 
    //      character 
    *p = '\0'; 
    return p - str; 
}