#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ASCII 128
#define COUNT 10

typedef struct huffman{
    char data;
    int oc;
    struct huffman *zero;
    struct huffman *one;    
} Huffman;

void add(char vector[], int qtd[], char c, int *size) {
    
    int flag = 0;

    for (int i = 0; i < *size; i++) {
        if (vector[i] == c) {
            qtd[i]++;
            flag = 1;
            break;
        }
    }

    if (!flag) {
        vector[*size] = c;
        qtd[*size] = 1;
        (*size)++;
    }
}

void boobleSort(char vector[], int qtd[], int size) {

    int flag = 0;
    int i = size - 1;

    while(i >= 0) {

        for(int j = 0; j < size; j++) {

            if(qtd[j] > qtd[j + 1]) {
               
                int aux = qtd[j]; 
                char c = vector[j];

                vector[j] = vector[j + 1];
                qtd[j] = qtd[j + 1];

                vector[j + 1] = c;
                qtd[j + 1] = aux;

                flag = 1;
            }

            else if(qtd[j] == qtd[j + 1] && vector[j] > vector[j + 1]) {
               
                int aux = qtd[j]; 
                char c = vector[j];

                vector[j] = vector[j + 1];
                qtd[j] = qtd[j + 1];

                vector[j + 1] = c;
                qtd[j + 1] = aux;

                flag = 1;
            }
        }

        if(!flag) break;
        i--;
    }
}

void createLeafs(char vector[], int size, Huffman data[], int qtd[]) {

    for(int i = 0; i < size; i++) {

        data[i].data = vector[i];
        data[i].oc = qtd[i];
        data[i].one = NULL;
        data[i].zero = NULL;
    }
}

Huffman *makeHuffman(char vector[], int qtd[], int size) {

    Huffman *data = (Huffman*)malloc(size * sizeof(Huffman));

    createLeafs(vector, size, data, qtd);

    while (size != 1) {
        Huffman *aux = (Huffman *)malloc(sizeof(Huffman));
        aux->data = '\0';
        aux->oc = qtd[0] + qtd[1];
        aux->one = &data[1];
        aux->zero = &data[0];

        for (int i = 0; i < size - 2; i++) {
            data[i] = data[i + 2];
            qtd[i] = qtd[i + 2];
        }

        data[size - 2] = *aux;
        qtd[size - 2] = aux->oc;
        qtd[size - 1] = 0;

        int current = size - 3;
        
        while (current >= 0) {
            if (qtd[current] > aux->oc) {
                qtd[current + 1] = qtd[current];
                data[current + 1] = data[current];
                qtd[current] = aux->data;
                data[current] = *aux;
            }
            current--;
        }

        size--;
    }

    return &data[0];
}

void print2DUtil(Huffman* root) {
    
    if (root == NULL)
        return;
 
    print2DUtil(root->one);

    printf("\n %c", root->data);

    print2DUtil(root->zero);
}
 
void print2D(Huffman* root) {
    print2DUtil(root);
}


int main() {

    FILE *txt = fopen("amostra.txt", "r"); 
    if (txt == NULL) {
        printf("Erro ao abrir o arquivo");
        exit(1);
    }

    char c= fgetc(txt);
    char vector[ASCII];
    int qtd[ASCII];
    int size = 0;

    while (!feof(txt)) {
        
      

        add(vector, qtd, c, &size);

          c = fgetc(txt);
    }

    boobleSort(vector, qtd, size);
    
    Huffman *root = makeHuffman(vector, qtd, size);

    print2D(root);

    fclose(txt);

    return 0;
}