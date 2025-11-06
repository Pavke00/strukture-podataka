/*
Napisati program koji iz datoteke cita postfiks izraz
i zatim koristenjem stoga racuna rezultat tog izraza.
Stog je potrebno realizirati preko vezane liste.
*/

#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MALLOC_ERROR -1      // kod pogreske za neuspjelu alokaciju memorije
#define EXIT_SUCCESS 0       // kod uspjesnog izlaska iz programa
#define EMPTY_STACK 1        // kod pogreske za prazan stog
#define FILE_OPEN_ERROR -2   // kod pogreske pri otvaranju datoteke
#define MAX_LINE 1024        // max duljina retka 
#define INVALID_INPUT 2      // kod pogreske za neispravan unos
#define SCANF_ERROR -3       // kod pogreske pri citanju s sscanf-om

typedef struct stackItem {
	float value;			//vrijednost
	struct stackItem* next;	//pokazivac na sljed cvor

}StackItem;



typedef struct {	//tag nije potreban
	StackItem* top;	//pokazivac na vrh stoga (zadnje dodani elem)

}Stack;



int push(Stack* stack, float x);	//dodaje novi elem na vrh stoga
int pop(Stack* stack, float* x);		//uklanja elem s vrha stoga

int main() {

	int size = 0;                   // velicina trenutno procitanog elementa (za pomak pokazivaca)
	float firstOperand = 0, secondOperand = 0, result = 0; // operandi i konacan rezultat
	char currItem[10] = { 0 };      // buffer za trenutni token
	char postfix[MAX_LINE] = { 0 }; // polje za postfix izraz procitan iz datoteke
	char* postfixPointer = postfix; // pokazivac koji se pomice kroz izraz
	FILE* filePointer = NULL;       // pokazivac na datoteku
	Stack stack = { .top = NULL };  // inicijalizacija praznog stoga



	filePointer = fopen("postfix.txt", "r");	//otvaramo dat
	if (filePointer == NULL) {
		printf("\nfile couldnt open\n");
		return FILE_OPEN_ERROR;
	}


	fgets(postfix, MAX_LINE, filePointer);	//citanje cijelog retka iz dat
	printf("postfix: %s\n", postfix);		//ispis procitanog retka



	// petlja prolazi kroz cijeli izraz dokle god ima znakova
	while (strlen(postfixPointer) > 0)
	{
		// sscanf cita sljedeci token 
		// %n zapisuje koliko je znakova procitano u varijablu size
		if (sscanf(postfixPointer, " %s %n ", currItem, &size) != 1) {
			printf("sscanf error.\n");
			return SCANF_ERROR;
		}




		// ako je trenutni element broj (provjera prvog znaka)
		if (currItem[0] >= '0' && currItem[0] <= '9') {
			// pretvori string u float i dodaj na stog
			if (push(&stack, atof(currItem)))
				return MALLOC_ERROR;
		}




		else if (currItem[0] == '+' || currItem[0] == '-' || currItem[0] == '*' || currItem[0] == '/')
		{
			// skini dva broja sa stoga (prvo drugi operand, zatim prvi)
			if (pop(&stack, &secondOperand) || pop(&stack, &firstOperand)) {
				printf("cant read from empty stack.\n");
				return EMPTY_STACK;
			}

			// izvrsi odgovarajucu operaciju i vrati rezultat na stog
			if (currItem[0] == '+') {
				if (push(&stack, firstOperand + secondOperand)) return MALLOC_ERROR;
			}

			else if (currItem[0] == '-') {
				if (push(&stack, firstOperand - secondOperand)) return MALLOC_ERROR;
			}

			else if (currItem[0] == '*') {
				if (push(&stack, firstOperand * secondOperand)) return MALLOC_ERROR;
			}

			else {
				if (push(&stack, firstOperand / secondOperand)) return MALLOC_ERROR;
			}
		}




		// ako znak nije ni broj ni operator — pogresan unos
		else {
			printf("Invalid character in postfix.\n");
			return INVALID_INPUT;
		}

		// pomak pokazivaca unaprijed za vec procitani dio izraza
		postfixPointer += size;

	}

		// konacan rezultat
		if (pop(&stack, &result)) return EMPTY_STACK;
		printf("Result: %g\n", result);

		return 0;

}


int push(Stack* stack, float x) {
	StackItem* item = NULL;

	item = (StackItem*)malloc(sizeof(StackItem));
	if (item == NULL) {
		printf("\nmalloc error\n");
		return MALLOC_ERROR;
	}

	item->value = x;			//postavi vrijednost novog elem
	item->next = stack->top;	//novi elem pokazuje na trenutni vrh
	stack->top = item;			//novi elem postaje vrh stoga

	return EXIT_SUCCESS;

}



int pop(Stack* stack, float* x) {
	StackItem* top;
	top = stack->top;

	if (top == NULL) {
		printf("\ncant read from empty stack\n");
		return EMPTY_STACK;
	}

	*x = top->value;			//spremi vrijednost vrh elem
	stack->top = top->next;		//pomakni vrh stoga na sljed elem
	free(top);					//oslob memorije vrh elem

	return EXIT_SUCCESS;

}



