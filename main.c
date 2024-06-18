#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASCII 256

typedef struct huffman{
    char data;
    int oc;
    struct huffman *zero;
    struct huffman *one;    
} Huffman;

//ADICIONANDO NO VETOR
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

//ORDENANDO VETORES
void bubbleSort(char vector[], int qtd[], int size) {

    int flag;

    for (int i = 0; i < size - 1; i++) {

        flag = 0;

        for (int j = 0; j < size - 1 - i; j++) {

            if (qtd[j] > qtd[j + 1] || (qtd[j] == qtd[j + 1] && vector[j] > vector[j + 1])) {

                int aux = qtd[j];
                qtd[j] = qtd[j + 1];
                qtd[j + 1] = aux;

                char c = vector[j];
                vector[j] = vector[j + 1];
                vector[j + 1] = c;

                flag = 1;
            }
        }

        if (!flag) break;
    }
}

//CRIANDO FOLHAS
void createLeafs(char vector[], int size, Huffman *array[], int qtd[]) {

    for(int i = 0; i < size; i++) {

        Huffman *aux = (Huffman *)malloc(sizeof(Huffman));
        aux->one = NULL;
        aux->zero = NULL;

        aux->data = vector[i];
        aux->oc = qtd[i];

        array[i] = aux;
    }
}

void makeHuffman(Huffman *array[], int size) {

    while (size > 1) {

        //ATRIBUINDO OS VALORES DO NOVO NÓ, ATRAVÉS DE UM AUXILIAR
        Huffman *aux = (Huffman *)malloc(sizeof(Huffman));

        //COLOCANDO O CARACTER DENTRO DO NÓ COMO VAZIO
        aux->data = '\0';
        int number = array[0]->oc + array[1]->oc;
        aux->oc = number;
        aux->one = array[1];
        aux->zero = array[0];

        //PUXANDO OS ELEMENTOS
        for (int i = 0; i < size - 2; i++) array[i] = array[i + 2];

        array[size - 2] = aux;

        //ORDENANDO
        for (int i = size - 2; i > 0 && array[i - 1]->oc >= array[i]->oc; i--) {

            Huffman *auxData = array[i];
            array[i] = array[i - 1];
            array[i - 1] = auxData;
        }
        size--;
    }
}

//RECURCIVIDADE PARA RETORNAR O CODIGO DA LETRA 
void findCodeLetter(Huffman *root, char *cod, char c, int depth, char *path) {
    
    if(root == NULL) return;

    if (root->data == c) {
        cod[depth] = '\0';
        strcpy(path, cod);
        return;
    }

    if (root->zero != NULL) {
        cod[depth] = '0';
        findCodeLetter(root->zero, cod, c, depth + 1, path);
    }

    if (root->one != NULL) {
        cod[depth] = '1';
        findCodeLetter(root->one, cod, c, depth + 1, path);
    }

    return;
}

//CODIFICAR O ARQUIVO
void codeFile(FILE *txt, Huffman *root) {

    FILE *code = fopen("codificado.txt", "w");

    if(code == NULL) exit(1);

    char c = fgetc(txt);

    while (!feof(txt)) {
        if (c != '\n') {
            char aux[20];
            char path[20];
            findCodeLetter(root, aux, c, 0, path);
            fputs(path, code);
        }
        c = fgetc(txt);
    }

    fclose(code);
}

//RECURSIVIDADE PARA RETORNAR A LETRA DO CODIGO
void findCode(FILE *txt, Huffman *root, char *character, int *flag) {

    if(root == NULL || *flag == 1) return;
    
    if(root->data != '\0') {
        *character = root->data;
        *flag = 1;
        return;
    }

    char c = fgetc(txt);

    if(c == '0') findCode(txt, root->zero, character, flag);

    if (c == '1') findCode(txt, root->one, character, flag);

}

//DECODIFICAR ARQUIVO
void decodeFile(FILE *txt, Huffman *root) {

    FILE *decode = fopen("decodificado.txt", "w");

    if(decode == NULL) exit(1);

    char c = fgetc(txt);
    char character = '\0';

    while (!feof(txt)) {

        int flag = 0;
        
        if(c == '0') findCode(txt, root->zero, &character, &flag);

        else if (c == '1') findCode(txt, root->one, &character, &flag);

        c = fgetc(txt);
        fputc(character, decode);
    }

    fclose(decode);
}

//PRINT
void print(Huffman *root, char *cod, int depth) {
    
    if(root == NULL) return;

    if (root->data != '\0') {
        cod[depth] = '\0';
        printf("%c: %s\n", root->data, cod);
        return;
    }

    if (root->zero != NULL) {
        cod[depth] = '0';
        print(root->zero, cod, depth + 1);
    }

    if (root->one != NULL) {
        cod[depth] = '1';
        print(root->one, cod, depth + 1);
    }

    return;
}

int main() {

    FILE *txt = fopen("amostra.txt", "r"); 
    
    if (txt == NULL) exit(1);

    char c= fgetc(txt);
    char vector[ASCII] = {0};
    int qtd[ASCII] = {0};
    int size = 0;

    //LER O ARQUIVO
    while (!feof(txt)) {
        if(c != '\n') add(vector, qtd, c, &size);
        c = fgetc(txt);
    }

    //SE O ARQUIVO ESTIVER EM BRANCO ELE DEIXAO EM BRANCO, MESMO QUE NOS DOIS TXT(codificado.txt E O decodificado.txt) ESTIVESSEM ALGO DENTRO ANTES
    if(size == 0) {
        FILE *code = fopen("codificado.txt", "w");
        FILE *decode = fopen("decodificado.txt", "w");
        fclose(code);
        fclose(decode);
        fclose(txt);
        exit(1);
    }

    bubbleSort(vector, qtd, size);

    Huffman *root = (Huffman *)malloc(sizeof(Huffman));

    //CRIARÁ UMA HUFFMAN DE SOMENTE UMA LETRA
    if(size > 1) {

        Huffman *array[size];

        createLeafs(vector, size, array, qtd);
        makeHuffman(array, size);

        root = array[0];

    } else {

        //CRIARÁ UMA HUFFMAN DE MAIS LETRAS
        root->one = NULL;
        root->oc = qtd[0];
        root->data = '\0';

        Huffman *aux = (Huffman *)malloc(sizeof(Huffman));

        aux->data = vector[0];
        aux->oc = qtd[0];
        aux->one = NULL;
        aux->zero = NULL;

        root->zero = aux;
    }

    //PRINT DOS CODIGO
    char empty[20] = {0};
    print(root, empty, 0);

    //RESETA O PONTEIRO DO ARQUIVO
    rewind(txt);
    codeFile(txt, root);

    FILE *code = fopen("codificado.txt", "r");
    
    //RESETA O PONTEIRO DO ARQUIVO
    rewind(code);
    decodeFile(code, root);

    //LIMPA E FECHA TUDO QUE PRECISA
    free(root);
    fclose(code);
    fclose(txt);
    
    return 0;
}