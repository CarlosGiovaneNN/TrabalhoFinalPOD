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

void bobbleSort(Array *first) {

    int flag = 0;

    for(Array *pointer = first; pointer != NULL; pointer = pointer->next) {

        for(Array *current = first; current->next != NULL; current = current->next) {

            if(current->qtd > current->next->qtd) {

                int aux;
                char auxc;

                aux = current->qtd;
                current->qtd = current->next->qtd;
                current->next->qtd = aux;

                auxc = current->c;
                current->c = current->next->c;
                current->next->c = auxc;

                flag = 1;                
            }
            else if(current->qtd == current->next->qtd & current->c > current->next->c ) {

                int aux;
                char auxc;

                aux = current->qtd;
                current->qtd = current->next->qtd;
                current->next->qtd = aux;

                auxc = current->c;
                current->c = current->next->c;
                current->next->c = auxc;

                flag = 1;                
            }
        }

        if(!flag) break;
    }
}

void bobbleSort(Huffman *vector[], int size) {

    int flag = 0;

    for(int i = size; i >= 0; i--) {

        for(int j = 0; vector[j+1] != NULL; j++) {

            if(vector[j]->data > vector[j+1]->data) {

                int aux;
                aux = vector[j]->data;
                vector[j]->data = vector[j+1]->data;
                vector[j+1]->data = aux;
                flag = 1;
            }
        }
        if(!flag) break;
    }
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

    Huffman *root = (Huffman*)malloc(sizeof(Huffman));
    Huffman *otherSide = (Huffman*)malloc(sizeof(Huffman));

    otherSide->data = first->c;
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

    root->data = first->qtd + side->data;

    Array *aux = first;
    first = first->next;
    free(aux);

    return root;
}

Huffman* minHeap(Array *first) {

    Huffman *root = (Huffman*)malloc(sizeof(Huffman));
    Huffman *aux1 = (Huffman*)malloc(sizeof(Huffman));
    Huffman *aux2 = (Huffman*)malloc(sizeof(Huffman));
    
    
    Array *min1 = first;
    Array *min2 = first;

    aux1->data = min1->c;
    aux1->one = NULL;
    aux1->zero = NULL;    
    aux2->data = min2->c;
    aux2->one = NULL;
    aux2->zero = NULL;

    
    if(min2->qtd >= min2->qtd) {

        root->zero = aux1;
        root->one = aux2;
    } 
    else {
        root->zero = aux2;
        root->one = aux2;
    }

    root->data = min1->qtd + min2->qtd;
    first = first->next->next;
    free(min1);
    free(min2);

    return root;
}

void makeHuffman(Huffman *root, Array *first) {

    int count = 0;
    for(Array *current = first; current != NULL; current = current->next) count++;
    
    Huffman *vector[count]; 
    int position = 0;

    while (first != NULL) {
        
        if(first->qtd == first->next->qtd) {

            vector[position] = minHeap(first);
            position++;

            if(first->qtd != first->next->qtd) {
                vector[position] = midHeap(vector[position-1], first);
                position++;
            }
            bobbleSort(vector);
        }
        else if(first->qtd != first->next->qtd) {
                vector[position] = midHeap(vector[position-1], first);
                position++;
        }

    }
    
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

    return 0;
}