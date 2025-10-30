/*2. Definirati strukturu osoba(ime, prezime, godina rođenja) i napisati program koji :
* 
A.dinamički dodaje novi element na početak liste,
B.ispisuje listu,
C.dinamički dodaje novi element na kraj liste,
D.pronalazi element u listi(po prezimenu),
E.briše određeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.

3. Prethodnom zadatku dodati funkcije :

A.dinamički dodaje novi element iza određenog elementa,
B.dinamički dodaje novi element ispred određenog elementa,
C.sortira listu po prezimenima osoba,
D.upisuje listu u datoteku,*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#define N 50
#define SCANF_ERROR -3
#define MALLOC_ERROR -2
#define EXIT_SUCCESS 0
// 3 zadatak
#define FILE_ERROR_OPEN -1


struct person; //predeklaracija strukture da kompajler zna da postoji
typedef struct person* Position; //dodavanje aliasa Position "struct person*"

typedef struct person { //deklaracija strukture s elementima, person-ime tipa strukture

    char firstName[N];
    char lastName[N];
    int birthYear;
    Position next; //pokazivac next na sljedecu osobu na listi

} Person; //zamjena za "struct person", novi tip


Position createPerson();
int addToFront(Position);
int printList(Position);
int addToEnd(Position);
Position findPerson(Position, char*);
Position findPersonPrev(Position, char*);
int deletePerson(Position, char*);
int deleteList(Position);

// 3 zadatak
int addAfter(Position, char*);
int addBefore(Position, char*);
int sortList(Position);
int fileWrite(Position);
int fileRead(Position);


int main()
{
    char in = 0;                 // Varijabla za unos opcije korisnika (f, e, p, s, d, 0)
    char surname[N];       // Polje za unos prezimena za pretragu ili brisanje
    Person head = { .next = NULL, .firstName = {0}, .lastName = {0}, .birthYear = 0 };
    // Dummy osoba na pocetku liste
    // head.next pokazuje na prvu stvarnu osobu u listi



    printf("f - add person to front of the list\ne - add person to end of the list\n");
    printf("p - print list\ns - find person with surname\nd - delete person with surname\n0 - exit\n");

    do
    {
        // Trazi od korisnika unos opcije
        printf("\nChoose action: ");
        if (scanf(" %c", &in) != 1) return SCANF_ERROR;

        switch (in)
        {
        case 'f': // Dodavanje osobe na pocetak liste
            printf("Add to front of the list\n");
            addToFront(&head);
            break;
        case 'e': // Dodavanje osobe na kraj liste
            printf("Add to end of the list\n");
            addToEnd(&head);
            break;
        case 'p': // Ispis svih osoba u listi
            printf("Printing list:\n");
            printList(head.next); // Prosljedjuje pokazivac na prvu stvarnu osobu
            break;
        case 's': // Pretraga osobe po prezimenu
            printf("Find person with surname: ");
            if (scanf(" %s", surname) != 1) return SCANF_ERROR; // Unos prezimena
            findPerson(head.next, surname) ? // Ako osoba postoji
                printf("\t%s %s %d\n", findPerson(head.next, surname)->firstName,
                    findPerson(head.next, surname)->lastName,
                    findPerson(head.next, surname)->birthYear) : // Ispis te osobe
                printf("\tPerson with surname '%s' was not found.\n", surname); // Poruka ako nije pronadena
            break;
        case 'd': // Brisanje osobe po prezimenu
            printf("Delete person with surname: ");
            if (scanf(" %s", surname) != 1) return SCANF_ERROR; // Unos prezimena
            deletePerson(&head, surname); // Poziva funkciju koja brise osobu iz liste
            break;
        case '0': // Izlazak iz programa
            break;
        default: // Nevazeci unos
            printf("\tInvalid input. Try again.\n");
            break;
        }
    } while (in != '0'); // Petlja se ponavlja dok korisnik ne unese '0'

    deleteList(&head); // Oslobadjanje memorije svih osoba prije zavrsetka programa

    return EXIT_SUCCESS; // Normalan zavrsetak programa
}


Position createPerson() //pomocna funkcija za dodavanje novih osoba na listu
{
    Position person = NULL; //postavljamo pokazivac na NULL da znamo da jos ne pokazuje nigdje
    person = (Position)malloc(sizeof(Person));//alokacija memorije
    if (!person) return NULL;//provjera je li malloc uspio

    printf("\tName: ");
    if (scanf(" %s", person->firstName) != 1) return SCANF_ERROR;

    printf("\tSurname: ");
    if (scanf(" %s", person->lastName) != 1) return SCANF_ERROR;

    printf("\tBirth year: ");
    if (scanf(" %d", &person->birthYear) != 1) return SCANF_ERROR;

    person->next = NULL;
    return person;
}


int addToFront(Position head) {

    Position newPerson = NULL;
    newPerson = createPerson(); //stvaranje strukture
    if (!newPerson) {
        printf("malloc error");
        return MALLOC_ERROR;
    }

    newPerson->next = head->next; //nova osoba sada pokazuje na trenutnu prvu osobu 
    head->next = newPerson;  //head sada pokazuje na novu osobu
    return EXIT_SUCCESS;

}


int printList(Position current) {
    if (current == NULL) {  // ispis ako je list prazna 
        printf("empty list");
        return EXIT_SUCCESS;

    }

    while (current != NULL) {  //prolazak kroz listu do kraja i ispis podataka svake osobe
        printf("\t%s %s %d\n", current->firstName, current->lastName, current->birthYear);

        current = current->next; //prelazak na sljedecu osobu u listi
    }


    return EXIT_SUCCESS;
}


int addToEnd(Position head) {

    Position newPerson = NULL;
    newPerson = createPerson();
    if (!newPerson) {
        printf("Malloc error!\n");
        return MALLOC_ERROR;
    }

    while (head->next != NULL) {
        head = head->next;
    }

    head->next = newPerson; //head sada pokazuje na novu osobu
    newPerson->next = NULL; //nova osoba sada pokazuje na kraj liste (NULL)

    return EXIT_SUCCESS;

}


Position findPerson(Position current, char* surname) {
    while (current != NULL && strcmp(current->lastName, surname)) //prolazimo do kraja i trazimo podudaranje
        current = current->next;  //pomicemo pokazivac na sljedecu osobu

    return current;
}


Position findPersonPrev(Position current, char* surname) {
    //prolazimo kroz listu dok ne pronadjemo prethodnika osobe koju zelimo obrisati
    while (current->next != NULL && strcmp(current->next->lastName, surname))
        current = current->next;

    if (current->next == NULL)
        return NULL;
    return current;
}


int deletePerson(Position head, char* surname) {

    Position temp = NULL, prev = NULL;

    prev = findPersonPrev(head, surname); //trazimo osobu prije one koju zelimo obrisati

    if (prev != NULL) {
        temp = prev->next;  // osoba koju cemo obrisati
        prev->next = temp->next;  // preskacemo tu osobu u listi
        free(temp);
        printf("\tperson deleted\n");


    }
    else printf("\tperson with last name %s wasnt found\n", surname);

    return EXIT_SUCCESS;

}


int deleteList(Position head) {
    Position temp = NULL;
    // prolazimo kroz cijelu listu i oslobadjamo sve osobe
    while (head->next != NULL) {
        temp = head->next;
        head->next = head->next->next; // preskacemo osobu
        free(temp);  // oslobadjamo memoriju


    }

    return EXIT_SUCCESS;
}

// 3 zadatak
// dodavanje nove osobe nakon osobe s određenim prezimenom
int addAfter(Position head, char* surname) {
    Position newPerson = NULL, personSurname = NULL;

    personSurname = findPerson(head, surname); // pronalazak ciljne osobe

    if (personSurname != NULL) {
        newPerson = createPerson();
        if (!newPerson) {
            printf("malloc error\n");
            return MALLOC_ERROR;
        }

        newPerson->next = personSurname->next; // povezivanje nove osobe
        personSurname->next = newPerson;

    }

    else printf("\tperson with surname %s not found\n", surname);

    return EXIT_SUCCESS;

}

// dodavanje nove osobe prije osobe s određenim prezimenom
int addBefore(Position head, char* surname) {
    Position newPerson = NULL, personSurname = NULL, personPrevious=NULL;

    personSurname = findPerson(head, surname); // pronalazak ciljane osobe
    personPrevious = findPersonPrev(head, surname); // pronalazak prethodne osobe
    

    if (personSurname != NULL) {
        newPerson = createPerson();
        if (!newPerson) {
            printf("malloc error\n");
            return MALLOC_ERROR;
        }

        personPrevious->next=newPerson; // povezivanje nove osobe
        newPerson->next = personSurname;
       

    }

    else printf("\tperson with surname %s not found\n", surname);

    return EXIT_SUCCESS;

}

// sortiranje liste po prezimenima
int sortList(Position head)
{
    Position i, j, prev;
    Position end = NULL;

    if (head->next == NULL || head->next->next == NULL) // prazna ili samo jedna osoba
        return EXIT_SUCCESS;

    int swapped;

    do {
        swapped = 0;
        prev = head;
        i = head->next;

        while (i->next != end) {
            j = i->next;

            // Pretvoranje prezimena u velika slova za usporedbu
            char lname_i[N], lname_j[N];
            strcpy(lname_i, i->lastName);
            strcpy(lname_j, j->lastName);

            for (int k = 0; lname_i[k]; k++) lname_i[k] = toupper(lname_i[k]);
            for (int k = 0; lname_j[k]; k++) lname_j[k] = toupper(lname_j[k]);

            if (strcmp(lname_i, lname_j) > 0) {
                // Zamjena čvorova i i j
                i->next = j->next;
                j->next = i;
                prev->next = j;

                swapped = 1;
                prev = j; // novi prethodnik
            }
            else {
                prev = i;
                i = i->next;
            }
        }
        end = i; // posljednji element je sada na svom mjestu
    } while (swapped);

    return EXIT_SUCCESS;
}

// spremanje liste u datoteku
int fileWrite(Position current) {
    FILE* filePointer = NULL;
    filePointer = fopen("persons.txt", "w"); // otvaranje datoteke za pisanje


    if (filePointer == NULL) {
        printf("\n file couldnt open\n");
        return FILE_ERROR_OPEN;
    }

    while (current != NULL) {
        fprintf(filePointer, "%s %s %d\n", current->firstName, current->lastName, current->birthYear);
        current=current->next;
    }
    fclose(filePointer);

    printf("\tsuccessfull\n");

    return EXIT_SUCCESS;


}

// učitavanje liste iz datoteke
int fileRead(Position head)
{
    FILE* filePointer = NULL;
    filePointer = fopen("persons.txt", "r"); // otvaranje datoteke za čitanje


    if (!filePointer) {
        printf("File could not be opened.\n");
        return FILE_ERROR_OPEN;
    }

    Position prev = NULL, current = NULL;

    deleteList(head); // brisanje trenutne liste prije učitavanja nove
    prev = head;

    while (!feof(filePointer))
    {
        current = (Position)malloc(sizeof(Person)); // alokacija memorije
        if (!current) {
            printf("Malloc error!\n");
            return MALLOC_ERROR;
        }
        if (fscanf(filePointer, " %s %s %d ", current->firstName, current->lastName, &current->birthYear) != 3)
            return SCANF_ERROR;

        
        current->next = prev->next; // umetanje na kraj liste
        prev->next = current;

        prev = current; // pomak prethodnog pokazivača
    }

    fclose(filePointer);

    return EXIT_SUCCESS;
}