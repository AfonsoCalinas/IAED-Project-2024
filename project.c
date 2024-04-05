/* iaed24 - ist1102373 - project */

# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>
# include <string.h>

/* Structures */
typedef struct ParkNode {
    struct vehicle *vehicle;
    struct ParkNode *next;
    struct ParkNode *prev;
} ParkNode;

typedef struct VehicleLeftNode {
    struct vehicle *vehicle;
    struct VehicleLeftNode *next;
    struct VehicleLeftNode *prev;
} VehicleLeftNode;

struct parking_lot
{
    char* name;
    int maxCapacity;
    double costBeforeOneHour;
    double costAfterOneHour;
    double dailyCost;
    int freeSpaces;
    ParkNode *head;
    ParkNode *tail;
    VehicleLeftNode *vHead;
    VehicleLeftNode *vTail;
    int index;
};

struct vehicle
{
    char* parking_lotName;
    char licencePlate[9];
    char dateIn[11];
    char hourIn[6];
    char dateOut[11];
    char hourOut[6];
    double bill;
};

typedef struct Node {
    struct parking_lot *parkingLot;
    char *licencePlate;
    struct Node *next;
} Node;


/* Functions */

void createParkingLot(struct parking_lot ***ParkingLotPtrList, int *nParkingLots, char *phParkingLotName, 
int phMaxCapacity, double phCostBeforeOneHour, double phCostAfterOneHour, double phDailyCost, int *parkingSorted);

int parkingLotVerifications(struct parking_lot ***ParkingLotPtrList, int *nParkingLots, char *phParkingLotName, 
int phMaxCapacity, double phCostBeforeOneHour, double phCostAfterOneHour, double phDailyCost);

void listParkingLots(struct parking_lot ***ParkingLotPtrList, int nParkingLots, int *parkingSorted);
void welcomeVehicle(struct parking_lot ***ParkingLotPtrList, int nParkingLots, char phVehicleParkingLotName[], 
char phLicencePlate[], char* phDate, char* phHour, char* lastSystemChange, Node **hash_table);

int vehicleEntryVerifications(struct parking_lot ***ParkingLotPtrList, int parkIndex, char phVehicleParkingLotName[],
char phLicencePlate[], char* phDate, char* phHour, char* lastSystemChange, Node **hash_table);

int checkLicencePlate(char* phLicencePlate);
int verifyDateAndHour(char* phLastSystemChange, char* phDate, char* phHour);
int isValidDate(int* day, int* month);
int isValidMonth(int* month);
int isValidDay(int* day, int* month);
int isValidHour(int* hour, int* min);
int isNewerTimestamp(char* phLastSystemChange, int* day, int* month, int* year, int* hour, int* min);
int isCarInParkingLot(Node *hash_table[], char* phLicencePlate);
void insertVehicleInParkRegistry(struct vehicle **vehicle, struct parking_lot *lot);
void cyaVehicle(struct parking_lot ***ParkingLotPtrList, int nParkingLots, char phVehicleParkingLotNameOUT[], 
char phLicencePlateOUT[], char* phDateOUT, char* phHourOUT, char* lastSystemChange, Node **hash_table);

int vehicleExitVerifications(int parkIndex, char phVehicleParkingLotNameOUT[],
char phLicencePlateOUT[], char* phDateOUT, char* phHourOUT, char* lastSystemChange, Node **hash_table);

void fillVehicleInfo(struct parking_lot ***ParkingLotPtrList, int parkIndex, 
char* phDateOUT, char* phHourOUT, double* vehicleBill, ParkNode *current);

double vehiclePayment(int timeDifferenceMin, double costBeforeOneHour, double costAfterOneHour, double dailyCost);
int totalTimeDifference(char *date_str1, char *time_str1, char *date_str2, char *time_str2);
void parseDateTime(char *date_str, char *time_str, int *day, int *month, int *year, int *hour, int *minute);
void insertVehicleInExitRegistry(struct vehicle **vehicle, struct parking_lot *lot);
void listVehicleHistory(struct parking_lot ***ParkingLotPtrList, int *nParkingLots, 
char *phLicencePlateSearch, int *parkingSorted);

void showParkBillingWithDate(struct parking_lot ***ParkingLotPtrList, int nParkingLots, 
char *phParkingLotNameBilling, char *phDateBilling, char* lastSystemChange);

void showParkBilling(struct parking_lot ***ParkingLotPtrList, int nParkingLots, char *phParkingLotNameBilling);
void removeParkingLot(struct parking_lot ***ParkingLotPtrList, int *nParkingLots, char *phParkingLotNameRemoval, 
int *parkingSorted, Node **hash_table);

void bubbleSortByName(struct parking_lot ***ParkingLotPtrList, int numParkingLots);
void bubbleSortByIndex(struct parking_lot ***ParkingLotPtrList, int numParkingLots);
unsigned long hashLicencePlate(char *str);
void insertVehicleInHashMap(struct parking_lot **ParkingLotPtrList, char *plate, Node *hash_table[]);
void removeVehicleInHashMap(char *license_plate, Node *hash_table[]);


/* Exception Functions */

void ParkingLotAlreadyExistsException(char* phParkingLotName);
void InvalidParkingLotException(int phMaxCapacity);
void InvalidCostException();
void TooManyParkingLotsException();
void ParkingLotDoesntExistException(char* phParkingLotName);
void ParkingLotFilledException(char* phParkingLotName);
void InvalidLicencePlateException(char* phLicencePlate);
void InvalidVehicleEntryException(char* phLicencePlate);
void InvalidDateException();
void VehicleNotHereException(char* phLicencePlate);
void NoEntriesFoundException(char* phLicencePlate);


/* Main */
int main()
{
    
    /* Command and Input Information*/
    char command, input[BUFSIZ];
    char *lastSystemChange = malloc(17 * sizeof(char));
    strcpy(lastSystemChange, "");

    /* Parking Lot Information */
    struct parking_lot **parkingLotCollection = malloc(20 * sizeof(struct parking_lot *));
    char parkName[BUFSIZ];
    int phMaxCapacity;
    double phCostBeforeOneHour;
    double phCostAfterOneHour;
    double phDailyCost;
    int nParkingLots = 0;
    int parkingSorted = 0;

    /* Hash Table Information */
    Node *hash_table[200] = {NULL};

    while (1)
    {

        if (fgets(input, sizeof(input), stdin) == NULL) {
            /* Here we're handling any errors that may come up in fgets */
            break;
        }

        command = input[0];

        switch (command)
        {
            case 'q':
                /*  User input: q */
                free(lastSystemChange);

                for (int i = 0; i < nParkingLots; i++)
                {
                    free(parkingLotCollection[i]->name);
                    free(parkingLotCollection[i]);
                }

                free(parkingLotCollection);
                
                return 0;

            case 'p':

                if (sscanf(input, " %c \"%[^\"]\" %d %lf %lf %lf", &command, parkName, &phMaxCapacity, 
                &phCostBeforeOneHour, &phCostAfterOneHour, &phDailyCost) == 6) {
                    /* User input: p "parkinglot Name" maxCapacity costBeforeOneHour costAfterOneHour DailyCost */

                    char *phParkingLotName = malloc((strlen(parkName)+1) * sizeof(char));
                    strcpy(phParkingLotName, parkName);
                    
                    createParkingLot(&parkingLotCollection, &nParkingLots, phParkingLotName, phMaxCapacity, 
                    phCostBeforeOneHour, phCostAfterOneHour, phDailyCost, &parkingSorted);
                    free(phParkingLotName);

                } else if (sscanf(input, " %c %s %d %lf %lf %lf", &command, parkName, &phMaxCapacity, 
                &phCostBeforeOneHour, &phCostAfterOneHour, &phDailyCost) == 6) {
                    /* User input: p parkinglotName maxCapacity costBeforeOneHour costAfterOneHour DailyCost */
                    
                    char *phParkingLotName = malloc((strlen(parkName)+1) * sizeof(char));
                    strcpy(phParkingLotName, parkName);

                    createParkingLot(&parkingLotCollection, &nParkingLots, phParkingLotName, phMaxCapacity, 
                    phCostBeforeOneHour, phCostAfterOneHour, phDailyCost, &parkingSorted);
                    free(phParkingLotName);

                } else if (sscanf(input, " %c", &command) == 1) {
                    /* User input: p */

                    /* Lists every parking lot there is */
                    listParkingLots(&parkingLotCollection, nParkingLots, &parkingSorted);
                }

                break;

            case 'e':

                char *phLicencePlate = malloc(9 * sizeof(char));
                char *phDate = malloc(11 * sizeof(char));
                char *phHour = malloc(6 * sizeof(char));

                if (sscanf(input, " %c \"%[^\"]\" %s %s %s", &command, parkName, phLicencePlate, phDate, phHour) == 5) {
                    /* User input: e "parkinglot Name" maxCapacity costBeforeOneHour costAfterOneHour DailyCost */
                    
                    char *phVehicleParkingLotName = malloc((strlen(parkName)+1) * sizeof(char));
                    strcpy(phVehicleParkingLotName, parkName);

                    int newSize = strlen(phLicencePlate) + 1;
                    char *newPhLicencePlate = realloc(phLicencePlate, newSize * sizeof(char));
                    phLicencePlate = newPhLicencePlate;

                    newSize = strlen(phDate) + 1;
                    char *newPhDate = realloc(phDate, newSize * sizeof(char));
                    phDate = newPhDate;

                    newSize = strlen(phHour) + 1;
                    char *newPhHour = realloc(phHour, newSize * sizeof(char));
                    phHour = newPhHour;

                    welcomeVehicle(&parkingLotCollection, nParkingLots, phVehicleParkingLotName, 
                    phLicencePlate, phDate, phHour, lastSystemChange, hash_table);
                    free(phVehicleParkingLotName);

                } else if (sscanf(input, " %c %s %s %s %s", &command, parkName, phLicencePlate, phDate, phHour) == 5) {
                    /* User input: e parkinglotName maxCapacity costBeforeOneHour costAfterOneHour DailyCost */

                    char *phVehicleParkingLotName = malloc((strlen(parkName)+1) * sizeof(char));
                    strcpy(phVehicleParkingLotName, parkName);

                    int newSize = strlen(phLicencePlate) + 1;
                    char *newPhLicencePlate = realloc(phLicencePlate, newSize * sizeof(char));
                    phLicencePlate = newPhLicencePlate;

                    newSize = strlen(phDate) + 1;
                    char *newPhDate = realloc(phDate, newSize * sizeof(char));
                    phDate = newPhDate;

                    newSize = strlen(phHour) + 1;
                    char *newPhHour = realloc(phHour, newSize * sizeof(char));
                    phHour = newPhHour;

                    welcomeVehicle(&parkingLotCollection, nParkingLots, phVehicleParkingLotName, 
                    phLicencePlate, phDate, phHour, lastSystemChange, hash_table);
                    free(phVehicleParkingLotName);
                }

                free(phLicencePlate);
                free(phDate);
                free(phHour);

                break;
            
            case 's':

                char *phLicencePlateOUT = malloc(9 * sizeof(char));
                char *phDateOUT = malloc(11 * sizeof(char));
                char *phHourOUT = malloc(6 * sizeof(char));

                if (sscanf(input, " %c \"%[^\"]\" %s %s %s", &command, parkName, phLicencePlateOUT, phDateOUT, phHourOUT) == 5) {

                    char *phVehicleParkingLotNameOUT = malloc((strlen(parkName)+1) * sizeof(char));
                    strcpy(phVehicleParkingLotNameOUT, parkName);

                    int newSize = strlen(phLicencePlateOUT) + 1;
                    char *newphLicencePlateOUT = realloc(phLicencePlateOUT, newSize * sizeof(char));
                    phLicencePlateOUT = newphLicencePlateOUT;

                    newSize = strlen(phDateOUT) + 1;
                    char *newphDateOUT = realloc(phDateOUT, newSize * sizeof(char));
                    phDateOUT = newphDateOUT;

                    newSize = strlen(phHourOUT) + 1;
                    char *newphHourOUT = realloc(phHourOUT, newSize * sizeof(char));
                    phHourOUT = newphHourOUT;

                    cyaVehicle(&parkingLotCollection, nParkingLots, phVehicleParkingLotNameOUT, 
                    phLicencePlateOUT, phDateOUT, phHourOUT, lastSystemChange, hash_table);
                    free(phVehicleParkingLotNameOUT);

                } else if(sscanf(input, " %c %s %s %s %s", &command, parkName, phLicencePlateOUT, phDateOUT, phHourOUT) == 5) {
                
                    char *phVehicleParkingLotNameOUT = malloc((strlen(parkName)+1) * sizeof(char));
                    strcpy(phVehicleParkingLotNameOUT, parkName);

                    int newSize = strlen(phLicencePlateOUT) + 1;
                    char *newphLicencePlateOUT = realloc(phLicencePlateOUT, newSize * sizeof(char));
                    phLicencePlateOUT = newphLicencePlateOUT;

                    newSize = strlen(phDateOUT) + 1;
                    char *newphDateOUT = realloc(phDateOUT, newSize * sizeof(char));
                    phDateOUT = newphDateOUT;

                    newSize = strlen(phHourOUT) + 1;
                    char *newphHourOUT = realloc(phHourOUT, newSize * sizeof(char));
                    phHourOUT = newphHourOUT;

                    cyaVehicle(&parkingLotCollection, nParkingLots, phVehicleParkingLotNameOUT, 
                    phLicencePlateOUT, phDateOUT, phHourOUT, lastSystemChange, hash_table);
                    free(phVehicleParkingLotNameOUT);
                }

                free(phLicencePlateOUT);
                free(phDateOUT);
                free(phHourOUT);

                break;

            case 'v':

                char *phLicencePlateSearch = malloc(9 * sizeof(char));

                if (sscanf(input, " %c %s", &command, phLicencePlateSearch) == 2) {
                    /* User input: v LicencePlate */

                    listVehicleHistory(&parkingLotCollection, &nParkingLots, phLicencePlateSearch, &parkingSorted);

                }

                free(phLicencePlateSearch);

                break;

            case 'f':

                char *phDateBilling = malloc(11 * sizeof(char));
                
                if (sscanf(input, " %c \"%[^\"]\" %s", &command, parkName, phDateBilling) == 3) {
                    /* User input: f "ParkingLot Name" BillingDate */

                    char *phParkingLotNameBilling = malloc((strlen(parkName)+1) * sizeof(char));
                    strcpy(phParkingLotNameBilling, parkName);

                    int newSize = strlen(phDateBilling) + 1;
                    char *newphDateBilling = realloc(phDateBilling, newSize * sizeof(char));
                    phDateBilling = newphDateBilling;

                    showParkBillingWithDate(&parkingLotCollection, nParkingLots, phParkingLotNameBilling, phDateBilling, lastSystemChange);
                    free(phParkingLotNameBilling);

                } else if (sscanf(input, "%c \"%[^\"]\"", &command, parkName) == 2) {
                    /* User input: f "ParkingLot Name" */
                    
                    char *phParkingLotNameBilling = malloc((strlen(parkName)+1) * sizeof(char));
                    strcpy(phParkingLotNameBilling, parkName);

                    showParkBilling(&parkingLotCollection, nParkingLots, phParkingLotNameBilling);
                    free(phParkingLotNameBilling);

                } else if (sscanf(input, " %c %s %s", &command, parkName, phDateBilling) == 3) {
                    /* User input: f ParkingLotName BillingDate */

                    char *phParkingLotNameBilling = malloc((strlen(parkName)+1) * sizeof(char));
                    strcpy(phParkingLotNameBilling, parkName);

                    int newSize = strlen(phDateBilling) + 1;
                    char *newphDateBilling = realloc(phDateBilling, newSize * sizeof(char));
                    phDateBilling = newphDateBilling;

                    showParkBillingWithDate(&parkingLotCollection, nParkingLots, phParkingLotNameBilling, phDateBilling, lastSystemChange);
                    free(phParkingLotNameBilling);
                
                } else if (sscanf(input, " %c %s", &command, parkName) == 2) {
                    /* User input: f ParkingLotName */

                    char *phParkingLotNameBilling = malloc((strlen(parkName)+1) * sizeof(char));
                    strcpy(phParkingLotNameBilling, parkName);

                    showParkBilling(&parkingLotCollection, nParkingLots, phParkingLotNameBilling);
                    free(phParkingLotNameBilling);
                }

                free(phDateBilling);

                break;

            case 'r':

                if (sscanf(input, " %c \"%[^\"]\"", &command, parkName) == 2) {
                    /* User input: r "ParkingLot Name" */
                    char *phParkingLotNameRemoval = malloc((strlen(parkName)+1) * sizeof(char));
                    strcpy(phParkingLotNameRemoval, parkName);

                    removeParkingLot(&parkingLotCollection, &nParkingLots, phParkingLotNameRemoval, &parkingSorted, hash_table);
                    free(phParkingLotNameRemoval);

                } else if (sscanf(input, " %c %s", &command, parkName) == 2) {
                    /* User input: r ParkingLotName */
                    char *phParkingLotNameRemoval = malloc((strlen(parkName)+1) * sizeof(char));
                    strcpy(phParkingLotNameRemoval, parkName);

                    removeParkingLot(&parkingLotCollection, &nParkingLots, phParkingLotNameRemoval, &parkingSorted, hash_table);
                    free(phParkingLotNameRemoval);
                }

                break;
        }
    }

    return 0;

}

/* Functions */


/**
 * FUNCTION: hashLicencePlate
 * 
 * INPUT: A Licence Plate
 * 
 * OUTPUT: The hashing of a licence plate
*/
unsigned long hashLicencePlate(char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        if (isalnum(c)) {
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        }
    }

    return hash % 200; // Ensure the hash value fits within the table size
}

/**
 * FUNCTION: insertVehicleInHashMap
 * 
 * INPUT: The ParkingLot's Pointer List, a Licence Plate and the Hash Map
 * 
 * OUTPUT: Inserts a new node in a specific index on the Hash Map
*/
void insertVehicleInHashMap(struct parking_lot **ParkingLotPtrList, char *plate, Node *hash_table[]) {

    unsigned long hashIndex = hashLicencePlate(plate);

    // Allocate memory for a new node
    Node *new_node = (Node *)malloc(sizeof(Node));
    
    // Save the Parking lot pointer in the new node
    new_node->parkingLot = *ParkingLotPtrList;
    new_node->licencePlate = strdup(plate);

    // Insert the node
    new_node->next = hash_table[hashIndex];
    hash_table[hashIndex] = new_node;
}

/**
 * FUNCTION: removeVehicleInHashMap
 * 
 * INPUT: A Licence Plate and the Hash Map
 * 
 * OUTPUT: Removes a specific node in a specific index on the Hash Map
*/
void removeVehicleInHashMap(char *license_plate, Node *hash_table[]) {
    unsigned long hashIndex = hashLicencePlate(license_plate);

    // Get the head of the linked list at the calculated index
    Node *current = hash_table[hashIndex];
    Node *prev = NULL;

    // Traverse the linked list to find the node to remove
    while (current != NULL) {
        if (current->parkingLot != NULL && strcmp(current->licencePlate, license_plate) == 0) {
        // Found the node to remove
            if (prev == NULL) {
                // The node to remove is the head of the list
                hash_table[hashIndex] = current->next;
            } else {
                // The node to remove is not the head of the list
                prev->next = current->next;
            }

        // Free the memory allocated for the node and the parking lot
        free(current->licencePlate);
        free(current);

        return; // Exit the function after removal
        }
        // Move to the next node
        prev = current;
        current = current->next;
    }
}

/** 
 * FUNCTION: createParkingLot
 *
 * INPUT: The ParkingLot's Pointer List, Number of existent Parking Lots, a Parking Lot Name, 
 * Maximum Capacity, three Fees and a flag
 * 
 * OUTPUT: Creates a Parking Lot, without printing anything
*/
void createParkingLot(struct parking_lot ***ParkingLotPtrList, int *nParkingLots, char *phParkingLotName, 
int phMaxCapacity, double phCostBeforeOneHour, double phCostAfterOneHour, double phDailyCost, int *parkingSorted) {

    // Exception handler
    if (parkingLotVerifications(ParkingLotPtrList, nParkingLots, phParkingLotName, phMaxCapacity, 
    phCostBeforeOneHour, phCostAfterOneHour, phDailyCost))
    {
        return;
    }

    // Allocate memory for the new park
    struct parking_lot * newParkingLot = malloc(sizeof(struct parking_lot));

    newParkingLot->name = malloc((strlen(phParkingLotName) + 1) * sizeof(char));
    strcpy(newParkingLot->name, phParkingLotName);
    newParkingLot->maxCapacity = phMaxCapacity;
    newParkingLot->costBeforeOneHour = phCostBeforeOneHour;
    newParkingLot->costAfterOneHour = phCostAfterOneHour;
    newParkingLot->dailyCost = phDailyCost;
    newParkingLot->freeSpaces = phMaxCapacity;
    newParkingLot->head = NULL;
    newParkingLot->tail = NULL;
    newParkingLot->vHead = NULL;
    newParkingLot->vTail = NULL;
    newParkingLot->index = (*nParkingLots);

    // Inserts the new park in the ParkingLot's Pointer List
    (*ParkingLotPtrList)[(*nParkingLots)] = newParkingLot; 
    (*nParkingLots)++;
    (*parkingSorted) = 0;
}

/** 
 * FUNCTION: parkingLotVerifications
 *
 * INPUT: The ParkingLot's Pointer List, the number of existent parking lots, a Parking Lot Name, 
 * Maximum Capacity and three Fees
 * 
 * OUTPUT: Handles all the exception that can occur in createParkingLot
*/
int parkingLotVerifications(struct parking_lot ***ParkingLotPtrList, int *nParkingLots, char *phParkingLotName, 
int phMaxCapacity, double phCostBeforeOneHour, double phCostAfterOneHour, double phDailyCost) {

    // Check if parking lot already exists
    for (int i = 0; i < (*nParkingLots); i++) {
        if (strcmp(phParkingLotName, (*ParkingLotPtrList)[i]->name) == 0) {
            ParkingLotAlreadyExistsException(phParkingLotName);
            return 1;
        }
    }

    // Check if park is at max capacity with no free spaces
    if (phMaxCapacity <= 0) {
        InvalidParkingLotException(phMaxCapacity);
        return 1;
    }

    // Check if the fees are valid
    if (!(phCostBeforeOneHour < phCostAfterOneHour && phCostAfterOneHour < phDailyCost && phCostBeforeOneHour > 0 && 
    phCostAfterOneHour > 0 && phDailyCost > 0)) {

        InvalidCostException();
        return 1;
    }

    // Checks if we reached the maximum amount of parking lots
    if ((*nParkingLots) == 20) {

        TooManyParkingLotsException();
        return 1;
    }

    return 0;
}

/**
 * FUNCTION: listParkingLots
 *
 * INPUT: The ParkingLot's Structure, Number of existent Parking Lots and a flag
 * 
 * OUTPUT: Prints all Parking Lots
*/
void listParkingLots(struct parking_lot ***ParkingLotPtrList, int nParkingLots, int *parkingSorted) {

    int i;

    if ((*parkingSorted) != 0) {
        bubbleSortByIndex(&(*ParkingLotPtrList), nParkingLots);
        (*parkingSorted) = 0;
    }

    for (i = 0; i < nParkingLots; i++) {
        
        printf("%s %d %d\n", (*ParkingLotPtrList)[i]->name, (*ParkingLotPtrList)[i]->maxCapacity, (*ParkingLotPtrList)[i]->freeSpaces);

    }

}

/**
 * FUNCTION: welcomeVehicle
 *
 * INPUT: The ParkingLot's Pointer List, number of existent parking lots, a parking lot's name, 
 * a vehicle's licence plate, date and time of arrival, the system's last change and the Hash Map
 * 
 * OUTPUT: Prints the name of the park the vehicle got into and it's free spaces left
*/
void welcomeVehicle(struct parking_lot ***ParkingLotPtrList, int nParkingLots, char phVehicleParkingLotName[], 
char phLicencePlate[], char* phDate, char* phHour, char* lastSystemChange, Node **hash_table) {

    // Find the parking lot index
    int parkIndex = -1;
    for (int i = 0; i < nParkingLots; i++) {        
        if (strcmp((*ParkingLotPtrList)[i]->name, phVehicleParkingLotName) == 0) {  
            parkIndex = i;
            break;
        }
    }

    // Exception handler
    if (vehicleEntryVerifications(ParkingLotPtrList, parkIndex, phVehicleParkingLotName,
    phLicencePlate, phDate, phHour, lastSystemChange, hash_table))
    {
        return;
    }

    // Allocates memory for the new vehicle
    struct vehicle * new_vehicle = malloc(sizeof(struct vehicle));

    // Initialize the new vehicle
    new_vehicle->parking_lotName = malloc((strlen(phVehicleParkingLotName)+1) * sizeof(char));
    strcpy(new_vehicle->parking_lotName, phVehicleParkingLotName);
    strcpy(new_vehicle->licencePlate, phLicencePlate);
    strcpy(new_vehicle->dateIn, phDate);
    strcpy(new_vehicle->hourIn, phHour);
    strcpy(new_vehicle->dateOut, "");
    strcpy(new_vehicle->hourOut, "");
    new_vehicle->bill = 0.00;

    // Updates parking lot spaces
    (*ParkingLotPtrList)[parkIndex]->freeSpaces--;

    // Inserts the new vehicle in the hash map
    insertVehicleInHashMap(&(*ParkingLotPtrList)[parkIndex], new_vehicle->licencePlate, hash_table);
    
    // Inserts the new vehicle in the park's registry
    insertVehicleInParkRegistry(&new_vehicle, (*ParkingLotPtrList)[parkIndex]);

    printf("%s %d\n", phVehicleParkingLotName, (*ParkingLotPtrList)[parkIndex]->freeSpaces);
}

/**
 * FUNCTION: vehicleEntryVerifications
 *
 * INPUT: The ParkingLot's Pointer List, a specific parking lot index, a parking lot's name, 
 * a vehicle's licence plate, date and time of arrival, the system's last change and the Hash Map
 * 
 * OUTPUT: Handles all the exception that can occur in welcomeVehicle
*/
int vehicleEntryVerifications(struct parking_lot ***ParkingLotPtrList, int parkIndex, char phVehicleParkingLotName[],
char phLicencePlate[], char* phDate, char* phHour, char* lastSystemChange, Node **hash_table) {

    // Check if parking lot exists
    if (parkIndex == -1) {
        ParkingLotDoesntExistException(phVehicleParkingLotName);
        return 1;
    }

    // Check if parking lot is full
    if ((*ParkingLotPtrList)[parkIndex]->freeSpaces == 0) {
        ParkingLotFilledException(phVehicleParkingLotName);
        return 1;
    }

    // Check if Licence Plate is valid and if the car is already in the park
    if (!(checkLicencePlate(phLicencePlate))) {
        InvalidLicencePlateException(phLicencePlate);
        return 1;
    }

    // Check if the date/hour are valid
    if (!(verifyDateAndHour(lastSystemChange, phDate, phHour))) {
        InvalidDateException();
        return 1;
    }

    // Check if the vehicle is in another parking lot
    if (!(isCarInParkingLot(hash_table, phLicencePlate))) {
        InvalidVehicleEntryException(phLicencePlate);
        return 1;
    }

    return 0;
}

/**
 * FUNCTION: insertVehicleInParkRegistry
 *
 * INPUT: A pointer to a new vehicle and a parking lot
 * 
 * OUTPUT: Inserts a new vehicle pointer on the park's registry
*/
void insertVehicleInParkRegistry(struct vehicle **vehicle, struct parking_lot *lot) {

    // Allocate memory for a new node
    ParkNode *new_node = (ParkNode *)malloc(sizeof(ParkNode));

    // Save the vehicle pointer in the new node
    new_node->vehicle = *vehicle;
    new_node->next = NULL;
    new_node->prev = NULL;
    
    // Insert the node
    if (lot->head == NULL)
    {
        lot->head = new_node;
        lot->tail = new_node;
    } else {
        lot->tail->next = new_node;
        new_node->prev = lot->tail;
        lot->tail = new_node;
    }
}

/** 
 * FUNCTION: checkLicencePlate
 *
 * INPUT: The vehicle's licence plate
 * 
 * OUTPUT: Checks if a Licence Plate is valid or not, returning 1 if valid, 0 otherwise
*/
int checkLicencePlate(char* phLicencePlate) {

    int letterCount = 0;
    int digitCount = 0;

    // Check licence plate length
    int len = strlen(phLicencePlate);

    // Check the pairs
    for (int i = 0; i < len; i += 3) {
        if (isalpha(phLicencePlate[i])) {
            if (!isalpha(phLicencePlate[i + 1]) || islower(phLicencePlate[i]) || islower(phLicencePlate[i + 1])) {
                return 0;
            }
            letterCount += 2;
        } else if (isdigit(phLicencePlate[i])) {
            if (!isdigit(phLicencePlate[i + 1])) {
                return 0;
            }
            digitCount += 2;
        } else {
            return 0; // Invalid character in pair
        }
    }

    // Check if there's at least 1 pair of letters and 1 pair of numbers
    if (letterCount < 2 || digitCount < 2) {
        return 0;
    }

    return 1;
}

/** 
 * FUNCTION: isCarInParkingLot
 *
 * INPUT: The Hash Map and the vehicle's licence plate
 * 
 * OUTPUT: Checks if a Licence Plate is in that park or not
*/
int isCarInParkingLot(Node *hash_table[], char* phLicencePlate) {

    unsigned long hashIndex = hashLicencePlate(phLicencePlate);

    Node *current = hash_table[hashIndex];

    while (current != NULL) {
        if (current != NULL && current->parkingLot != NULL && strcmp(current->licencePlate, phLicencePlate) == 0)
        {
            return 0;
        } else {
            current = current->next;
        }
    }
    return 1;
}

/** 
 * FUNCTION: verifyDateAndHour
 *
 * INPUT: The Parking Lot's last register, the vehicle's Date and Hour of arrival
 * 
 * OUTPUT: Checks if the Date and Hour are valid and if they are up to date, returning True, also refreshes the Parking Lot's
 * last register if True
*/
int verifyDateAndHour(char* phLastSystemChange, char* phDate, char* phHour) {

    // Extract day, month, and year as integers
    int day, month, year;
    sscanf(phDate, "%2d-%2d-%4d", &day, &month, &year);

    // Extract hours and minutes as integers
    int hour, min;
    sscanf(phHour, "%2d:%2d", &hour, &min);

    if (!isValidDate(&day, &month) || !isValidHour(&hour, &min)) {
        return 0;
    }

    // Check if the last change is older than the date and hour provided
    if (strcmp(phLastSystemChange, "") != 0) {
        if (!isNewerTimestamp(phLastSystemChange, &day, &month, &year, &hour, &min))
        {
            return 0;
        }
    }

    // Update the last change
    sprintf(phLastSystemChange, "%s %s", phDate, phHour);
    
    return 1;
}

/** 
 * FUNCTION: isValidMonth
 *
 * INPUT: A month
 * 
 * OUTPUT: Checks if a month is valid without printing
*/
int isValidMonth(int *month) {
    return (*month >= 1 && *month <= 12);
}

/** 
 * FUNCTION: isValidDay
 *
 * INPUT: A day, month
 * 
 * OUTPUT: Checks if a day is valid
*/
int isValidDay(int *day, int *month) {
    if (*day < 1 || *day > 31)
        return 0;

    if ((*month == 4 || *month == 6 || *month == 9 || *month == 11) && *day > 30)
        return 0;

    if (*month == 2 && *day > 28)
        return 0;

    return 1;
}

/** 
 * FUNCTION: isValidDate
 *
 * INPUT: A day, month
 * 
 * OUTPUT: Calls other functions so that in the end it can return if a date is valid or not
*/
int isValidDate(int *day, int *month) {
    if (!isValidMonth(month))
        return 0;

    if (!isValidDay(day, month))
        return 0;

    return 1;
}

/** 
 * FUNCTION: isValidHour
 *
 * INPUT: An amount of hours and minutes
 * 
 * OUTPUT: Checks if the hours and minutes are valid
*/
int isValidHour(int* hour, int* min) {
    // Check if the hour is valid
    if (*hour < 0 || *hour > 23)
        return 0;
    
    // Check if the minutes are valid
    if (*min < 0 || *min > 59)
        return 0;
    
    return 1;
}

/** 
 * FUNCTION: isNewerTimestamp
 *
 * INPUT: The last change of a parking lot, a day, month, year, hour and minutes
 * 
 * OUTPUT: Checks if the date introduced by the user is more recent than the last change to be valid
*/
int isNewerTimestamp(char* phLastSystemChange, int* day, int* month, int* year, int* hour, int* min) {
    // Extract day, month, year, hours and minutes as integers
    int phLastSystemChangeDay, phLastSystemChangeMonth, phLastSystemChangeYear, phLastSystemChangeHour, phLastSystemChangeMin;
    sscanf(phLastSystemChange, "%2d-%2d-%4d %2d:%2d", &phLastSystemChangeDay, &phLastSystemChangeMonth, &phLastSystemChangeYear, &phLastSystemChangeHour, &phLastSystemChangeMin);

    if (*year < phLastSystemChangeYear || (*year == phLastSystemChangeYear && (*month < phLastSystemChangeMonth || (*month == phLastSystemChangeMonth && 
    (*day < phLastSystemChangeDay || (*day == phLastSystemChangeDay && (*hour < phLastSystemChangeHour || 
    (*hour == phLastSystemChangeHour && *min < phLastSystemChangeMin))))))))
        return 0;

    return 1;
}

/** 
 * FUNCTION: cyaVehicle
 *
 * INPUT: The ParkingLot's Pointer List, the number of existent parking lots, a parking lot's name, 
 * a vehicle's licence plate, date and time of departure, the last system change, and the Hash Map
 * 
 * OUTPUT: Prints out the vehicle's licence plate, date and hour of entry/departure
*/
void cyaVehicle(struct parking_lot ***ParkingLotPtrList, int nParkingLots, char phVehicleParkingLotNameOUT[], 
char phLicencePlateOUT[], char* phDateOUT, char* phHourOUT, char* lastSystemChange, Node **hash_table) {

    int i, parkIndex = -1;

    // Identify the park
    for (i = 0; i < nParkingLots; i++) {
        if (strcmp((*ParkingLotPtrList)[i]->name, phVehicleParkingLotNameOUT) == 0) {  
            parkIndex = i;
            break;
        }
    }

    // Exception handler
    if (vehicleExitVerifications(parkIndex, phVehicleParkingLotNameOUT, phLicencePlateOUT, phDateOUT, phHourOUT, lastSystemChange, hash_table))
    {
        return;
    }

    double vehicleBill = 0.00;

    // Looking for the vehicle in the parking lot's linked list of vehicles (registry)
    ParkNode *current = (*ParkingLotPtrList)[parkIndex]->head;
    while(current != NULL) {
        if (strcmp(current->vehicle->licencePlate, phLicencePlateOUT) == 0 && (current->vehicle->bill == 0.00))
        {
            fillVehicleInfo(ParkingLotPtrList, parkIndex, phDateOUT, phHourOUT, &vehicleBill, current);

            // Insert vehicle in parking lot's exit registry
            insertVehicleInExitRegistry(&current->vehicle, (*ParkingLotPtrList)[parkIndex]);
            
            printf("%s %s %s %s %s %.2f\n", phLicencePlateOUT, current->vehicle->dateIn, current->vehicle->hourIn, phDateOUT, phHourOUT, vehicleBill);

            break;
        } else {
            current = current->next;
        }
    }

    ((*ParkingLotPtrList)[i]->freeSpaces)++;

    // Remove vehicle from the hash map
    removeVehicleInHashMap(phLicencePlateOUT, hash_table);
}

/** 
 * FUNCTION: vehicleExitVerifications
 *
 * INPUT: A specific parking lot index, a parking lot's name, 
 * a vehicle's licence plate, date and time of departure, the last system change, and the Hash Map
 * 
 * OUTPUT: Handles all the exception that can occur in cyaVehicle
*/
int vehicleExitVerifications(int parkIndex, char phVehicleParkingLotNameOUT[],
char phLicencePlateOUT[], char* phDateOUT, char* phHourOUT, char* lastSystemChange, Node **hash_table) {

    // Check if the park exists
    if (parkIndex == -1) {
        ParkingLotDoesntExistException(phVehicleParkingLotNameOUT);
        return 1;
    }

    // Check if the licence plate is valid
    if (!(checkLicencePlate(phLicencePlateOUT))) {
        InvalidLicencePlateException(phLicencePlateOUT);
        return 1;
    }

    // Check if the date and hour are valid
    if (!(verifyDateAndHour(lastSystemChange, phDateOUT, phHourOUT))) {
        InvalidDateException();
        return 1;
    }

    // Check if the vehicle is in the park
    if (isCarInParkingLot(hash_table, phLicencePlateOUT)) {
        VehicleNotHereException(phLicencePlateOUT);
        return 1;
    }

    return 0;
}

/** 
 * FUNCTION: fillVehicleInfo
 *
 * INPUT: The ParkingLot's Pointer List, a specific parking lot index, a vehicle's date and time of departure, 
 * the vehicle's bill, node for the parking lot's registry
 * 
 * OUTPUT: Calculates vehicle bill and populates the vehicle with it's exit details
*/
void fillVehicleInfo(struct parking_lot ***ParkingLotPtrList, int parkIndex, char* phDateOUT, char* phHourOUT, double* vehicleBill, ParkNode *current) {

    // Creates the vehicle's bill
    *vehicleBill = vehiclePayment(totalTimeDifference(current->vehicle->dateIn, current->vehicle->hourIn, phDateOUT, phHourOUT), 
    (*ParkingLotPtrList)[parkIndex]->costBeforeOneHour, (*ParkingLotPtrList)[parkIndex]->costAfterOneHour, (*ParkingLotPtrList)[parkIndex]->dailyCost);

    // Populates the vehicle with it's exit details
    strcpy(current->vehicle->dateOut, phDateOUT);
    strcpy(current->vehicle->hourOut, phHourOUT);
    current->vehicle->bill = *vehicleBill;
}

/** 
 * FUNCTION: insertVehicleInExitRegistry
 *
 * INPUT: A pointer to a pointer of a vehicle and a pointer to a specific parking lot
 * 
 * OUTPUT: Inserts a vehicle pointer on the park's exit registry
*/
void insertVehicleInExitRegistry(struct vehicle **vehicle, struct parking_lot *lot) {

    // Allocate memory for a new node
    VehicleLeftNode *new_node = (VehicleLeftNode *)malloc(sizeof(VehicleLeftNode));

    // Save the vehicle pointer in the new node
    new_node->vehicle = *vehicle;
    new_node->next = NULL;
    new_node->prev = NULL;

    // Insert the node
    if (lot->vHead == NULL)
    {
        lot->vHead = new_node;
        lot->vTail = new_node;
    } else {
        lot->vTail->next = new_node;
        new_node->prev = lot->vTail;
        lot->vTail = new_node;
    }
}

/** 
 * FUNCTION: parseDateTime
 *
 * INPUT: A date and an hour and parses the day/month/year/hour/minute
 * 
 * OUTPUT: Parses the time and date into variables, without printing anything
*/
void parseDateTime(char *date_str, char *time_str, int *day, int *month, int *year, int *hour, int *minute) {
    sscanf(date_str, "%d-%d-%d", day, month, year);
    sscanf(time_str, "%d:%d", hour, minute);
}

/** 
 * FUNCTION: totalTimeDifference
 *
 * INPUT: Two pairs of dates and hours
 * 
 * OUTPUT: Returns the amount of time in minutes between both dates/hours
*/
int totalTimeDifference(char *date_str1, char *time_str1, char *date_str2, char *time_str2) {

    int day1, month1, year1, hour1, minute1;
    int day2, month2, year2, hour2, minute2;

    parseDateTime(date_str1, time_str1, &day1, &month1, &year1, &hour1, &minute1);
    parseDateTime(date_str2, time_str2, &day2, &month2, &year2, &hour2, &minute2);

    int totalDays1 = 0;
    totalDays1 += day1;
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    for (int i = month1-1; i > 0; i--)
    {
        totalDays1 += daysInMonth[i];
    }
    totalDays1 += year1 * 365;
    int minTotal1 = totalDays1 * 24 * 60;
    minTotal1 += hour1 * 60;
    minTotal1 += minute1;

    int totalDays2 = 0;
    totalDays2 += day2;
    for (int i = month2-1; i > 0; i--)
    {
        totalDays2 += daysInMonth[i];
    }
    totalDays2 += year2 * 365;
    int minTotal2 = totalDays2 * 24 * 60;
    minTotal2 += hour2 * 60;
    minTotal2 += minute2;

    int diffMin = minTotal2 - minTotal1;
    return diffMin;
}

/** 
 * FUNCTION: vehiclePayment
 *
 * INPUT: An amount of time in minutes and the three fees a parking lot has
 * 
 * OUTPUT: Returns how much the vehicle has to pay
*/
double vehiclePayment(int timeDifferenceMin, double costBeforeOneHour, double costAfterOneHour, double dailyCost) {
    
    int days = timeDifferenceMin / (24 * 60);
    int hours = (timeDifferenceMin % (24 * 60)) / 60;
    int remainingMinutes = timeDifferenceMin % 60;

    double totalCost = 0.00;
    double hourCost = 0.00;

    totalCost += dailyCost * days;

    if (hours == 1) {
        hourCost += (4 * costBeforeOneHour) + (remainingMinutes / 15) * costAfterOneHour;
        if ((remainingMinutes % 15) > 0) {
            hourCost += costAfterOneHour;
        }
    } else if (hours > 1) {
        hourCost += (4 * costBeforeOneHour) + ((hours - 1) * (4 * costAfterOneHour)) + (remainingMinutes / 15) * costAfterOneHour;
        if ((remainingMinutes % 15) > 0) {
            hourCost += costAfterOneHour;
        }
    } else {
        hourCost += (remainingMinutes / 15) * costBeforeOneHour;
        if ((remainingMinutes % 15) > 0) {
            hourCost += costBeforeOneHour;
        }
    }

    totalCost += (hourCost >= dailyCost) ? dailyCost : hourCost;

    return (double)(((int)(((totalCost)*100+0.5))/100.0));
}

/** 
 * FUNCTION: listVehicleHistory
 *
 * INPUT: The ParkingLot's Pointer List, the number of existent parking lots, a vehicle's licence plate and a flag
 * 
 * OUTPUT: Lists the vehicle's parking lot history ordered by the parking lot names and date of arrival
*/
void listVehicleHistory(struct parking_lot ***ParkingLotPtrList, int *nParkingLots, char *phLicencePlateSearch, int *parkingSorted) {

    if (!checkLicencePlate(phLicencePlateSearch)) {
        InvalidLicencePlateException(phLicencePlateSearch);
        return;
    }

    int found = 0;
    
    // Sort parks by parking lot name
    if ((*parkingSorted) != 1) {
        bubbleSortByName(&(*ParkingLotPtrList), (*nParkingLots));
        (*parkingSorted) = 1;
    }

    for (int i = 0; i < (*nParkingLots); i++)
    {
        ParkNode *current = (*ParkingLotPtrList)[i]->head;
        while (current != NULL)
        {
            if (strcmp(current->vehicle->licencePlate, phLicencePlateSearch) == 0)
            {
                if (strcmp(current->vehicle->dateOut, "") != 0)
                {
                    printf("%s %s %s %s %s\n", (*ParkingLotPtrList)[i]->name, current->vehicle->dateIn, 
                    current->vehicle->hourIn, current->vehicle->dateOut, current->vehicle->hourOut);
                    found = 1;
                } else {
                    printf("%s %s %s\n", (*ParkingLotPtrList)[i]->name, current->vehicle->dateIn, current->vehicle->hourIn);
                    found = 1;
                }
                current = current->next;
            } else {
                current = current->next;
            }
        }
    }

    if (!found) {
        NoEntriesFoundException(phLicencePlateSearch);
    }
}

/** 
 * FUNCTION: showParkBillingWithDate
 *
 * INPUT: The ParkingLot's Pointer List, the number of parking lots, a parking lot name and date
 * 
 * OUTPUT: Shows the parking lot's billing for the date the user chose
*/
void showParkBillingWithDate(struct parking_lot ***ParkingLotPtrList, int nParkingLots, 
char *phParkingLotNameBilling, char *phDateBilling, char* lastSystemChange) {

    int foundPark = 0;
    int i;

    for (i = 0; i < nParkingLots; i++) {
        if (strcmp(phParkingLotNameBilling, (*ParkingLotPtrList)[i]->name) == 0) {
            foundPark = 1;
            break;
        }
    }
    
    if (!foundPark)
    {
        ParkingLotDoesntExistException(phParkingLotNameBilling);
        return;
    }

    int day, month, year;
    int hours = 0, minutes = 0;
    sscanf(phDateBilling, "%2d-%2d-%4d", &day, &month, &year);

    if (isNewerTimestamp(lastSystemChange, &day, &month, &year, &hours, &minutes) || (*ParkingLotPtrList)[i]->vHead == NULL)
    {
        InvalidDateException();
        return;
    }

    VehicleLeftNode *current = (*ParkingLotPtrList)[i]->vHead;
    while(current != NULL)  {
        if (strcmp(current->vehicle->dateOut, phDateBilling) == 0)
        {
            printf("%s %s %.2f\n", current->vehicle->licencePlate, current->vehicle->hourOut, current->vehicle->bill);
            current = current->next;
        } else {
            current = current->next;
        }
    }
}

/** 
 * FUNCTION: showParkBilling
 *
 * INPUT: The ParkingLot's Pointer List, the number of existent parking lots and a specific parking lot name
 * 
 * OUTPUT: Shows the parking lot's billing, for every day it had vehicles
*/
void showParkBilling(struct parking_lot ***ParkingLotPtrList, int nParkingLots, char *phParkingLotNameBilling) {

    // Check if the parking lot exists
    int found = 0;
    int i;
    for (i = 0; i < nParkingLots; i++) {
        if (strcmp(phParkingLotNameBilling, (*ParkingLotPtrList)[i]->name) == 0) {
            found = 1;
            break;
        }
    }

    if (!found) {
        ParkingLotDoesntExistException(phParkingLotNameBilling);
        return;
    }

    if ((*ParkingLotPtrList)[i]->vHead == NULL)
    {
        return;
    }

    VehicleLeftNode *current = (*ParkingLotPtrList)[i]->vHead;
    double dailyBilling = 0;
    char *date = current->vehicle->dateOut;
    while (current != NULL)
    {
        if (current->next != NULL)
        {
            if (strcmp(date, current->next->vehicle->dateOut) == 0)
            {
                dailyBilling += current->vehicle->bill;
                current = current->next;
            } else if (strcmp(date, current->next->vehicle->dateOut) != 0)
            {
                dailyBilling += current->vehicle->bill;
                printf("%s %.2f\n", date, dailyBilling);
                dailyBilling = 0;
                current = current->next;
                date = current->vehicle->dateOut;
            }
        } else {
            if (current->prev != NULL)
            {
                if (strcmp(date, current->prev->vehicle->dateOut) == 0)
                {
                    dailyBilling += current->vehicle->bill;
                    printf("%s %.2f\n", date, dailyBilling);
                    current = current->next;
                } else {
                    date = current->vehicle->dateOut;
                    dailyBilling = current->vehicle->bill;
                    printf("%s %.2f\n", date, dailyBilling);
                    current = current->next;
                }
            } else {
                dailyBilling += current->vehicle->bill;
                printf("%s %.2f\n", date, dailyBilling);
                break;
            }
        }
    }
}

/** 
 * FUNCTION: removeParkingLot
 *
 * INPUT: The ParkingLot's Pointer List, the number of existent parking lots, a flag and the Hash Map
 * 
 * OUTPUT: Zaps a parking lot from existence (also removes vehicles that were in that parking lot)
*/
void removeParkingLot(struct parking_lot ***ParkingLotPtrList, int *nParkingLots, char *phParkingLotNameRemoval, 
int *parkingSorted, Node **hash_table) {
    int found = 0;
    int i;
    // Finds the parking lot
    for (i = 0; i < (*nParkingLots); i++) {
        if (strcmp(phParkingLotNameRemoval, (*ParkingLotPtrList)[i]->name) == 0) {
            found = 1;
            break;
        }
    }

    // Check if the parking lot exists
    if (!found)
    {
        ParkingLotDoesntExistException(phParkingLotNameRemoval);
        return;
    }

    // Free the nodes that we used to register the exists from the parking lot
    VehicleLeftNode *vehicleRexit = (*ParkingLotPtrList)[i]->vTail;
    while (vehicleRexit != NULL) {
        if (vehicleRexit->prev != NULL)
        {
            vehicleRexit = vehicleRexit->prev;
            free(vehicleRexit->next);
        } else {
            free(vehicleRexit);
            break;
        }
    }

    // Free the nodes that we used to register vehicles from the parking lot
    ParkNode *vehicleRall = (*ParkingLotPtrList)[i]->tail;
    while (vehicleRall != NULL) {
        if (vehicleRall->prev != NULL)
        {
            removeVehicleInHashMap(vehicleRall->vehicle->licencePlate, hash_table);
            free(vehicleRall->vehicle->parking_lotName);
            free(vehicleRall->vehicle);
            vehicleRall = vehicleRall->prev;
            free(vehicleRall->next);
        } else {
            removeVehicleInHashMap(vehicleRall->vehicle->licencePlate, hash_table);
            free(vehicleRall->vehicle->parking_lotName);
            free(vehicleRall->vehicle);
            free(vehicleRall);
            break;
        }
    }

    free((*ParkingLotPtrList)[i]->name);
    free((*ParkingLotPtrList)[i]);

    for (int j = i; j < (*nParkingLots) - 1; j++) {
        (*ParkingLotPtrList)[j+1]->index -= 1;
        (*ParkingLotPtrList)[j] = (*ParkingLotPtrList)[j + 1];
    }
    (*nParkingLots)--;

    if (!(*parkingSorted)) {
        bubbleSortByName(ParkingLotPtrList, (*nParkingLots));
        (*parkingSorted) = 1;
    }

    for (i = 0; i < (*nParkingLots); i++) {
        printf("%s\n", (*ParkingLotPtrList)[i]->name);
    }
}

/** 
 * FUNCTION: bubbleSortByName
 *
 * INPUT: The ParkingLot's Pointer List and the number of existent parking lots
 * 
 * OUTPUT: Orders the Parking Lot Map by their names, with a Bubble Sort Algorithm
*/
void bubbleSortByName(struct parking_lot ***ParkingLotPtrList, int numParkingLots) {

    for (int i = 0; i < numParkingLots - 1; i++) {
        // Initialize a flag to track if any swaps were made in this pass
        int swapped = 0;
        
        // Reduce the range of comparison in each pass
        for (int j = 0; j < numParkingLots - i - 1; j++) {
            if (strcmp((*ParkingLotPtrList)[j]->name, (*ParkingLotPtrList)[j + 1]->name) > 0) {
                // Swap the parking lots
                struct parking_lot *temp = (*ParkingLotPtrList)[j];
                (*ParkingLotPtrList)[j] = (*ParkingLotPtrList)[j + 1];
                (*ParkingLotPtrList)[j + 1] = temp;
                swapped = 1; // Set the flag to indicate a swap occurred
            }
        }
        
        // If no swap occurred in this pass, the array is already sorted
        if (!swapped) {
            break;
        }
    }
}

/* 
 * FUNCTION: bubbleSortByIndex
 *
 * INPUT: The ParkingLot's Pointer List and the number of existent parking lots
 * 
 * OUTPUT: Orders the Parking Lot Map by their index attribute, with a Bubble Sort Algorithm
 */
void bubbleSortByIndex(struct parking_lot ***ParkingLotPtrList, int numParkingLots) {

    for (int i = 0; i < numParkingLots - 1; i++) {
        // Initialize a flag to track if any swaps were made in this pass
        int swapped = 0;
        
        // Reduce the range of comparison in each pass
        for (int j = 0; j < numParkingLots - i - 1; j++) {
            if ((*ParkingLotPtrList)[j]->index > (*ParkingLotPtrList)[j + 1]->index) {
                // Swap the parking lots
                struct parking_lot *temp = (*ParkingLotPtrList)[j];
                (*ParkingLotPtrList)[j] = (*ParkingLotPtrList)[j + 1];
                (*ParkingLotPtrList)[j + 1] = temp;
                swapped = 1; // Set the flag to indicate a swap occurred
            }
        }
        
        // If no swap occurred in this pass, the array is already sorted
        if (!swapped) {
            break;
        }
    }
}


/* Exception Functions */

void ParkingLotAlreadyExistsException(char* phParkingLotName) {
    printf("%s: parking already exists.\n", phParkingLotName);
}

void InvalidParkingLotException(int phMaxCapacity) {
    printf("%d: invalid capacity.\n", phMaxCapacity);
}

void InvalidCostException() {
    printf("invalid cost.\n");
}

void TooManyParkingLotsException() {
    printf("too many parks.\n");
}

void ParkingLotDoesntExistException(char* phParkingLotName) {
    printf("%s: no such parking.\n", phParkingLotName);
}

void ParkingLotFilledException(char* phParkingLotName) {
    printf("%s: parking is full.\n", phParkingLotName);
}

void InvalidLicencePlateException(char* phLicencePlate) {
    printf("%s: invalid licence plate.\n", phLicencePlate);
}

void InvalidVehicleEntryException(char* phLicencePlate) {
    printf("%s: invalid vehicle entry.\n", phLicencePlate);
}

void InvalidDateException() {
    printf("invalid date.\n");
}

void VehicleNotHereException(char* phLicencePlate) {
    printf("%s: invalid vehicle exit.\n", phLicencePlate);
}

void NoEntriesFoundException(char* phLicencePlate) {
    printf("%s: no entries found in any parking.\n", phLicencePlate);
}
