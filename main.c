/*
REQUISITOS
1- número de eleitores
2- número de chapas a serem cadastradas
    Nome do candidato (50)
    número do candidato (1-99)
    data de nascimento
    nome do vice (50)
3- Adicionar opção de parar de cadastrar chapas
4- Votação
    Votos não cadastrados como candidatos são nulos
    0 é voto em branco
    Exibir as opções de candidatos a votar
    Confirmaçao de voto é necessários
    Salvar o boletim de urna ao final
        número de votos de cada chapa
        votos em branco
        votos nulos
        votos válidos (soma das chapas)
        votos totais
        Incluir a porcentagem de votos válidos, totais e de cada chapa. Também dos votos nulos e brancos em relação ao total
5- 2° turno - aviso prévio de segundo turno
    Número mínimo de eleitores <= 10
    Apresentar informação dos candidatos
    Fim da votação opcional ou até o fim dos eleitores
    estatiticas do turno igual ao primeiro
    Se empate -> candidato mais velho
*/

#ifdef WIN32 
    #define limpa "cls"
#else 
    #define limpa "clear"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int numeroChapas = 0;
int nulos = 0;
int brancos = 0;
int total = 0;
int validos = 0;

typedef struct chapa Chapa;
struct chapa{
    char candidato[50];
    int numero;
    int data[3];
    char vice[50];
    int votos;
};

typedef struct lista Lista;
struct lista{
    Chapa *candidato;
    Lista *prox;
};

void limpaBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

Chapa *criarChapa(Lista *list) {
    Chapa *candidato = (Chapa*) malloc(sizeof(Chapa));
    if(candidato == NULL) {
        printf("\nErro ao alocar memoria para o candidato!");
        free(candidato);
        return NULL;
    }
    limpaBuffer();
    
    printf("\nNome do candidato: ");
    fgets(candidato->candidato, 50, stdin);
    candidato->candidato[strcspn(candidato->candidato, "\n")] = '\0'; // Remove o \n ao final
   
    printf("\nNumero da chapa: ");
    scanf("%i", &candidato->numero);
    for(Lista *aux = list; aux != NULL; aux = aux->prox){
        while(candidato->numero == aux->candidato->numero){
            printf("\nJa existem candidatos com esse numero de chapa!\nInsira outro numero de chapa: ");
            scanf("%i", &candidato->numero);
        }
    }
    limpaBuffer();

    // Ler a data de nascimento
    printf("\nData de nascimento");
    printf("\nDia: ");
    scanf("%i", &candidato->data[0]);
    while(candidato->data[0] < 1 || candidato->data[0] > 31){
        printf("\nInsira o dia corretamente (entre 1 e 31): ");
        scanf("%i", &candidato->data[0]);
    }
    printf("\nMes: ");
    scanf("%i", &candidato->data[1]);
    while(candidato->data[1] < 1 || candidato->data[1] > 12){
        printf("\nInsira o mes corretamente (entre 1 e 12): ");
        scanf("%i", &candidato->data[1]);
    }
    printf("\nAno: ");
    scanf("%i", &candidato->data[2]);
    while(candidato->data[2] < 1908 || candidato->data[0] > 2024){
        printf("\nInsira o ano corretamente (entre 1908 e 2024): ");
        scanf("%i", &candidato->data[2]);
    }
    limpaBuffer();  // Limpa o buffer após scanf

    // Ler o nome do vice
    printf("\nNome do vice: ");
    fgets(candidato->vice, 50, stdin);
    candidato->vice[strcspn(candidato->vice, "\n")] = '\0'; // Remove o \n ao final

    candidato->votos = 0;
    numeroChapas += 1;
    return candidato;
}

Lista *insereChapaLista(Lista *list, Chapa * chapa){
    Lista *new = (Lista*)malloc(sizeof(Lista));
    if(new == NULL){
        printf("\nErro ao alocar memoria na insercao do candidato na lista");
        return NULL;
    }
    new->candidato = chapa;
    new->prox = NULL;
    if(list == NULL){
        return new;
    }
    Lista *aux = list;
    while(aux->prox != NULL){
        aux = aux->prox;
    }
    aux->prox = new;
    return list;
}


void exibeCandidatos(Lista *list){
    if(list == NULL){
        printf("\nNenhuma chapa cadastrada!");
    }
    printf("\n[0] Voto em branco");
    Lista *aux = list;
    for(int i = 0; i < numeroChapas; i++, aux = aux->prox){
        printf("\n[%i] %s", aux->candidato->numero, aux->candidato->candidato);
    }
    printf("\n[demais numeros] nulo");
}

int atribuirVoto(Lista *list, int voto){
    Lista *aux = list;
    int confirmacao;
    if(voto == 0){
        printf("\nDeseja votar em branco?\n[0] nao\n[1] sim\nResposta: ");
        scanf("%i", &confirmacao);
        if(confirmacao == 1){
            brancos += 1;
            return 1;
        }
        else{
            return 0;
        }
    }
    if(voto == -1){
        printf("\nDeseja encerrar a votacao?\n[0] nao\n[1] sim\nResposta: ");
        scanf("%i", &confirmacao);
        if(confirmacao == 1){
            return -1;
        }
        else{
            return 0;
        }
    }
    for(int i = 0; i < numeroChapas; i++, aux = aux->prox){
        if(aux->candidato->numero == voto){
            printf("\n\nCONFIRMACAO DE VOTO");
            printf("\nDeseja votar em %s e %s como vice?\n[1] sim\n[0] nao\nResposta: ", aux->candidato->candidato, aux->candidato->vice);
            scanf("%i", &confirmacao);
            if(confirmacao == 1){
                aux->candidato->votos++;
                validos++;
                return 1;
            }
            else{
                return 0;
            } 
        }
    }
    printf("\nDeseja votar nulo?\n[0] nao\n[1] sim\n Resposta: ");
    scanf("%i", &confirmacao);
    if(confirmacao == 1){
        nulos += 1;
        return 1;
    }
    else{
        return 0;
    }
}

void relatorioTurno(Lista *list, int i, Chapa *campeao){
     char nomeArquivo[30];
    if (i == 1) {
        sprintf(nomeArquivo, "Relatorio_primeiro_turno.txt");
    } else {
        sprintf(nomeArquivo, "Relatorio_segundo_turno.txt");
    }

    FILE *arquivo = fopen(nomeArquivo, "w");
    if(arquivo == NULL){
        printf("\nErro ao adicionar relatorio!");
        return;
    }
    fprintf(arquivo, "Numero de votos totais: %i\n", total);
    fprintf(arquivo, "Votos em branco: %i (%.2f%%)\n", brancos, ((double) brancos/total) * 100);
    fprintf(arquivo, "Votos nulos: %i (%.2f%%)\n", nulos, ((double)nulos/total)*100);
    fprintf(arquivo, "Votos validos: %i (%.2f%%)\n", validos, ((double) validos/total) * 100);
    Lista *aux = list;
    for(int i = 0; i < numeroChapas; i++, aux = aux->prox){
        fprintf(arquivo, "Chapa %i - %s: %i votos (%.2f%% do total)\n", aux->candidato->numero, aux->candidato->candidato, aux->candidato->votos, ((double)aux->candidato->votos / total)*100);
    }
    fprintf(arquivo, "\n\nA CHAPA %i VENCEU AS ELEICOES\nPREFEITO: %s\nVICE-PREFEITO: %s", campeao->numero, campeao->candidato, campeao->vice);
    fclose(arquivo);
}

void liberaLista(Lista *list){
    Lista *aux = list;
    while (aux != NULL){
        aux = list;
        list = list->prox;
        free(aux);
    }
}

Lista *chapaFinal(Lista *list, Chapa *chapa1, Chapa *chapa2) {
    list = insereChapaLista(list, chapa1);
    list = insereChapaLista(list, chapa2);
    return list;
}

int comparaDatas(int data1[3], int data2[3]){
    if(data1[2] != data2[2])
        return data1[2] - data2[2];
    else if(data1[1] != data2[1])
        return data1[1] - data2[1];
    else{
        return data1[0] - data2[0];
    }
}


int main(){
    int numEleitores, chapasCadastrar, voto;
    Lista *chapas = NULL;
    Chapa *candidato;

    printf("\nNumero de eleitores: ");
    scanf("%i", &numEleitores);
    while(numEleitores <= 0){
        printf("\nInsira um numero de eleitores valido: ");
        scanf("%i", &numEleitores);
    }
    printf("\nNumero de chapas: ");
    scanf("%i", &chapasCadastrar);
    while(chapasCadastrar <= 1){
        printf("\nInsira duas ou mais chapas: ");
        scanf("%i", &chapasCadastrar);
    }
    system(limpa);
    printf("\n\nCADASTRAMENTO DE CHAPAS");
    for(int i = 0; i < chapasCadastrar; i++){
        printf("\n\nCHAPA %i", i+1);
        candidato = criarChapa(chapas);
        if(candidato == NULL){
            printf("\nNao foi possivel cadastrar a chapa!");
            return 0;
        }
        chapas = insereChapaLista(chapas, candidato);
    }

    printf("\n\nVOTACAO DE CANDIDATOS");
    for(int i = 0; i < numEleitores; i++){
        system(limpa);
        printf("\n\n%i- Selecione o numero do candidato a votar (-1 para finalizar votacao).\n", i+1);
        exibeCandidatos(chapas);
        printf("\n\nVoto: " );
        scanf("%i", &voto);
        voto = atribuirVoto(chapas, voto);
        if(voto == 1){
            total++;
            printf("\nVoto atribuido com sucesso!");
            sleep(1);
        }
        else if(voto == -1){
            system(limpa);
            printf("\n\nVOTACAO FINALIZADA!");
            printf("\nELABORANDO RELATORIO...");
            sleep(2);
            break;
        }
        else{
            i--;
        }
    }
    
    Lista  *aux = chapas;
    int maior1 = 0, maior2 = 0;
    Chapa *registro1, *registro2;
    for(int i = 0; i < numeroChapas; i++, aux = aux->prox){
        if(((double)aux->candidato->votos / (double)total) * 100 > 50){
            relatorioTurno(chapas, 1, aux->candidato);
            printf("\n\n A CHAPA %i VENCEU AS ELEICOES\nPREFEITO: %s\nVICE-PREFEITO: %s", aux->candidato->numero, aux->candidato->candidato, aux->candidato->vice);
            liberaLista(chapas);
            return 0;
        }
        else{
            if(aux->candidato->votos  > maior1){
                maior1 = aux->candidato->votos;
                registro1 = aux->candidato;
            }
            else if(aux->candidato->votos > maior2){
                maior2 = aux->candidato->votos;
                registro2 = aux->candidato;
            }
        }
    }

    if(numEleitores <= 10){
        if(registro1->votos == registro2->votos){
            if(comparaDatas(registro1->data, registro2->data) < 0){
                printf("\n\n A CHAPA %i VENCEU AS ELEICOES\nPREFEITO: %s\nVICE-PREFEITO: %s", registro1->numero, registro1->candidato, registro1->vice);
                relatorioTurno(chapas, 1, registro1);
            }
            else{
                printf("\n\n A CHAPA %i VENCEU AS ELEICOES\nPREFEITO: %s\nVICE-PREFEITO: %s", registro2->numero, registro2->candidato, registro2->vice);
                relatorioTurno(chapas, 1, registro2);
            }
            liberaLista(chapas);
            free(registro1);
            free(registro2);
            return 0;
        }
        for(Lista *aux = chapas; aux != NULL; aux = aux->prox){
            if(aux->candidato->numero == registro1->numero){
                printf("\n\nA CHAPA %i VENCEU AS ELEICOES\nPREFEITO: %s\nVICE-PREFEITO: %s", aux->candidato->numero, aux->candidato->candidato, aux->candidato->vice);
                relatorioTurno(chapas, 1, aux->candidato);
                free(registro1);
                free(registro2);
                liberaLista(chapas);
                return 0;
            }
        }
    }

    system(limpa);
    printf("\nNENHUM CANDIDATO ALCANCOU MAIS DE 50%% DOS VOTOS!\nCANDIDATOS DO SEGUNDO TURNO:");
    printf("\n%s\n%s", registro1->candidato, registro2->candidato);
    printf("\nDIGITE 1 PARA CONTINUAR O SEGUNDO TURNO: ");
    int confirmacao = 0;
    scanf("%i", &confirmacao);
    while(confirmacao != 1){
        printf("\nDIGITE 1 PARA CONTINUAR O SEGUNDO TURNO: ");
        scanf("%i", &confirmacao);
    }

    // SEGUNDO TURNO 
    system(limpa);
    for(int i = 0; i < 3; i++){
        printf("\n\nINICIO DO SEGUNDO TURNO");
        sleep(1);
        system(limpa);
        if(i == 2) break;
        sleep(1);
    }
    Lista *final = NULL;
    registro1->votos = 0;
    registro2->votos=0;
    final = chapaFinal(final, registro1, registro2);
    nulos = 0;
    brancos = 0;
    total = 0;
    validos = 0;
    numeroChapas = 2;
    
    printf("\n\nVOTACAO DE CANDIDATOS");
    for(int i = 0; i < numEleitores; i++){
        system(limpa);
        printf("\n\n%i- Selecione o numero do candidato a votar (-1 para finalizar votacao).\n", i+1);
        exibeCandidatos(final);
        printf("\n\nVoto: " );
        scanf("%i", &voto);
        voto = atribuirVoto(final, voto);
        if(voto == 1){
            total++;
            printf("\nVoto atribuido com sucesso!");
            sleep(1);
        }
        else if(voto == -1){
            system(limpa);
            printf("\n\nVOTACAO FINALIZADA!");
            printf("\nELABORANDO RELATORIO");
            sleep(2);
            break;
        }
        else{
            i--;
        }
    }

    if(final->candidato->votos > final->prox->candidato->votos){
        printf("\n\nA CHAPA %i VENCEU AS ELEICOES\nPREFEITO: %s\nVICE-PREFEITO: %s", final->candidato->numero, final->candidato->candidato, final->candidato->vice);
        relatorioTurno(final, 2, final->candidato);
    }
    else if(final->candidato->votos < final->prox->candidato->votos){
        printf("\n\nA CHAPA %i VENCEU AS ELEICOES\nPREFEITO: %s\nVICE-PREFEITO: %s", final->prox->candidato->numero, final->prox->candidato->candidato, final->prox->candidato->vice);
        relatorioTurno(final, 2, final->prox->candidato);
    }
    else{
        if(comparaDatas(final->candidato->data, final->prox->candidato->data) < 0){
            printf("\n\nA CHAPA %i VENCEU AS ELEICOES\nPREFEITO: %s\nVICE-PREFEITO: %s", final->candidato->numero, final->candidato->candidato, final->candidato->vice);
            relatorioTurno(final, 2, final->candidato);
        }
        else{
            printf("\n\nA CHAPA %i VENCEU AS ELEICOES\nPREFEITO: %s\nVICE-PREFEITO: %s", final->prox->candidato->numero, final->prox->candidato->candidato, final->prox->candidato->vice);
            relatorioTurno(final, 2, final->prox->candidato);
        }
    }

    liberaLista(chapas);
    free(registro1);
    free(registro2);
    liberaLista(final);
    return 0;
}