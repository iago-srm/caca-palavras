
#include <stdlib.h>
#include <stdio.h>

#define MAX 50
#define MAX_PALAVRA 20

#define tipoDaPilha palavra*



typedef struct{
    int linha;
    int coluna;
    int size;
    int vertical;
} espaco;

typedef struct {
    espaco* espacos;
    int num_esp;
    char texto [MAX_PALAVRA];
    int tamanho;
    int index;

} palavra;

typedef struct{
	tipoDaPilha *v;
	int topo;
	int size;
}
pilha;

typedef pilha *Pilha;

//TOPO INDICA O PRIMEIRO ESPA�O VAZIO
Pilha criaPilha() {

	Pilha p = (Pilha) malloc(sizeof(pilha));
	p->v = (tipoDaPilha*) malloc(sizeof(tipoDaPilha) * MAX);
	p->topo = 0;
	p->size=MAX;
	return p;
}

void redimensionar(Pilha p){
	tipoDaPilha* u = (tipoDaPilha*) malloc(sizeof(tipoDaPilha)*p->size*2);
	int i;
	for (i=0; i<p ->size; i++)
		u[i] = p->v[i];
	free(p->v);
	p->v = u;
	p->size*=2;
}

void push(Pilha p, tipoDaPilha x) {
	if (p->topo == p->size)
		redimensionar(p);
	p->v[p->topo++] = x;
	
}

void pop(Pilha p){

    
    p->topo--;

}

tipoDaPilha topoDaPilha(Pilha p){

        return p->v[p->topo-1];

}

int pilhaVazia(Pilha p){
    if(p->topo == 0){
        return 1;
    } return 0;
}




//PRODUZ O VETOR DOS ESPACOS DISPONIVEIS
espaco* produz_espacos(int** tab_num, int m, int n,int* num_espacos){

    espaco* e = (espaco*) malloc(MAX * sizeof(espaco));
    int primeiro_linha=0, primeiro_coluna=0, definido_primeiro=0,l=0,linha=0,k = 0,vertical=1, count=0;

        for(k=0;k<n;k++){
            for(linha=0;linha<=m;linha++){
                //PARA CADA CELULA DE UMA COLUNA
                if(tab_num[linha][k] == 0 && definido_primeiro != 1){
                    primeiro_linha = linha;
                    primeiro_coluna = k;
                    count++;
                    definido_primeiro = 1;
                }
                else if (tab_num[linha][k] == 0){
                    count++;
                }
                else if(tab_num[linha][k] == -1 || linha == m-1){
                    if(count >= 2){
                        espaco esp;
                        esp.linha = primeiro_linha;
                        esp.coluna = primeiro_coluna;
                        esp.size = count;
                        esp.vertical = vertical;
                        e[l] = esp;
                        l++;
                    }
                    count = 0;
                    definido_primeiro = 0;
                }
            }
        }

        vertical = 0;
         for(linha=0;linha<m;linha++){
            for(k=0;k<=n;k++){
                //PARA CADA CELULA DE UMA LINHA
                if(tab_num[linha][k] == 0 && definido_primeiro!= 1){
                    primeiro_linha = linha;
                    primeiro_coluna = k;
                    count++;
                    definido_primeiro = 1;
                }
                else if (tab_num[linha][k] == 0){
                    count++;
                }
                else if(tab_num[linha][k] == -1 || linha == m-1){
                    if(count >= 2){
                        espaco esp;
                        esp.linha = primeiro_linha;
                        esp.coluna = primeiro_coluna;
                        esp.size = count;
                        esp.vertical = vertical;
                        e[l] = esp;
                        l++;
                    }
                    count = 0;
                    definido_primeiro = 0;
                }
            }
        }

    *num_espacos = l;
    return e;
}
//TRANSFORMA OS -1 EM * DO TABULEIRO
char** converte_num (int** tab_num, int m, int n){

    char** tab_char = (char**) malloc(m * sizeof(char*));
    int i,j;
    for(i=0;i<m;i++){
        tab_char[i] = (char*) malloc(n * sizeof(char));
        for(j=0;j<n;j++){
            if(tab_num[i][j] == -1){
                tab_char[i][j] = '*';
            } else{
                tab_char[i][j] = 48;
            }
        }
    }
    return tab_char;
}

//CONFERE SE O TEXTO JA EXISTENTE NO TABULEIRO PERMITE QUE A PALAVRA SEJA COLOCADA NO ESPACO INDEX
int confere(char** tab_char, palavra pal,int index){
    espaco esp = pal.espacos[index];
    int i;
    for(i=0;i<pal.tamanho;i++){
        if(esp.vertical){
            if(tab_char[esp.linha+i][esp.coluna] != pal.texto[i] && tab_char[esp.linha+i][esp.coluna] != 48){
                return 0;
            }
        }else{
            if(tab_char[esp.linha][esp.coluna+i] != pal.texto[i] && tab_char[esp.linha][esp.coluna+i] != 48){
                return 0;
            }
        }
    }
    return 1;
}

//ESCREVE NO TABULEIRO TODAS AS PALAVRAS EMPILHADAS
void insere(char** tab_char, Pilha p,int m, int n, espaco* espacos,int num_espacos){
    int i,j;
    palavra* pal;
    espaco esp;

    //ZERA TUDO
    for(i=0;i<num_espacos;i++){
        int linha, coluna;
        linha = espacos[i].linha;
        coluna = espacos[i].coluna;
        if(espacos[i].vertical){
            for(j=0;j<espacos[i].size;j++){
                tab_char[linha+j][coluna] = 48;
            }
        } else{
            for(j=0;j<espacos[i].size;j++){
                tab_char[linha][coluna+j] = 48;
            }
        }

    }
    //INSERE O QUE ESTA NA PILHA
    if(pilhaVazia(p))
        return;

    for(i=0;i<p->topo;i++){
        pal = p->v[i];
        esp = pal->espacos[pal->index];

        for(j=0;j<pal->tamanho;j++){
            if(esp.vertical){
                tab_char[esp.linha+j][esp.coluna] = pal->texto[j];
            }
            else{
                tab_char[esp.linha][esp.coluna+j] = pal->texto[j];
            }
        }

    }
  
}
//IMPRIME O TABULEIRO
void printtable(char** tab_char, int m, int n){
    int i,j;
    //printf("\n");
    for(i=0;i<n;i++){
        printf("-");
    }

    for (i=0;i<m;i++){
        printf("\n");
        for(j=0;j<n;j++){
            printf("%c ",tab_char[i][j]);
        }
    }
    printf("\n");
    for(i=0;i<n;i++){
        printf("-");
    }
    printf("\n\n");
}

int palavraRepetida (palavra** palavras, char s [],int k){

  
    int i,l,Eq=1;
    //P CADA PALAVRA JA INSERIDA
    for(i=0;i<k;i++){
        //P CADA LETRA DA PALAVRA REQUISITADA
        for(l=0;palavras[i]->texto[l] != '\0' && s[l] != '\0';l++){
            if(palavras[i]->texto[l] != s[l]){
                Eq = 0;
                break;
            }
        }
        if (Eq){
            return 1;
        }

    }
    return 0;

}

int main (){


    int m,n,p,i,j,instancia=0;

    while(1){
        scanf("%d%d", &m, &n);

        if(m==0 && n==0){
            return 0;
        }

        //COLOCA NUMEROS NA MATRIZ E PRODUZ UMA MARGEM
        int** tab_num = malloc((m+1) * sizeof(int*));
        for(i=0;i<m;i++){
            tab_num[i] = malloc((n+1) * sizeof(int));
            for(j=0;j<n;j++){
                scanf("%d",&tab_num[i][j]);
            }
            tab_num[i][j] = -1;
        }
        tab_num[i] = malloc((n+1) * sizeof(int));
        for(j=0;j<=n;j++){
            tab_num[i][j] = -1;
        }


        scanf("%d",&p);



       // IMPRIME MATRIZ NUMERICA PARA VERIFICACAO
      /* printf("p = %d, m = %d, n = %d\n",p,m,n);
        for (i=0;i<=m;i++){
            printf("\n");
            for(j=0;j<=n;j++){
                printf("%d ",tab_num[i][j]);
            }
        }
*/
        //REGISTRA AS PALAVRAS EM UM VETOR
        palavra** palavras = (palavra**)malloc(p*sizeof(palavra*));
        for(i=0;i<p;i++){
            palavras[i] = (palavra*) malloc(sizeof(palavra));
            palavras[i]->espacos = (espaco*)malloc(MAX * sizeof(espaco));
            palavras[i]->index=0;
        }

        //CONTA O TAMANHO DE CADA PALAVRA
        for(i=0;i<p;i++){
           // int j=0;
            //char s [MAX_PALAVRA];
            scanf("%s",palavras[i]->texto);
            /*if(palavraRepetida(palavras,s,i)){
               p--;
               continue;
            }

            for(j=0;s[j]!='\0';j++){
                palavras[i]->texto[j] = s[j];
            }
*/
            int count=0;
            for(j=0;palavras[i]->texto[j]!='\0';j++){
                count++;
            }
            palavras[i]->tamanho = count;


        }
/*
        for(i=0;i<p;i++){
            printf("palavra %d = %.*s\n",i,palavras[i]->tamanho,palavras[i]->texto);
        }
*/
        //REGISTRA OS ESPACOS EM UM VETOR
        int num_espacos=0;
        espaco* espacos  = produz_espacos(tab_num,m,n,&num_espacos);

        //VERIFICA SE HA ESPACO DISPONIVEL PARA AS PALAVRAS
       /* if(num_espacos != p){
            instancia++;
            printf("Instancia %d \n nao ha solucao, %d espacos != %d palavras\n",instancia,num_espacos,p);
           // int i;
            //scanf("%d",&i);
            //return 0;
            //continue;
           continue;
        }
*/

        //PREENCHE O VETOR ESPACOS DE CADA PALAVRA COM OS ESPACOS DO MESMO TAMANHO
        for(i=0;i<p;i++){
            int k=0;

            for(j=0;j<num_espacos;j++){
                if(palavras[i]->tamanho == espacos[j].size){
                    palavras[i]->espacos[k] = (espaco)espacos[j];

                    k++;
                }
            }
            palavras[i]->num_esp=k;

        }

       char** tab_char = converte_num(tab_num,m,n);

        //IMPRIME MATRIZ CHAR PARA VERIFICACAO
       // printtable(tab_char,m,n);


        Pilha pi = criaPilha();
        int temsol=1,ok=0;
        i=0;
        while(temsol && i<p){
            //ENQUANTO NAO HOUVER UM FIT E HOUVER ESPACO P PROCURAR
            while(!ok && palavras[i]->index<palavras[i]->num_esp){
                ok = 0;
                //printf("%.*s\n",palavras[i]->tamanho,palavras[i]->texto);

                if(confere(tab_char,*palavras[i],palavras[i]->index)){
                //HA FIT
                    push(pi,palavras[i]);
                    insere(tab_char,pi,m,n,espacos,num_espacos);
                    ok=1;
                    i++;
                    //printtable(tab_char,m,n);
                    continue;
                }

                palavras[i]->index++;


            }

            if(!ok){ //BACKTRACK

                palavras[i]->index=0;
                if(!pilhaVazia(pi)){
                    palavra* pal = topoDaPilha(pi);
                    pal->index++;
                    pop(pi);
                    i--;
                    insere(tab_char,pi,m,n,espacos,num_espacos);
                    //printtable(tab_char,m,n);
                }
                else
                    temsol=0;

            }else{//PROXIMA PALAVRA PARA INSERIR
                ok=0;
                continue;
            }
        }
        instancia++;
        if(!temsol){

            printf("Instancia %d \n nao ha solucao",instancia);
        }else{

            printf("Instancia %d \n",instancia);
            printtable(tab_char,m,n);
        }



    }

}
