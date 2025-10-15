/* Napisati program koji prvo pročita koliko redaka ima datoteka, tj. koliko ima studenata
zapisanih u datoteci. Nakon toga potrebno je dinamički alocirati prostor za niz struktura
studenata (ime, prezime, bodovi) i učitati iz datoteke sve zapise. Na ekran ispisati ime,
prezime, apsolutni i relativni broj bodova.
Napomena: Svaki redak datoteke sadrži ime i prezime studenta, te broj bodova na kolokviju.
relatvan_br_bodova = br_bodova/max_br_bodova*100 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define N 50

//sturcture with student info (name,surname and points)
typedef struct {
	char firstName[N];
	char lastName[N];
	double points;
} Student;

//function for counting lines (students)
int countLines() {
	FILE* file = fopen("list.txt", "r"); //opening file for reading
	if (!file) {
		printf("Error:file couldnt open\n");
		return 0;
	}

	int lines = 0;//counter of lines
	char buffer[200];//empty buffer

	while (fgets(buffer, sizeof(buffer), file)){ //reading line by line
		lines++;//increasing counter
	}
	fclose(file);//closing the file
	return lines;
	
}


void loadStudents(Student* students, int count) {
	FILE* file = fopen("list.txt", "r"); //opening file for reading
	if (!file) {
		printf("Error:file couldnt open\n");
		return 0;
	}
	int i;
	for (i = 0;i < count;i++) { //reading the file and storing info in the students array
		fscanf(file, "%s %s %lf", students[i].firstName, students[i].lastName, &students[i].points);
	}

	fclose(file); //closing the file

}

// Function to calculate relative points (percentage)
double calculatePercentage(double points) { 
	return (points / N) * 100; 
}

void printStudents(Student* students, int count) {
	printf("FirstName  LastName  Points  Relative\n");
	printf("------------------------------------\n");

	for (int i = 0; i < count; i++) {
		double rel = calculatePercentage(students[i].points);
		printf("%-10s %-10s %-6.2lf %-6.2lf%%\n",
			students[i].firstName, students[i].lastName,
			students[i].points, rel);
	}
}


int main() {

	int lineCount = countLines();
	if (lineCount == 0) {
		printf("The file is empty or cannot be opened.\n");
		return -1;
	}


	Student* list = (Student*)malloc(lineCount * sizeof(Student)); //dinamic allocation 
	if (!list) {
		printf("Error: memory allocation failed\n");
		return -1;
	}


	loadStudents(list, lineCount); //loading students from filwe

	printStudents(list, lineCount); //printing on the screen

	free(list); //free allocated memory

	return 0;
}






















