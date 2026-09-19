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
    printf("01. Register Patient\n");
    printf("02. Display Patients\n");
    printf("03. Search Patient\n");
    printf("04. Allocate Bed\n");
    printf("05. Calculate Bill\n");
    printf("06. Display Reports\n");
    printf("07. Display Admission Details\n");
    printf("08. Display Queue Status\n");
    printf("09. Consultation Fee\n");
    printf("10. Emergency Surcharge\n");
    printf("11. Complete Bill\n");
    printf("12. Age Discount\n");
    printf("13. Sort Patients by Priority\n");
    printf("14. Save Hospital Data\n");
    printf("15. Exit\n");
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

    if(patientAge[index] < 0 || patientAge[index] > 120)
    {
         printf("\nInvalid age!\n");
         return;
   }

    printf("\nEmergency / Triage Level\n");
    printf("1. Normal\n");
    printf("2. Urgent\n");
    printf("3. Critical\n");

    printf("Enter level: ");
    scanf("%d", &urgencyLevel[index]);

    if(urgencyLevel[index] < 1 || urgencyLevel[index] > 3)
    {
         printf("\nInvalid emergency level!\n");
         return;
    }

    displaySpecialties();

    printf("Enter specialty ID (1-4): ");
    scanf("%d", &specialty);

    if(specialty < 1 || specialty > 4)
    {
       printf("\nInvalid specialty!\n");
       return;
    }

    patientSpecialty[index] = specialty - 1;

    waitingTime[index] = specialtyQueue[patientSpecialty[index]] * consultationTime[patientSpecialty[index]];

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
    int days;

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

    printf("\nEnter Ward Number (1-%d): ", NUM_WARDS);
    scanf("%d", &ward);

    ward--;

    if(ward < 0 || ward >= NUM_WARDS)
    {
        printf("\nInvalid ward number!\n");
        return;
    }

    printf("Enter number of days: ");
    scanf("%d", &days);

    if(days <= 0)
    {
        printf("\nInvalid number of days!\n");
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
        printf("\nNo beds available in %s!\n",
               wardNames[ward]);
        return;
    }

    bedOccupancy[ward][bed] = 1;

    admitted[patientIndex] = 1;
    patientWard[patientIndex] = ward;
    assignedBed[patientIndex] = bed;
    daysAdmitted[patientIndex] = days;

    printf("\n=====================================\n");
    printf("       BED ALLOCATION SUCCESSFUL\n");
    printf("=====================================\n");

    printf("Patient : %s\n",
           patientName[patientIndex]);

    printf("Ward    : %s\n",
           wardNames[ward]);

    printf("Bed No  : %d\n",
           bed + 1);

    printf("Days    : %d\n",
           days);

    printf("=====================================\n");
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


void calculateBill()
{
    int patientIndex;
    int specialty;
    int ward;
    int level;
    int age;
    float consultation;
    float surchargeRate;
    float surcharge;
    float wardCost;
    float gross;
    float discountRate;
    float discount;
    float finalBill;

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
    level = urgencyLevel[patientIndex];
    age = patientAge[patientIndex];

    consultation = consultationFee[specialty];


    if(level == 1)
        surchargeRate = 0.00;
    else if(level == 2)
        surchargeRate = 0.20;
    else
        surchargeRate = 0.50;

    surcharge = consultation * surchargeRate;



    wardCost = 0.00;

    if(admitted[patientIndex] == 1)
    {
        ward = patientWard[patientIndex];
        wardCost = wardRate[ward] * daysAdmitted[patientIndex];
    }

    gross = consultation + surcharge + wardCost;


    if(age < 5 || age > 65)
        discountRate = 0.15;
    else
        discountRate = 0.00;

    discount = gross * discountRate;

    finalBill = gross - discount;

    printf("\n=====================================\n");
    printf("            PATIENT BILL\n");
    printf("=====================================\n");
    printf("Patient          : %s\n", patientName[patientIndex]);
    printf("Consultation     : Rs. %.2f\n", consultation);
    printf("Emergency Charge : Rs. %.2f\n", surcharge);
    printf("Ward Cost        : Rs. %.2f\n", wardCost);
    printf("Gross Total      : Rs. %.2f\n", gross);
    printf("Discount         : Rs. %.2f\n", discount);
    printf("Final Bill       : Rs. %.2f\n", finalBill);
    printf("=====================================\n");
}



void completeBill()
{
    int patientIndex;
    int specialty;
    int level;
    float consultation;
    float surchargeRate;
    float surcharge;
    float wardCost;
    float gross;
    float discountRate;
    float discount;
    float finalBill;

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
    level = urgencyLevel[patientIndex];


    consultation = consultationFee[specialty];


    if(level == 1)
        surchargeRate = 0.00;
    else if(level == 2)
        surchargeRate = 0.20;
    else
        surchargeRate = 0.50;

    surcharge = consultation * surchargeRate;

    /* Ward cost */
    wardCost = 0.00;

    if(admitted[patientIndex] == 1)
    {
        wardCost = wardRate[patientWard[patientIndex]]
                   * daysAdmitted[patientIndex];
    }

    /* Gross total */
    gross = consultation + surcharge + wardCost;

    /* Age discount */
    if(patientAge[patientIndex] < 5 ||
       patientAge[patientIndex] > 65)
    {
        discountRate = 0.15;
    }
    else
    {
        discountRate = 0.00;
    }

    discount = gross * discountRate;

    finalBill = gross - discount;

    printf("\n=====================================\n");
    printf("          COMPLETE BILL\n");
    printf("=====================================\n");

    printf("Patient ID       : %s\n",
           patientID[patientIndex]);

    printf("Patient Name     : %s\n",
           patientName[patientIndex]);

    printf("Age              : %d\n",
           patientAge[patientIndex]);

    printf("Specialty        : %s\n",
           specialtyNames[specialty]);

    printf("Consultation Fee : Rs. %.2f\n",
           consultation);

    printf("Emergency Charge : Rs. %.2f\n",
           surcharge);

    printf("Ward Cost        : Rs. %.2f\n",
           wardCost);

    printf("-------------------------------------\n");

    printf("Gross Total      : Rs. %.2f\n",
           gross);

    printf("Age Discount     : Rs. %.2f\n",
           discount);

    printf("-------------------------------------\n");

    printf("FINAL PAYABLE    : Rs. %.2f\n",
           finalBill);

    printf("=====================================\n");
}


void displayReports()
{
    int i, j;

    int level1 = 0;
    int level2 = 0;
    int level3 = 0;

    int occupied = 0;
    int totalBeds = 0;

    float totalRevenue = 0.0;
    float totalDiscount = 0.0;

    int highestPatient = -1;
    float highestBill = 0.0;


    for(i = 0; i < patientCount; i++)
    {
        float consultation;
        float surchargeRate;
        float surcharge;
        float wardCost;
        float discountRate;
        float discount;
        float finalBill;

        if(urgencyLevel[i] == 1)
            level1++;
        else if(urgencyLevel[i] == 2)
            level2++;
        else if(urgencyLevel[i] == 3)
            level3++;


        consultation = consultationFee[patientSpecialty[i]];


        if(urgencyLevel[i] == 1)
            surchargeRate = 0.00;
        else if(urgencyLevel[i] == 2)
            surchargeRate = 0.20;
        else
            surchargeRate = 0.50;

        surcharge = consultation * surchargeRate;

        wardCost = 0.00;

        if(admitted[i] == 1)
        {
            wardCost = wardRate[patientWard[i]]
                       * daysAdmitted[i];
        }

        finalBill = consultation + surcharge + wardCost;

        if(patientAge[i] < 5 || patientAge[i] > 65)
            discountRate = 0.15;
        else
            discountRate = 0.00;

        discount = finalBill * discountRate;

        finalBill = finalBill - discount;

        totalRevenue += finalBill;
        totalDiscount += discount;

        if(finalBill > highestBill)
        {
            highestBill = finalBill;
            highestPatient = i;
        }
    }

    for(i = 0; i < NUM_WARDS; i++)
    {
        for(j = 0; j < wardCapacity[i]; j++)
        {
            if(bedOccupancy[i][j] == 1)
                occupied++;
        }

        totalBeds += wardCapacity[i];
    }

    printf("\n=====================================\n");
    printf("          HOSPITAL REPORT\n");
    printf("=====================================\n");

    printf("Total Patients       : %d\n", patientCount);
    printf("Normal Patients      : %d\n", level1);
    printf("Urgent Patients      : %d\n", level2);
    printf("Critical Patients    : %d\n", level3);

    printf("\nOccupied Beds        : %d\n", occupied);
    printf("Total Beds           : %d\n", totalBeds);

    printf("\nTotal Revenue        : Rs. %.2f\n", totalRevenue);
    printf("Total Discounts      : Rs. %.2f\n", totalDiscount);

    if(highestPatient != -1)
    {
        printf("Highest-Paying Patient : %s\n",
               patientName[highestPatient]);

        printf("Highest Bill           : Rs. %.2f\n",
               highestBill);
    }

    printf("=====================================\n");
}



void sortPatientsByPriority()
{
    int i, j;
    int tempInt;

    char tempName[100];
    char tempID[20];

    for(i = 0; i < patientCount - 1; i++)
    {
        for(j = i + 1; j < patientCount; j++)
        {

            if(urgencyLevel[i] < urgencyLevel[j])
            {

                tempInt = urgencyLevel[i];
                urgencyLevel[i] = urgencyLevel[j];
                urgencyLevel[j] = tempInt;


                tempInt = patientAge[i];
                patientAge[i] = patientAge[j];
                patientAge[j] = tempInt;


                tempInt = patientSpecialty[i];
                patientSpecialty[i] = patientSpecialty[j];
                patientSpecialty[j] = tempInt;


                tempInt = admitted[i];
                admitted[i] = admitted[j];
                admitted[j] = tempInt;


                tempInt = patientWard[i];
                patientWard[i] = patientWard[j];
                patientWard[j] = tempInt;


                tempInt = daysAdmitted[i];
                daysAdmitted[i] = daysAdmitted[j];
                daysAdmitted[j] = tempInt;


                tempInt = assignedBed[i];
                assignedBed[i] = assignedBed[j];
                assignedBed[j] = tempInt;


                tempInt = waitingTime[i];
                waitingTime[i] = waitingTime[j];
                waitingTime[j] = tempInt;


                strcpy(tempName, patientName[i]);
                strcpy(patientName[i], patientName[j]);
                strcpy(patientName[j], tempName);


                strcpy(tempID, patientID[i]);
                strcpy(patientID[i], patientID[j]);
                strcpy(patientID[j], tempID);
            }
        }
    }

    printf("\n=====================================\n");
    printf("   PATIENTS SORTED BY PRIORITY\n");
    printf("=====================================\n");

    for(i = 0; i < patientCount; i++)
    {
        printf("\n%d. %s\n", i + 1, patientName[i]);
        printf("   Patient ID : %s\n", patientID[i]);
        printf("   Emergency  : %d\n", urgencyLevel[i]);
        printf("   Specialty  : %s\n",
               specialtyNames[patientSpecialty[i]]);
    }

    printf("\n=====================================\n");
}



void savePatientRecords()
{
    FILE *file;
    int i;

    file = fopen("patient_records.txt", "w");

    if(file == NULL)
    {
        printf("\nError opening patient_records.txt!\n");
        return;
    }

    for(i = 0; i < patientCount; i++)
    {
        fprintf(file, "Patient ID: %s\n", patientID[i]);
        fprintf(file, "Name: %s\n", patientName[i]);
        fprintf(file, "Age: %d\n", patientAge[i]);
        fprintf(file, "Emergency Level: %d\n", urgencyLevel[i]);
        fprintf(file, "Specialty: %s\n",
                specialtyNames[patientSpecialty[i]]);
        fprintf(file, "Waiting Time: %d minutes\n",
                waitingTime[i]);

        if(admitted[i] == 1)
        {
            fprintf(file, "Ward: %s\n",
                    wardNames[patientWard[i]]);
            fprintf(file, "Bed: %d\n",
                    assignedBed[i] + 1);
            fprintf(file, "Days Admitted: %d\n",
                    daysAdmitted[i]);
        }
        else
        {
            fprintf(file, "Status: Not Admitted\n");
        }

        fprintf(file, "-----------------------------\n");
    }

    fclose(file);

    printf("\nPatient records saved successfully!\n");
}



void saveBedsStatus()
{
    FILE *file;
    int i, j;

    file = fopen("beds_status.txt", "w");

    if(file == NULL)
    {
        printf("\nError opening beds_status.txt!\n");
        return;
    }

    for(i = 0; i < NUM_WARDS; i++)
    {
        fprintf(file, "Ward: %s\n", wardNames[i]);
        fprintf(file, "Capacity: %d\n", wardCapacity[i]);

        for(j = 0; j < wardCapacity[i]; j++)
        {
            fprintf(file, "Bed %d: %s\n",
                    j + 1,
                    bedOccupancy[i][j] == 1 ? "Occupied" : "Available");
        }

        fprintf(file, "-----------------------------\n");
    }

    fclose(file);

    printf("\nBed status saved successfully!\n");
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
                calculateBill();
                break;

            case 6:
               displayReports();
                break;

            case 7:
                displayAdmissionDetails();
                break;

            case 8:
                displayQueueStatus();
                break;

            case 9:
                calculateConsultationFee();
                break;

            case 10:
                calculateEmergencySurcharge();
                break;

            case 11:
                completeBill();
                break;

            case 12:
                calculateAgeDiscount();
                break;

            case 13:
                sortPatientsByPriority();
                break;

            case 14:
                savePatientRecords();
                saveBedsStatus();
                break;

            case 15:
                printf("\nThank you for using Smart Hospital System!\n");
                break;

            default:
                printf("\nInvalid choice! Please enter 1-15.\n");
        }

    } while(choice != 15);

    return 0;
}
