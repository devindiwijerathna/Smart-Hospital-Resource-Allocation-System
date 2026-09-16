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
    printf("8. Display Admission Details\n");
    printf("9. Display Queue Status\n");
    printf("10. Consultation Fee\n");
    printf("11. Emergency Surcharge\n");
    printf("12 Age Discount\n");
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

void displayAllPatients()
{
    int i;

    if(patientCount == 0)
    {
        printf("\nNo patients registered yet.\n");
        return;
    }

    printf("\n==============================================\n");
    printf("              ALL PATIENTS\n");
    printf("==============================================\n");

    for(i = 0; i < patientCount; i++)
    {
        printf("\nPatient %d\n", i + 1);
        printf("Patient ID    : %s\n", patientID[i]);
        printf("Name          : %s\n", patientName[i]);
        printf("Age           : %d\n", patientAge[i]);
        printf("Emergency     : %d\n", urgencyLevel[i]);
        printf("Specialty     : %s\n",
               specialtyNames[patientSpecialty[i]]);
        printf("Waiting Time  : %d minutes\n", waitingTime[i]);
    }
}


void searchPatient()
{
    char searchID[20];
    int i;
    int found = 0;

    printf("\nEnter Patient ID: ");
    scanf("%s", searchID);

    for(i = 0; i < patientCount; i++)
    {
        if(strcmp(patientID[i], searchID) == 0)
        {
            printf("\nPatient Found!\n");
            printf("Patient ID   : %s\n", patientID[i]);
            printf("Name         : %s\n", patientName[i]);
            printf("Age          : %d\n", patientAge[i]);
            printf("Emergency    : %d\n", urgencyLevel[i]);
            printf("Specialty    : %s\n",
                   specialtyNames[patientSpecialty[i]]);
            printf("Waiting Time : %d minutes\n", waitingTime[i]);

            found = 1;
            break;
        }
    }

    if(found == 0)
    {
        printf("\nPatient not found!\n");
    }
}


void allocateBed()
{
    int patientIndex;
    int ward;
    int bed;
    int i;

    if(patientCount == 0)
    {
        printf("\nNo patients registered!\n");
        return;
    }

    printf("\nEnter Patient Number (1-%d): ", patientCount);
    scanf("%d", &patientIndex);

    patientIndex--;

    if(patientIndex < 0 || patientIndex >= patientCount)
    {
        printf("\nInvalid patient number!\n");
        return;
    }

    if(admitted[patientIndex] == 1)
    {
        printf("\nBed already allocated to this patient!\n");
        return;
    }

    displayWards();

    printf("\nEnter Ward Number (1-4): ");
    scanf("%d", &ward);

    ward--;

    if(ward < 0 || ward >= NUM_WARDS)
    {
        printf("\nInvalid ward number!\n");
        return;
    }

    bed = -1;

    for(i = 0; i < wardCapacity[ward]; i++)
    {
        if(bedOccupancy[ward][i] == 0)
        {
            bed = i;
            break;
        }
    }

    if(bed == -1)
    {
        printf("\nNo beds available in %s!\n", wardNames[ward]);
        return;
    }

    bedOccupancy[ward][bed] = 1;

    admitted[patientIndex] = 1;
    patientWard[patientIndex] = ward;
    assignedBed[patientIndex] = bed;

    printf("\nBed allocated successfully!\n");
    printf("Patient : %s\n", patientName[patientIndex]);
    printf("Ward    : %s\n", wardNames[ward]);
    printf("Bed No  : %d\n", bed + 1);
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

void displayAdmissionDetails()
{
    int i;

    if(patientCount == 0)
    {
        printf("\nNo patients registered!\n");
        return;
    }

    printf("\n=====================================\n");
    printf("       ADMISSION DETAILS\n");
    printf("=====================================\n");

    for(i = 0; i < patientCount; i++)
    {
        printf("\nPatient ID : %s\n", patientID[i]);
        printf("Name       : %s\n", patientName[i]);

        if(admitted[i] == 1)
        {
            printf("Status     : Admitted\n");
            printf("Ward       : %s\n", wardNames[patientWard[i]]);
            printf("Bed No     : %d\n", assignedBed[i] + 1);
        }
        else
        {
            printf("Status     : Not Admitted\n");
        }
    }
}



void displayQueueStatus()
{
    int i;

    printf("\n=====================================\n");
    printf("          QUEUE STATUS\n");
    printf("=====================================\n");

    for(i = 0; i < NUM_SPECIALTIES; i++)
    {
        printf("%d. %s\n", i + 1, specialtyNames[i]);
        printf("   Patients in queue : %d\n", specialtyQueue[i]);
        printf("   Waiting time      : %d minutes\n",
               specialtyQueue[i] * consultationTime[i]);
    }
}



void calculateConsultationFee()
{
    int patientIndex;
    int specialty;

    if(patientCount == 0)
    {
        printf("\nNo patients registered!\n");
        return;
    }

    printf("\nEnter Patient Number (1-%d): ", patientCount);
    scanf("%d", &patientIndex);

    patientIndex--;

    if(patientIndex < 0 || patientIndex >= patientCount)
    {
        printf("\nInvalid patient number!\n");
        return;
    }

    specialty = patientSpecialty[patientIndex];

    printf("\n=====================================\n");
    printf("       CONSULTATION FEE\n");
    printf("=====================================\n");
    printf("Patient    : %s\n", patientName[patientIndex]);
    printf("Specialty  : %s\n", specialtyNames[specialty]);
    printf("Fee        : Rs. %.2f\n", consultationFee[specialty]);
}



void calculateEmergencySurcharge()
{
    int patientIndex;
    int level;
    float fee;
    float surchargeRate;
    float surcharge;

    if(patientCount == 0)
    {
        printf("\nNo patients registered!\n");
        return;
    }

    printf("\nEnter Patient Number (1-%d): ", patientCount);
    scanf("%d", &patientIndex);

    patientIndex--;

    if(patientIndex < 0 || patientIndex >= patientCount)
    {
        printf("\nInvalid patient number!\n");
        return;
    }

    level = urgencyLevel[patientIndex];
    fee = consultationFee[patientSpecialty[patientIndex]];

    if(level == 1)
        surchargeRate = 0.00;
    else if(level == 2)
        surchargeRate = 0.20;
    else if(level == 3)
        surchargeRate = 0.50;
    else
    {
        printf("\nInvalid emergency level!\n");
        return;
    }

    surcharge = fee * surchargeRate;

    printf("\n=====================================\n");
    printf("       EMERGENCY SURCHARGE\n");
    printf("=====================================\n");
    printf("Patient           : %s\n", patientName[patientIndex]);
    printf("Emergency Level   : %d\n", level);
    printf("Consultation Fee  : Rs. %.2f\n", fee);
    printf("Surcharge         : Rs. %.2f\n", surcharge);
    printf("Total              : Rs. %.2f\n", fee + surcharge);
}



void calculateAgeDiscount()
{
    int patientIndex;
    float fee;
    float discountRate;
    float discount;

    if(patientCount == 0)
    {
        printf("\nNo patients registered!\n");
        return;
    }

    printf("\nEnter Patient Number (1-%d): ", patientCount);
    scanf("%d", &patientIndex);

    patientIndex--;

    if(patientIndex < 0 || patientIndex >= patientCount)
    {
        printf("\nInvalid patient number!\n");
        return;
    }

    fee = consultationFee[patientSpecialty[patientIndex]];

    if(patientAge[patientIndex] < 5 ||
       patientAge[patientIndex] > 65)
    {
        discountRate = 0.15;
    }
    else
    {
        discountRate = 0.00;
    }

    discount = fee * discountRate;

    printf("\n=====================================\n");
    printf("          AGE DISCOUNT\n");
    printf("=====================================\n");
    printf("Patient          : %s\n", patientName[patientIndex]);
    printf("Age              : %d\n", patientAge[patientIndex]);
    printf("Consultation Fee : Rs. %.2f\n", fee);
    printf("Discount         : Rs. %.2f\n", discount);
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
                displayAllPatients();
                break;

            case 3:
                searchPatient();
                break;

            case 4:
                allocateBed();
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

            case 8:
                displayAdmissionDetails();
                break;

            case 9:
                displayQueueStatus();
                break;

            case 10:
                calculateConsultationFee();
                break;

            case 11:
                calculateEmergencySurcharge();
                break;

            case 12:
                calculateAgeDiscount();
                break;

            default:
                printf("\nInvalid choice! Please enter 1-7.\n");
        }

    } while(choice != 7);

    return 0;
}
