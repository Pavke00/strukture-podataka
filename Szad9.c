#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// struktura cvora binarnog stabla
typedef struct node* NodePtr;
typedef struct node {
    int data;
    NodePtr left;
    NodePtr right;
} Node;

// deklaracije funkcija
NodePtr insert(NodePtr root, int data);
int replace(NodePtr root);
void printInorder(NodePtr root, FILE* f);
void printInorderConsole(NodePtr root);
void deleteTree(NodePtr root);

int main() {
    NodePtr root = NULL;
    int n = 10;
    int niz[10] = { 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 }; // fiksni niz iz zadatka

    srand(time(NULL));

    // dio a) – dodavanje elemenata iz niza
    for (int i = 0; i < n; i++) {
        root = insert(root, niz[i]);
    }

    FILE* f = fopen("stablo.txt", "w");
    if (f == NULL) {
        printf("Greska pri otvaranju datoteke\n");
        return 1;
    }

    // inorder prije replace
    fprintf(f, "Inorder prije replace:\n");
    printInorder(root, f);
    printf("Inorder prije replace:\n");
    printInorderConsole(root);
    printf("\n");

    // dio b) – zamjena vrijednosti cvora
    replace(root);

    // inorder nakon replace
    fprintf(f, "\nInorder nakon replace:\n");
    printInorder(root, f);
    printf("Inorder nakon replace:\n");
    printInorderConsole(root);
    printf("\n");

    fclose(f);
    deleteTree(root);

    // dio c) – generiranje slučajnih brojeva u novom stablu
    NodePtr rootRandom = NULL;
    for (int i = 0; i < n; i++) {
        int value = rand() % 81 + 10; // 10–90
        rootRandom = insert(rootRandom, value);
    }

    f = fopen("stablo.txt", "a"); // dodaj na kraj datoteke
    if (f == NULL) {
        printf("Greska pri otvaranju datoteke\n");
        return 1;
    }

    fprintf(f, "\n\nInorder nasumicno generiranog stabla prije replace:\n");
    printInorder(rootRandom, f);
    printf("Inorder nasumicno generiranog stabla prije replace:\n");
    printInorderConsole(rootRandom);
    printf("\n");

    replace(rootRandom);

    fprintf(f, "\nInorder nasumicno generiranog stabla nakon replace:\n");
    printInorder(rootRandom, f);
    printf("Inorder nasumicno generiranog stabla nakon replace:\n");
    printInorderConsole(rootRandom);
    printf("\n");

    fclose(f);
    deleteTree(rootRandom);

    printf("Program je zavrsio. Provjerite datoteku stablo.txt\n");
    return 0;
}

// funkcija za dodavanje elementa u stablo
NodePtr insert(NodePtr root, int data) {
    if (root == NULL) {
        root = (NodePtr)malloc(sizeof(Node));
        root->data = data;
        root->left = root->right = NULL;
        return root;
    }

    if (data < root->data)
        root->left = insert(root->left, data);
    else
        root->right = insert(root->right, data);

    return root;
}

// funkcija replace – svaki cvor postaje suma svih potomaka
int replace(NodePtr root) {
    if (root == NULL)
        return 0;

    int oldData = root->data;
    root->data = replace(root->left) + replace(root->right);

    return root->data + oldData;
}

// inorder prolazak i upis u datoteku
void printInorder(NodePtr root, FILE* f) {
    if (root != NULL) {
        printInorder(root->left, f);
        fprintf(f, "%d ", root->data);
        printInorder(root->right, f);
    }
}

// inorder prolazak i ispis u konzolu
void printInorderConsole(NodePtr root) {
    if (root != NULL) {
        printInorderConsole(root->left);
        printf("%d ", root->data);
        printInorderConsole(root->right);
    }
}

// rekurzivno brisanje stabla
void deleteTree(NodePtr root) {
    if (root != NULL) {
        deleteTree(root->left);
        deleteTree(root->right);
        free(root);
    }
}



