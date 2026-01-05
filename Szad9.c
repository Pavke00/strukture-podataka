#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// struktura cvora binarnog stabla
typedef struct node* NodePtr;
typedef struct node {
    int data;            // vrijednost u cvoru
    NodePtr left;        // lijevo dijete
    NodePtr right;       // desno dijete
} Node;

// deklaracije funkcija
NodePtr addNode(NodePtr root, int data);
NodePtr generateTree(NodePtr root, int n);
int replaceValues(NodePtr root);
void printInorder(NodePtr root, FILE* f);
void deleteTree(NodePtr root);

int main() {
    NodePtr root = NULL;     // korijen stabla
    int n = 10;              // broj elemenata

    srand(time(NULL));       // inicijalizacija rand funkcije

    // generiranje stabla pomocu slucajnih brojeva
    root = generateTree(root, n);

    FILE* f = fopen("stablo.txt", "w");
    if (f == NULL) {
        printf("greska pri otvaranju datoteke\n");
        return 1;
    }

    // inorder ispis prije replace funkcije
    fprintf(f, "inorder prije replace\n");
    printInorder(root, f);

    // zamjena vrijednosti cvorova
    replaceValues(root);

    // inorder ispis nakon replace funkcije
    fprintf(f, "\ninorder nakon replace\n");
    printInorder(root, f);

    fclose(f);

    // oslobadanje memorije
    deleteTree(root);

    return 0;
}

// dodavanje novog elementa u binarno stablo pretrazivanja
NodePtr addNode(NodePtr root, int data) {

    // ako je stablo prazno stvaram novi cvor
    if (root == NULL) {
        root = (NodePtr)malloc(sizeof(Node));
        root->data = data;
        root->left = NULL;
        root->right = NULL;
        return root;
    }

    // manja vrijednost ide u lijevo podstablo
    if (data < root->data)
        root->left = addNode(root->left, data);

    // veca vrijednost ide u desno podstablo
    else if (data > root->data)
        root->right = addNode(root->right, data);

    return root;
}

// funkcija za stvaranje stabla sa slucajnim brojevima
NodePtr generateTree(NodePtr root, int n) {
    int i;
    int value;

    for (i = 0; i < n; i++) {
        value = rand() % 81 + 10;   // raspon od 10 do 90
        root = addNode(root, value);
    }

    return root;
}

// replace funkcija koja mijenja vrijednost cvora
// nova vrijednost je suma svih potomaka
int replaceValues(NodePtr root) {

    // ako nema cvora vracam 0
    if (root == NULL)
        return 0;

    // spremam staru vrijednost cvora
    int oldData = root->data;

    // rekurzivno racunam sumu lijevog i desnog podstabla
    root->data = replaceValues(root->left) + replaceValues(root->right);

    // vracam ukupnu sumu podstabla
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

// rekurzivno brisanje stabla
void deleteTree(NodePtr root) {
    if (root != NULL) {
        deleteTree(root->left);
        deleteTree(root->right);
        free(root);
    }
}
