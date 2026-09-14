#include <stdio.h>
#include <string.h>

#define MAX_PATIENTS 100
#define NUM_SPECIALTIES 4
#define NUM_WARDS 4
#define MAX_BEDS 20


const char specialtyNames[NUM_SPECIALTIES][30] = {
    "General Practice",
    "Paediatrics",
    "Cardiology",
    "Neurology"
};

const float consultationFee[NUM_SPECIALTIES] = {
    1500.00,
    2500.00,
    4500.00,
    5000.00
};

const int consultationTime[NUM_SPECIALTIES] = {
    15,
    20,
    30,
    30
};


const char wardNames[NUM_WARDS][30] = {
    "General Ward",
    "Paediatric Ward",
    "Surgical Ward",
    "ICU"
};

const float wardRate[NUM_WARDS] = {
    3000.00,
    6000.00,
    12000.00,
    25000.00
};

const int wardCapacity[NUM_WARDS] = {
    20,
    10,
    10,
    5
};


int bedOccupancy[NUM_WARDS][MAX_BEDS] = {0};


char patientName[MAX_PATIENTS][100];
char patientID[MAX_PATIENTS][20];

int patientAge[MAX_PATIENTS];
int urgencyLevel[MAX_PATIENTS];
int patientSpecialty[MAX_PATIENTS];
int admitted[MAX_PATIENTS];
int patientWard[MAX_PATIENTS];
int daysAdmitted[MAX_PATIENTS];
int assignedBed[MAX_PATIENTS];

int waitingTime[MAX_PATIENTS];
int specialtyQueue[NUM_SPECIALTIES];

int patientCount = 0;



void displayMenu()
{
    printf("\n=====================================\n");
    printf("       SMART HOSPITAL SYSTEM\n");
    printf("=====================================\n");
    printf("1. Register Patient\n");
    printf("2. Display Patients\n");
    printf("3. Search Patient\n");
    printf("4. Allocate Bed\n");
    printf("5. Calculate Bill\n");
    printf("6. Display Reports\n");
    printf("7. Exit\n");
    printf("=====================================\n");
}


void displaySpecialties();
void displayWards();
void registerPatient()
{
    int index;
    int specialty;

    if(patientCount >= MAX_PATIENTS)
    {
        printf("\nPatient limit reached!\n");
        return;
    }

    index = patientCount;

    printf("\n=====================================\n");
    printf("       PATIENT REGISTRATION\n");
    printf("=====================================\n");

    sprintf(patientID[index], "PAT-%04d", 1001 + index);

    printf("Patient ID: %s\n", patientID[index]);

    printf("Enter patient name: ");
    scanf(" %[^\n]", patientName[index]);

    printf("Enter age: ");
    scanf("%d", &patientAge[index]);

    printf("\nEmergency / Triage Level\n");
    printf("1. Normal\n");
    printf("2. Urgent\n");
    printf("3. Critical\n");

    printf("Enter level: ");
    scanf("%d", &urgencyLevel[index]);

    displaySpecialties();

    printf("Enter specialty ID (1-4): ");
    scanf("%d", &specialty);

    patientSpecialty[index] = specialty - 1;

    waitingTime[index] =
        specialtyQueue[patientSpecialty[index]]
        * consultationTime[patientSpecialty[index]];

    specialtyQueue[patientSpecialty[index]]++;

    admitted[index] = 0;
    patientWard[index] = -1;
    daysAdmitted[index] = 0;
    assignedBed[index] = -1;

    patientCount++;

    printf("\nPatient registered successfully!\n");
    printf("Patient ID   : %s\n", patientID[index]);
    printf("Patient Name : %s\n", patientName[index]);
    printf("Waiting Time : %d minutes\n", waitingTime[index]);
}


void displaySpecialties()
{
    int i;

    printf("\n---------- SPECIALTIES ----------\n");

    for(i = 0; i < NUM_SPECIALTIES; i++)
    {
        printf("%d. %s - Rs. %.2f - %d minutes\n",
               i + 1,
               specialtyNames[i],
               consultationFee[i],
               consultationTime[i]);
    }
}



void displayWards()
{
    int i;

    printf("\n---------- WARDS ----------\n");

    for(i = 0; i < NUM_WARDS; i++)
    {
        printf("%d. %s - Rs. %.2f/day - Capacity: %d\n",
               i + 1,
               wardNames[i],
               wardRate[i],
               wardCapacity[i]);
    }
}


int main()
{
    int choice;

    do
    {
        displayMenu();

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                registerPatient();
                break;

            case 2:
                printf("\nPatient display will be added next.\n");
                break;

            case 3:
                printf("\nPatient search will be added next.\n");
                break;

            case 4:
                printf("\nBed allocation will be added next.\n");
                break;

            case 5:
                printf("\nBill calculation will be added next.\n");
                break;

            case 6:
                printf("\nReports will be added next.\n");
                break;

            case 7:
                printf("\nThank you for using Smart Hospital System!\n");
                break;

            default:
                printf("\nInvalid choice! Please enter 1-7.\n");
        }

    } while(choice != 7);

    return 0;
}
