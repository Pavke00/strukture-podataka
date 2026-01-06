#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// struktura stabla gradova
struct cityTree;
typedef struct cityTree* CityTreePosition;
typedef struct cityTree {
    int population;
    char name[20];
    CityTreePosition left;
    CityTreePosition right;
} CityTree;

// struktura vezane liste drzava
struct countryList;
typedef struct countryList* CountryListPosition;
typedef struct countryList {
    char name[20];
    CityTreePosition cities;   // stablo gradova
    CountryListPosition next;
} CountryList;

// struktura vezane liste gradova
struct cityList;
typedef struct cityList* CityListPosition;
typedef struct cityList {
    int population;
    char name[20];
    CityListPosition next;
} CityList;

// struktura stabla drzava
struct countryTree;
typedef struct countryTree* CountryTreePosition;
typedef struct countryTree {
    char name[20];
    CountryTreePosition left;
    CountryTreePosition right;
    CityList cityHead;         // glava liste gradova
} CountryTree;

// prototipi funkcija
int addCountryToList(CountryListPosition current, CountryListPosition newCountry);
CountryTreePosition addCountryToTree(CountryTreePosition current, CountryTreePosition newCountry);
int addCityToList(CityListPosition current, CityListPosition newCity);
CityTreePosition addCityToTree(CityTreePosition current, CityTreePosition newCity);
int printCityTree(CityTreePosition current);
int printCountryList(CountryListPosition current);
int printCityList(CityListPosition current);
int printCountryTree(CountryTreePosition current);
CountryTreePosition findCountryByName(CountryTreePosition current, char* name);
void trim(char* s);

int main()
{
    // dummy glava liste drzava
    CountryList countryListHead = { "", NULL, NULL };

    CountryTreePosition countryTree = NULL;
    CountryTreePosition countryChoice = NULL;
    CityListPosition firstCity = NULL;

    char country[20] = { 0 };
    char countryFilePath[30] = { 0 };
    char city[20] = { 0 };
    char countryNameChoice[20] = { 0 };

    int population = 0;
    int populationChoice = 0;

    FILE* filePointer = fopen("drzave.txt", "r");
    FILE* countryFilePointer = NULL;

    // provjera otvaranja glavne datoteke
    if (filePointer == NULL) {
        printf("greska pri otvaranju datoteke drzave.txt\n");
        return -1;
    }

    // citanje drzava iz datoteke
    while (fscanf(filePointer, "%[^,], %s", country, countryFilePath) == 2)
    {
        trim(country);
        // alokacija novog cvora liste drzava
        CountryListPosition newCountryList = malloc(sizeof(CountryList));
        strcpy(newCountryList->name, country);
        newCountryList->cities = NULL;
        newCountryList->next = NULL;

        // alokacija novog cvora stabla drzava
        CountryTreePosition newCountryTree = malloc(sizeof(CountryTree));
        strcpy(newCountryTree->name, country);
        newCountryTree->left = NULL;
        newCountryTree->right = NULL;

        // inicijalizacija glave liste gradova
        newCountryTree->cityHead.population = 0;
        newCountryTree->cityHead.name[0] = '\0';
        newCountryTree->cityHead.next = NULL;

        // dodavanje drzave u listu i stablo
        addCountryToList(&countryListHead, newCountryList);
        countryTree = addCountryToTree(countryTree, newCountryTree);

        // otvaranje datoteke gradova
        countryFilePointer = fopen(countryFilePath, "r");
        if (countryFilePointer == NULL) {
            printf("greska pri otvaranju %s\n", countryFilePath);
            return -1;
        }

        // citanje gradova
        while (fscanf(countryFilePointer, "%[^,], %d", city, &population) == 2)
        {
            // novi grad za listu
            CityListPosition newCityList = malloc(sizeof(CityList));
            strcpy(newCityList->name, city);
            newCityList->population = population;
            newCityList->next = NULL;

            // novi grad za stablo
            CityTreePosition newCityTree = malloc(sizeof(CityTree));
            strcpy(newCityTree->name, city);
            newCityTree->population = population;
            newCityTree->left = NULL;
            newCityTree->right = NULL;

            // dodavanje grada
            addCityToList(&newCountryTree->cityHead, newCityList);
            newCountryList->cities = addCityToTree(newCountryList->cities, newCityTree);
        }

        fclose(countryFilePointer);
    }

    fclose(filePointer);

    // ispis liste drzava
    printf("lista drzava:\n----------------\n");
    printCountryList(countryListHead.next);

    // ispis stabla drzava
    printf("\nstablo drzava:\n----------------\n");
    printCountryTree(countryTree);

    // korisnicka pretraga
    while (1)
    {
        printf("\nunesi drzavu (0 za izlaz): ");
        scanf(" %19[^\n]", countryNameChoice);

        if (strcmp(countryNameChoice, "0") == 0)
            break;

        // trazenje drzave u stablu
        countryChoice = findCountryByName(countryTree, countryNameChoice);
        if (countryChoice == NULL) {
            printf("drzava ne postoji\n");
            continue;
        }

        printf("unesi broj stanovnika: ");
        scanf("%d", &populationChoice);

        // preskoci gradove s manjom ili jednakom populacijom
        firstCity = countryChoice->cityHead.next;
        while (firstCity != NULL && firstCity->population <= populationChoice)
            firstCity = firstCity->next;

        if (firstCity == NULL)
            printf("nema takvih gradova\n");
        else {
            printf("gradovi s vise stanovnika:\n");
            printCityList(firstCity);
        }
    }

    return 0;
}

void trim(char* s)
{
    int len = strlen(s);
    if (len > 0 && s[len - 1] == '\r')
        s[len - 1] = '\0';
}


// dodavanje drzave u sortiranu listu
int addCountryToList(CountryListPosition current, CountryListPosition newCountry)
{
    while (current->next != NULL && strcmp(newCountry->name, current->next->name) > 0)
        current = current->next;

    newCountry->next = current->next;
    current->next = newCountry;

    return 0;
}

// dodavanje drzave u stablo
CountryTreePosition addCountryToTree(CountryTreePosition current, CountryTreePosition newCountry)
{
    if (current == NULL)
        return newCountry;

    if (strcmp(newCountry->name, current->name) < 0)
        current->left = addCountryToTree(current->left, newCountry);
    else if (strcmp(newCountry->name, current->name) > 0)
        current->right = addCountryToTree(current->right, newCountry);

    return current;
}

// dodavanje grada u sortiranu listu
int addCityToList(CityListPosition current, CityListPosition newCity)
{
    while (current->next != NULL && newCity->population > current->next->population)
        current = current->next;

    if (current->next != NULL && current->next->population == newCity->population) {
        while (current->next != NULL &&
            strcmp(newCity->name, current->next->name) > 0)
            current = current->next;
    }

    newCity->next = current->next;
    current->next = newCity;

    return 0;
}

// dodavanje grada u stablo
CityTreePosition addCityToTree(CityTreePosition current, CityTreePosition newCity)
{
    if (current == NULL)
        return newCity;

    if (newCity->population < current->population)
        current->left = addCityToTree(current->left, newCity);
    else if (newCity->population > current->population)
        current->right = addCityToTree(current->right, newCity);
    else {
        if (strcmp(newCity->name, current->name) < 0)
            current->left = addCityToTree(current->left, newCity);
        else
            current->right = addCityToTree(current->right, newCity);
    }

    return current;
}

// ispis stabla gradova
int printCityTree(CityTreePosition current)
{
    if (current == NULL)
        return 0;

    printCityTree(current->left);
    printf("\t%s, %d\n", current->name, current->population);
    printCityTree(current->right);

    return 0;
}

// ispis liste drzava
int printCountryList(CountryListPosition current)
{
    while (current != NULL) {
        printf("%s\n", current->name);
        printCityTree(current->cities);
        current = current->next;
    }
    return 0;
}

// ispis liste gradova
int printCityList(CityListPosition current)
{
    while (current != NULL) {
        printf("\t%s, %d\n", current->name, current->population);
        current = current->next;
    }
    return 0;
}

// ispis stabla drzava
int printCountryTree(CountryTreePosition current)
{
    if (current == NULL)
        return 0;

    printCountryTree(current->left);
    printf("%s\n", current->name);
    printCityList(current->cityHead.next);
    printCountryTree(current->right);

    return 0;
}

// trazenje drzave po imenu
CountryTreePosition findCountryByName(CountryTreePosition current, char* name)
{
    if (current == NULL)
        return NULL;

    if (strcmp(name, current->name) < 0)
        return findCountryByName(current->left, name);
    else if (strcmp(name, current->name) > 0)
        return findCountryByName(current->right, name);

    return current;
}
