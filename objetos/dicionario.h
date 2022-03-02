#ifndef DICIONARIO_H_INCLUDED
#define DICIONARIO_H_INCLUDED

typedef struct noArvore{
    char cod;
    struct noArvore *dir;
    struct noArvore *prox;
} no;


void initArvore(no *pNovo){
	pNovo->cod = '#';
	pNovo->prox  = NULL;
	pNovo->dir  = NULL;
}

// --- ORDENAR PALAVRAS NA ÁRVORE N-ÁRIA ---
no *insertLetter(no *pNovo, char palavra[], int tamanho, int *indice){
    if(*indice < tamanho){
        if(pNovo->cod == '#'){
            pNovo->cod = palavra[*indice];
            // --- INICIALIZAR A DIREITA ---
            no *direita = (no*)malloc(sizeof(no));
            direita->cod = '#';
            direita->dir = NULL;
            direita->prox = NULL;

            pNovo->dir = direita;

            // --- INICIALIZAR O PRÓXIMO ---
            no *proximo = (no*)malloc(sizeof(no));
            proximo->cod = '#';
            proximo->dir = NULL;
            proximo->prox = NULL;

            pNovo->prox = proximo;

        }

        if(pNovo->cod != palavra[*indice]){
            pNovo->dir = insertLetter(pNovo->dir, palavra, tamanho, indice);
        }
        else if(pNovo->cod == palavra[*indice]){

            *indice = *indice + 1;

            pNovo->prox = insertLetter(pNovo->prox, palavra, tamanho, indice);
        }


    }
    if(pNovo->cod == '#'){
        pNovo->cod = '!';
        no *direita = (no*)malloc(sizeof(no));
        direita->cod = '#';
        direita->dir = NULL;
        direita->prox = NULL;
        pNovo->dir = direita;
    }
    return pNovo;
}


void getLetter(no *pNovo){
    FILE *dicionario;
    dicionario  = fopen("res/palavras.txt", "r");
    while(!feof(dicionario)){
        char palavra[15];
        int tamanho, i;
        int *indice = (int *)malloc(sizeof(int));
        *indice = 0;
        fscanf(dicionario, "%s", palavra);

        tamanho = strlen(palavra);

        if(tamanho > 3){
            pNovo = insertLetter(pNovo, palavra, tamanho, indice);
        }



    }
    fclose(dicionario);
}


#endif // DICIONARIO_H_INCLUDED
