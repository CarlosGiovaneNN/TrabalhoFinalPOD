#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct huffman{
    unsigned data;
    struct huffman *zero;
    struct huffman *one;    
} Huffman;

typedef struct array{
    char c;
    int qtd;
    struct array *next;
} Array;

void excElement(Array *first, char c) {

    Array *aux;

    if(first->c == c) {
        aux = first;
        first = first->next;
        free(aux);
    } 
    else {
        Array *current = first;
    
        while (current != NULL) {
            
            if(current->next->c == c) {

                aux = current->next;
                current->next = current->next->next;
                free(aux);
                break; 
            }
        }
    }
}

Array findMin(Array *first) {

    Array aux;
    aux.c = first->c;
    aux.qtd = first->qtd;
    aux.next = NULL;

    for(Array *current = first; current != NULL; current = current->next) {
        
        if(aux.qtd > current->qtd) {
        
            aux.qtd = current->qtd;
            aux.c = current->c;
        }
    }

    excElement(first, aux.c);

    return aux;
}

Huffman* topHeap( Huffman *aux1, Huffman *aux2) {

    Huffman *root = (Huffman*)malloc(sizeof(Huffman));

    if(aux1->data >= aux2->data) {

        root->zero = aux1;
        root->one = aux2;
    }
    else {
        
        root->zero = aux2;
        root->one = aux1;
    }

    return root;
}

Huffman* midHeap(Huffman *side, Array *first) {

    Array min = findMin(first);
    Huffman *root = (Huffman*)malloc(sizeof(Huffman));
    Huffman *otherSide = (Huffman*)malloc(sizeof(Huffman));

    otherSide->data = min.c;
    otherSide->one = NULL;
    otherSide->zero = NULL;

    if(otherSide->data >= side->data) {

        root->zero = otherSide;
        root->one = side;
    } 
    else {
        root->zero = side;
        root->one = otherSide;
    }

    root->data = min.qtd + side->data;

    return root;
}

Huffman* minHeap(Array *first) {

    Huffman *root = (Huffman*)malloc(sizeof(Huffman));
    Huffman *aux1 = (Huffman*)malloc(sizeof(Huffman));
    Huffman *aux2 = (Huffman*)malloc(sizeof(Huffman));
    Array min1 = findMin(first);
    Array min2 = findMin(first);
    
    aux1->data = min1.c;
    aux1->one = NULL;
    aux1->zero = NULL;    
    aux2->data = min2.c;
    aux2->one = NULL;
    aux2->zero = NULL;

    
    if(min2.qtd >= min2.qtd) {

        root->zero = aux1;
        root->one = aux2;
    } 
    else {
        root->zero = aux2;
        root->one = aux2;
    }

    root->data = min1.qtd + min2.qtd;

    return root;
}

void makeHuffman(Huffman *root, Array *first) {

    int count = 0;
    for(Array *current = first; current != NULL; current = current->next) count++;
    
    //n sei o que fazer
}

void add(Array *first, char c) {

    Array *aux = (Array*)malloc(sizeof(Array));

    Array *current = first;

    int flag = 0;

    if(current == NULL) {
        aux->c = c;
        aux->qtd = 1;
        aux->next = NULL;
        current = aux;
        flag = 1;
    }
    else { 
        while(current != NULL) {

            if(current->c == c) {
                current->qtd++;
                flag = 1;
            }
            current = current->next;
        }
    }

    if(!flag) {
        aux->c = c;
        aux->qtd++;
        aux->next = NULL;
        current = aux;
    }
}

int main() {

    FILE *txt = fopen("amostra.txt", "r"); 
    if (txt == NULL) {
        printf("Erro ao abrir o arquivo");
        exit(1);
    }
    
    Array *first = NULL;
    Huffman *root = NULL;
    char c;

    while (!feof(txt)) {
        
        c = fgetc(txt);

        add(first, c);
    }

    makeHuffman(root, first);

    return 1;
}
