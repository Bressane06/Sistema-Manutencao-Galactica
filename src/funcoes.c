/******************************************
*					   					  *
* Gabriel Fiorentino Bressane		      *
* Trabalho Prático 			              *
* Disciplina: Algoritmos e Programaçãoo II*
* Professores: Ivone e Ronaldo            *
* 								          *
******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"



/*********************LIBERAR MEMÓRIA*******************/


/* Free na lista de serviço */
void liberar_lista_servicos(reg_servicos **servicos){
    reg_servicos *aux;
    
    while(*servicos != NULL){
        aux = *servicos;
        *servicos = aux->prox;
        free(aux);
    }
}

/* Free na lista de naves */
void liberar_lista_naves(reg_naves **naves){
    reg_naves *aux;
    
    while(*naves != NULL){
        aux = *naves;
        *naves = aux->prox;
        free(aux);
    }
}

/* Free na lista de prioridades */
void liberar_lista_prioridade(reg_prioridade **prioridade) {
    reg_prioridade *aux;
    
    while(*prioridade != NULL){
        aux = *prioridade;
        *prioridade = aux->prox;
        free(aux);
    }
}

/* Free na lista de atendimento */
void liberar_servico_atendimento(reg_lst_atendimento **atendimento){

    reg_servicos *aux;
    
    if((*atendimento) -> servicos != NULL){
        aux = (*atendimento) -> servicos;
        (*atendimento) -> servicos = aux -> prox; 
        free(aux);
    }    
}

/* Função para liberar a memória alocada para a lista de naves no relatório */
void liberar_relatorio(reg_relatorio *relatorio){

    lst_R1 *atual = relatorio->lst_naves_feitas;
    lst_R1 *aux;

    /* Percorre cada elemento da lista, liberando um por um */
    while(atual != NULL){
        aux = atual->prox;
        free(atual);  
        atual = aux;
    }

    relatorio->lst_naves_feitas = NULL;
}



/********************INSERIR**********************/




/* Insere lista serviço */
void insere_servico(reg_servicos **servicos, reg_servicos *novo_servico){
   
    if(*servicos == NULL){

        *servicos = novo_servico;
        novo_servico->prox = NULL;
    }else{

        novo_servico->prox = *servicos;
        *servicos = novo_servico; 
    }
}

/* Insere lista prioridade */
void insere_prioridade(reg_prioridade **prioridade, reg_prioridade *nova_prioridade){

    if(*prioridade == NULL){

        *prioridade = nova_prioridade;
        nova_prioridade->prox = NULL;
    }else{

        nova_prioridade->prox = *prioridade;
        *prioridade = nova_prioridade;
    }
}

/* Insere uma nave na lista */
void insere_nave(reg_naves **nave, reg_naves *nova_nave){

    if(*nave == NULL){

        *nave = nova_nave;
        nova_nave->prox = NULL;
    }else{

        nova_nave->prox = *nave;
        *nave = nova_nave;
    }
}

/* Insere uma nave na lista de atendimentos de forma ordenada. */
void inserir_nave_ordenado(reg_lst_atendimento **atendimento, reg_lst_atendimento *novoatendimento){
    
    reg_lst_atendimento *atual = *atendimento; 
    reg_lst_atendimento *anterior = NULL;

    /* Encontra a posição correta de inserção baseada na prioridade */
    while(atual != NULL && atual->prioridade <= novoatendimento->prioridade){
        anterior = atual;
        atual = atual->prox;
    }

    /* Insere no início da lista se não houver anterior */
    if(anterior == NULL){

        /* Verifica se a nave atual está com serviço em andamento */
        if((*atendimento) != NULL && (*atendimento)->servicos->sendo_feito == 'S'){
            novoatendimento->prox = (*atendimento)->prox;
            (*atendimento)->prox = novoatendimento;
        } 
        else{
            novoatendimento->prox = *atendimento;
            *atendimento = novoatendimento;
        }
    } 
    else{

        /* Insere o novo atendimento após o nó anterior */
        novoatendimento->prox = atual;
        anterior->prox = novoatendimento;
    }
}


/* Insere a nave ordenada na lista do relatório 1 */
void inserir_ordenada_relatorio(lst_R1 **R1, reg_lst_atendimento *atendimento, int aux){
    
    lst_R1 *nova_nave, *p, *q;
    p = *R1;
    q = NULL;

    while(p != NULL && p->NIN < atendimento->NIN){
        q = p;
        p = p->prox;
    }

    /* Se a nave já existe na lista ele só vai mexer nos serviços*/
    if(p != NULL && p->NIN == atendimento->NIN) {
        p->qtd_servico += 1;
        p->custo_nave += atendimento->servicos->custo;
        p->tempo_ancoragem = aux;
        return;
    }

    /* Adiciona na lista */
    nova_nave = (lst_R1 *)malloc(sizeof(lst_R1));
    nova_nave->NIN = atendimento->NIN;
    strcpy(nova_nave->tipo_nave,atendimento->tipo_nave);
    nova_nave->qtd_servico = 1;
    nova_nave->custo_nave = atendimento->servicos->custo;
    nova_nave->tempo_ancoragem = aux;
    nova_nave->max_ancorado = 0;
    nova_nave->min_ancorado = 0;
    nova_nave->qtd_vezes_na_lista = 0; 
    
    /* Ajusta o ponteiro de R1 */
    nova_nave->prox = p;

    if(q == NULL)
        *R1 = nova_nave;

    else
        q->prox = nova_nave; 
    
}

/* Inserindo serviço de forma ordenada */
void inserir_servico_ordenado(reg_servicos **servicos, int codigo_servico, int tempo_servico, float custo){
    
    reg_servicos *atual = *servicos;
    reg_servicos *anterior = NULL;
    reg_servicos *novo_servico;
   
    novo_servico = (reg_servicos *) malloc(sizeof(reg_servicos));

    if(novo_servico == NULL){
        printf("Erro ao alocar memória para o novo serviço.\n");
        return;
    }

    /* Configura os valores do novo serviço */
    novo_servico->codigo = codigo_servico;
    novo_servico->tempo_servico = tempo_servico;
    novo_servico->custo = custo;
    novo_servico->sendo_feito = 'N';
    novo_servico->prox = NULL;

    /* Insere em ordem crescente de tempo de serviço */
    while(atual != NULL && atual->tempo_servico <= tempo_servico){
        anterior = atual;
        atual = atual->prox;
    }

    /* Insere o novo serviço no início da lista, se não houver anterior */
    if(anterior == NULL){

        novo_servico->prox = *servicos;
        *servicos = novo_servico;
    } 
    else{

        /* Insere o novo serviço após a célula anterior */
        novo_servico->prox = atual;
        anterior->prox = novo_servico;
    }
}

/* Função usada para ordenar o relatório pelo tipo */
void inserir_ordenado_tipo_nave(lst_R1 **relatorio){

    lst_R1 *lst_ordenada = NULL;
    lst_R1 *atual = *relatorio;
    lst_R1 *proximo;
    lst_R1 *aux;

    while(atual != NULL){
        
        proximo = atual->prox;

        /* Insere no início da lista ordenada se estiver vazia ou se o tipo for menor */
        if(lst_ordenada == NULL || strcmp(atual->tipo_nave, lst_ordenada->tipo_nave) < 0){
           
            atual->prox = lst_ordenada;
            lst_ordenada = atual;
        }
        else{
            
            /* Percorre a lista ordenada até encontrar a posição correta */
            aux = lst_ordenada;
            
            while(aux->prox != NULL && strcmp(aux->prox->tipo_nave, atual->tipo_nave) < 0)
                aux = aux->prox;
            
            /* Insere a célula atual na posição correta */
            atual->prox = aux->prox;
            aux->prox = atual;
        }

        /* Avança para o próximo nó da lista original */
        atual = proximo;

    }
    /* Atualiza a lista original para apontar para a lista ordenada */
    *relatorio = lst_ordenada;
}



/*************************BUSCA**************************/



/* Busca tempo serviço */
int busca_tempo_servico(reg_servicos *servicos, int codigo_servico){

    int tempo_servico;
    reg_servicos *p;
    p = servicos;

    while (p != NULL){
        if (p->codigo == codigo_servico){  

            tempo_servico = p->tempo_servico;
            p = NULL;
        }
        else
            p = p->prox;
    }
    return tempo_servico; 
}

/* Busca custo */
float busca_custo_servico(reg_servicos *servicos, int codigo_servico){

    float custo_servico;
    reg_servicos *p;
    p = servicos;

    while (p != NULL){
        if(p->codigo == codigo_servico){  
            custo_servico = p->custo;
            p = NULL;
        }
        else
            p = p->prox;
    }
    return custo_servico; 
}

/* Busca o NIN */
reg_lst_atendimento* busca_nin_atendimento(reg_lst_atendimento *atendimento, int NIN){

    while(atendimento != NULL){
    
        if(atendimento->NIN == NIN)
            return atendimento;
        
        atendimento = atendimento->prox;
    }
    return NULL;
}

/* Busca a prioridade com base no NIN da nave */
int buscar_prioridade(int nin, reg_naves *naves, reg_prioridade *prioridades){

    reg_naves *nave_atual = naves;
    reg_prioridade *prioridade_atual;

    /* Localiza a nave pelo NIN */
    while(nave_atual != NULL && nave_atual->NIN != nin)
        nave_atual = nave_atual->prox;

    /* Verifica se a nave foi encontrada */
    if (nave_atual == NULL){
        printf("Nave com NIN %d não encontrada.\n", nin);
        return -1;
    }

    prioridade_atual = prioridades;
    
    /* Procura a prioridade correspondente ao tipo da nave */
    while(prioridade_atual != NULL){

        if(strcmp(prioridade_atual->tipo_nave, nave_atual->tipo_nave) == 0)
            return prioridade_atual->numero_prioridade; 
        
        prioridade_atual = prioridade_atual->prox;
    }
    
    /* Retorna -1 se não encontrou a prioridade para o tipo da nave */
    return -1;
}


/* Busca o tipo da nave */
reg_naves* busca_tipo_nave(int NIN, reg_naves *naves){

    while(naves != NULL){

        if(naves->NIN == NIN)
            return naves;

        naves = naves -> prox;
    }
    return NULL;

}


/*************************OUTRAS FUNÇÕES**************************/



/* Conta o tempo total em minutos considerando os serviços pendentes e os descansos entre os serviços */
int conta_minutos(reg_lst_atendimento *atendimento, int descanso, int *qtd_servicos){
    
    reg_lst_atendimento *p = atendimento; 
    int contador = 0;  
    *qtd_servicos = 0;  
    
    /* Percorre todos os atendimentos */
    while(p != NULL){

        *qtd_servicos += p->qtd_servicos_a_fazer;  /* Acumula a quantidade total de serviços */
        /* Soma o tempo total de serviço e os tempos de descanso */
        contador += p->tempo_total_servico + (p->qtd_servicos_a_fazer * descanso); 
        p = p->prox;  
    }
    
    /* Subtrai o descanso final (não há descanso após o último serviço) */
    contador -= descanso;
    return contador; 
}


/* Atualiza os tempos de espera e ancoragem para cada nave na lista de atendimento */
void colocar_tempo_espera(reg_lst_atendimento **atendimento, int tempo_simulacao){

    reg_lst_atendimento *p;
    p = *atendimento;
    
    /* Itera pela lista de atendimento para somar o tempo de simulação aos tempos de espera e ancoragem */
    while(p != NULL){
        p->tempo_ancorado_atual += tempo_simulacao;          /* Incrementa o tempo atual de ancoragem */
        p->tempo_espera_troca_prioridade += tempo_simulacao; /* Incrementa o tempo de espera para troca de prioridade */
        p = p->prox;
    }
}

