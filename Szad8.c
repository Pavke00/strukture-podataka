#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define EXIT_SUCCESS 0
#define MALLOC_ERROR -1

// Struktura ?vora stabla
struct treeNode;
typedef struct treeNode* TreePosition;
typedef struct treeNode {
    int data;
    TreePosition left;
    TreePosition right;
} TreeNode;

// Struktura ?vora reda za level-order
struct queueNode;
typedef struct queueNode* QueuePosition;
typedef struct queueNode {
    TreePosition treeNode;
    QueuePosition next;
} QueueNode;

// Globalni pokaziva? na kraj reda
QueuePosition rear = NULL;

// Deklaracije funkcija
TreePosition insert(int x, TreePosition root);
TreePosition search(int x, TreePosition root);
int printPreorder(TreePosition root, int level);
int printInorder(TreePosition root, int level);
int printPostorder(TreePosition root, int level);
QueuePosition enqueue(TreePosition treeNode);
TreePosition dequeue();
int printLevelorder(TreePosition root);
TreePosition findMin(TreePosition root);
TreePosition deleteElement(TreePosition root, int x);
int deleteTree(TreePosition root);

int main() {
    TreePosition root = NULL;
    int choice, value;

    do {
        printf("\n=== Operacije nad BST ===\n");
        printf("1. Unos elementa\n");
        printf("2. Pronadi element\n");
        printf("3. Obrisi element\n");
        printf("4. Preorder ispis\n");
        printf("5. Inorder ispis\n");
        printf("6. Postorder ispis\n");
        printf("7. Level-order ispis\n");
        printf("0. Izlaz\n");
        printf("Unesi izbor: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Unesi vrijednost: ");
            scanf("%d", &value);
            root = insert(value, root);
            if (root == NULL) printf("Greska kod alociranja memorije!\n");
            else printf("Element dodan.\n");
            break;
        case 2:
            printf("Unesi vrijednost za pronalazak: ");
            scanf("%d", &value);
            if (search(value, root) != NULL)
                printf("Element %d je pronaden.\n", value);
            else
                printf("Element %d nije pronaden.\n", value);
            break;
        case 3:
            printf("Unesi vrijednost za brisanje: ");
            scanf("%d", &value);
            root = deleteElement(root, value);
            break;
        case 4:
            printf("Preorder ispis:\n");
            printPreorder(root, 0);
            break;
        case 5:
            printf("Inorder ispis:\n");
            printInorder(root, 0);
            break;
        case 6:
            printf("Postorder ispis:\n");
            printPostorder(root, 0);
            break;
        case 7:
            printf("Level-order ispis:\n");
            printLevelorder(root);
            break;
        case 0:
            printf("Izlaz iz programa.\n");
            break;
        default:
            printf("Neispravan izbor!\n");
        }

    } while (choice != 0);

    deleteTree(root);
    return EXIT_SUCCESS;
}

// Funkcija za unos elementa u stablo
TreePosition insert(int x, TreePosition root) {
    if (root == NULL) {
        root = (TreePosition)malloc(sizeof(TreeNode));
        if (root == NULL) {
            printf("Malloc error.\n");
            return NULL;
        }
        root->data = x;
        root->left = root->right = NULL;
    }
    else if (x < root->data) {
        root->left = insert(x, root->left);
    }
    else if (x > root->data) {
        root->right = insert(x, root->right);
    }
    return root;
}

// Funkcija za pretragu elementa
TreePosition search(int x, TreePosition root) {
    if (root == NULL) return NULL;
    if (x < root->data) return search(x, root->left);
    else if (x > root->data) return search(x, root->right);
    else return root;
}

// Preorder ispis
int printPreorder(TreePosition root, int level) {
    if (root == NULL) return 0;
    for (int i = 0; i < level; i++) printf("   ");
    printf("%d\n", root->data);
    printPreorder(root->left, level + 1);
    printPreorder(root->right, level + 1);
    return 0;
}

// Inorder ispis
int printInorder(TreePosition root, int level) {
    if (root == NULL) return 0;
    printInorder(root->left, level + 1);
    for (int i = 0; i < level; i++) printf("   ");
    printf("%d\n", root->data);
    printInorder(root->right, level + 1);
    return 0;
}

// Postorder ispis
int printPostorder(TreePosition root, int level) {
    if (root == NULL) return 0;
    printPostorder(root->left, level + 1);
    printPostorder(root->right, level + 1);
    for (int i = 0; i < level; i++) printf("   ");
    printf("%d\n", root->data);
    return 0;
}

// Red za level-order
QueuePosition enqueue(TreePosition treeNode) {
    QueuePosition queueNode = (QueuePosition)malloc(sizeof(QueueNode));
    if (!queueNode) {
        printf("Malloc error.\n");
        return NULL;
    }
    queueNode->treeNode = treeNode;
    queueNode->next = NULL;

    if (rear == NULL) {
        rear = queueNode;
        rear->next = rear; // kružni red
    }
    else {
        queueNode->next = rear->next;
        rear->next = queueNode;
        rear = queueNode;
    }
    return queueNode;
}

TreePosition dequeue() {
    if (rear == NULL) return NULL;
    TreePosition item;
    if (rear == rear->next) {
        item = rear->treeNode;
        free(rear);
        rear = NULL;
    }
    else {
        QueuePosition front = rear->next;
        item = front->treeNode;
        rear->next = front->next;
        free(front);
    }
    return item;
}

// Level-order ispis
int printLevelorder(TreePosition root) {
    if (root == NULL) return 0;
    rear = enqueue(root);
    while (rear != NULL) {
        TreePosition node = dequeue();
        printf("%d ", node->data);
        if (node->left != NULL) rear = enqueue(node->left);
        if (node->right != NULL) rear = enqueue(node->right);
    }
    printf("\n");
    return 0;
}

// Prona?i najmanji element u stablu
TreePosition findMin(TreePosition root) {
    while (root->left != NULL) root = root->left;
    return root;
}

// Brisanje elementa iz stabla
TreePosition deleteElement(TreePosition root, int x) {
    if (root == NULL) {
        printf("Element %d nije pronaden.\n", x);
        return NULL;
    }
    if (x < root->data) root->left = deleteElement(root->left, x);
    else if (x > root->data) root->right = deleteElement(root->right, x);
    else {
        if (root->left != NULL && root->right != NULL) {
            TreePosition minRight = findMin(root->right);
            root->data = minRight->data;
            root->right = deleteElement(root->right, minRight->data);
        }
        else {
            TreePosition temp = root;
            if (root->left == NULL) root = root->right;
            else root = root->left;
            free(temp);
        }
    }
    return root;
}

// Osloba?anje cijelog stabla
int deleteTree(TreePosition root) {
    if (root == NULL) return 0;
    deleteTree(root->left);
    deleteTree(root->right);
    free(root);
    return 0;
}
