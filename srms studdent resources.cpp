#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

struct Student {
    int rollno;
    char name[50];
    float marks;
};

char currentUserRole[20];
char currentUser[50];

/* Function Declarations */
int loginSystem();
void mainmenu();
void adminMenu();
void staffMenu();
void userMenu();
void guestMenu();

void addStudent();
void viewStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

int main() {

    if (loginSystem()) {
        mainmenu();
    } else {
        printf("Login failed. Exiting program.\n");
    }

    return 0;
}

/* ================= LOGIN SYSTEM ================= */
int loginSystem() {
    char username[20], password[20];
    char fileuser[20], filepass[20], fileRole[20];

    printf("=== Login System ===\n");
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp) {
        printf("Error: credentials.txt not found\n");
        return 0;
    }

    while (fscanf(fp, "%s %s %s", fileuser, filepass, fileRole) == 3) {
        if (strcmp(username, fileuser) == 0 && strcmp(password, filepass) == 0) {
            strcpy(currentUserRole, fileRole);
            strcpy(currentUser, fileuser);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

/* ================= MAIN MENU (FIXED LOGOUT) ================= */
void mainmenu() {

    if (strcmp(currentUserRole, "ADMIN") == 0) {
        adminMenu();
    } 
    else if (strcmp(currentUserRole, "STAFF") == 0) {
        staffMenu();
    }
    else if (strcmp(currentUserRole, "USER") == 0) {
        userMenu();
    }
    else {
        guestMenu();
    }

    // After logout, program ends.
    printf("\nYou have been logged out.\n");
}

/* ================= STAFF MENU ================= */
void staffMenu() {
    int choice;
    do {
        printf("\n=== STAFF MENU ===\n");
        printf("1. Add Student\n");
        printf("2. View Students\n");
        printf("3. Search Student\n");
        printf("4. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: searchStudent(); break;
            case 4: return;  // Logout
            default: printf("Invalid choice! Try again.\n");
        }
    } while (1);
}

/* ================= USER MENU ================= */
void userMenu() {
    int choice;
    do {
        printf("\n=== USER MENU ===\n");
        printf("1. View Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: viewStudents(); break;
            case 2: searchStudent(); break;
            case 3: return;  // Logout
            default: printf("Invalid choice! Try again.\n");
        }
    } while (1);
}

/* ================= GUEST MENU ================= */
void guestMenu() {
    int choice;
    do {
        printf("\n=== GUEST MENU ===\n");
        printf("1. View Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: viewStudents(); break;
            case 2: searchStudent(); break;
            case 3: return;
            default: printf("Invalid choice! Try again.\n");
        }
    } while (1);
}

/* ================= ADMIN MENU ================= */
void adminMenu() {
    int choice;

    do {
        printf("\n=== ADMIN MENU ===\n");
        printf("1. Add Student\n");
        printf("2. View Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: return;
            default: printf("Invalid choice! Try again.\n");
        }

    } while (1);
}

/* ================= ADD STUDENT ================= */
void addStudent() {
    FILE *fp = fopen(STUDENT_FILE, "a");
    struct Student st;

    if (!fp) {
        printf("Error opening file\n");
        return;
    }

    printf("\nEnter Roll No: ");
    scanf("%d", &st.rollno);
    printf("Enter Name: ");
    scanf("%s", st.name);
    printf("Enter Marks: ");
    scanf("%f", &st.marks);

    fprintf(fp, "%d %s %.2f\n", st.rollno, st.name, st.marks);
    fclose(fp);

    printf("Student added successfully.\n");
}

/* ================= VIEW STUDENTS ================= */
void viewStudents() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    struct Student st;

    if (!fp) {
        printf("No student data found.\n");
        return;
    }

    printf("\n=== Student Records ===\n");
    printf("Roll No\tName\tMarks\n");

    while (fscanf(fp, "%d %s %f", &st.rollno, st.name, &st.marks) == 3) {
        printf("%d\t%s\t%.2f\n", st.rollno, st.name, st.marks);
    }

    fclose(fp);
}

/* ================= SEARCH STUDENT ================= */
void searchStudent() {
    int rollno, found = 0;
    struct Student st;

    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("Error opening file\n");
        return;
    }

    printf("\nEnter Roll No to search: ");
    scanf("%d", &rollno);

    while (fscanf(fp, "%d %s %f", &st.rollno, st.name, &st.marks) == 3) {
        if (st.rollno == rollno) {
            printf("Student Found: %d %s %.2f\n", st.rollno, st.name, st.marks);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Student with Roll No %d not found.\n", rollno);

    fclose(fp);
}

/* ================= UPDATE STUDENT ================= */
void updateStudent() {
    int rollno, found = 0;
    struct Student st;

    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("Error opening file\n");
        return;
    }

    printf("\nEnter Roll No to update: ");
    scanf("%d", &rollno);

    while (fscanf(fp, "%d %s %f", &st.rollno, st.name, &st.marks) == 3) {
        if (st.rollno == rollno) {
            printf("Current Record: %d %s %.2f\n", st.rollno, st.name, st.marks);

            printf("Enter New Name: ");
            scanf("%s", st.name);
            printf("Enter New Marks: ");
            scanf("%f", &st.marks);

            found = 1;
        }
        fprintf(temp, "%d %s %.2f\n", st.rollno, st.name, st.marks);
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(STUDENT_FILE);
        rename("temp.txt", STUDENT_FILE);
        printf("Student updated successfully.\n");
    } else {
        printf("Roll No %d not found.\n", rollno);
        remove("temp.txt");
    }
}

/* ================= DELETE STUDENT ================= */
void deleteStudent() {
    int rollno, found = 0;
    struct Student st;

    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("Error opening file\n");
        return;
    }

    printf("\nEnter Roll No to delete: ");
    scanf("%d", &rollno);

    while (fscanf(fp, "%d %s %f", &st.rollno, st.name, &st.marks) == 3) {
        if (st.rollno == rollno) {
            printf("Deleted: %d %s %.2f\n", st.rollno, st.name, st.marks);
            found = 1;
        } else {
            fprintf(temp, "%d %s %.2f\n", st.rollno, st.name, st.marks);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(STUDENT_FILE);
        rename("temp.txt", STUDENT_FILE);
    } else {
        printf("Roll No %d not found.\n", rollno);
        remove("temp.txt");
    }
}

