/******************************************
*					                      *
* Gabriel Fiorentino Bressane		      *
* Trabalho Prático 			              *
* Disciplina: Algoritmos e Programaçãoo II*
* Professores: Ivone e Ronaldo            *
* 				                    	  *
******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"


/* Escreve o relatório no terminal */
void criar_relatorio1(reg_relatorio relatorio){

    lst_R1 *nave_atendida; 
    int qtd_naves;
        
    printf("---- Relatório 1 - Naves Atendidas ----\n");
    printf("NIN\tTipo_Nave\tQtd\tCusto Galatico\n");
    
    nave_atendida = relatorio.lst_naves_feitas;  

    qtd_naves = 0;
    while(nave_atendida != NULL){

        /* Imprime os detalhes da nave atual */
        printf("%d\t%s\t%2d\t%10.2f\n", nave_atendida->NIN, nave_atendida->tipo_nave, nave_atendida->qtd_servico, nave_atendida->custo_nave);
        qtd_naves += 1;
        nave_atendida = nave_atendida->prox;  
    }
    printf("\n");  
    printf("Quantidade de minutos simulados: %d\n", relatorio.tempo_total);
    printf("Quantidade de naves atendidas: %d\n", qtd_naves);
    printf("Total de serviços realizados: %2d\n", relatorio.servicos_feitos);
    printf("Valor total recebido: %.2f\n", relatorio.custo_total);
    printf("\n");  
}

/* Cria relatório 2 */
void criar_relatorio2(reg_lst_atendimento *atendimento, int descanso){

    reg_lst_atendimento *p;
    reg_servicos *q; 
    int qtd_servico, naves, servicos, tempo;
    float custo, custo_total;

    /* Cabeçalho do relatório */
    printf("---- Relatório 2 - Naves a Serem Atendidas ----\n");
    printf("NIN\tTipo_Nave\tQtd\tCusto Galatico\n");

    p = atendimento;
    naves = 0; 
    servicos = 0;
    custo_total = 0.00;
    tempo = 0;

    while(p != NULL){  /* Percorre cada atendimento pendente */

        naves += 1;
        qtd_servico = 0;
        custo = 0.00;
        q = p->servicos;

        printf("%d\t%s\t", p->NIN, p->tipo_nave);

        while(q != NULL){  /* Processa cada serviço da nave */
            servicos += 1;
            qtd_servico += 1;
            custo += q->custo;
            tempo += q->tempo_servico;
            tempo += descanso;
            q = q->prox;
        }
        
        custo_total += custo;
        printf("%2d\t%10.2f\t\n", qtd_servico, custo);  
        p = p->prox;
    }

    /* Ajuste final do tempo total com descanso */
    if(tempo < descanso)
        tempo = 0;
    else
        tempo = tempo - descanso;

    /* Resumo do relatório */
    printf("\n");
    printf("Quantidade de minutos a serem simulados: %d\n", tempo);
    printf("Quantidade de naves a serem atendidas: %d\n", naves);
    printf("Total de serviços a serem feitos: %d\n", servicos);
    printf("Total valor a ser recebido: %.2f\n", custo_total);

    printf("\n");
}


/* RELATÓRIO QUE IRÁ NO ARQUIVO SIMULACAO.LOG */
void criar_relatorio_final(reg_relatorio relatorio, int tempo_trabalho, int descanso){

    FILE *ptr_arq;
    lst_R1 *p;
    int contador = 0, naves = 0;

    /* Ordena a lista de naves atendidas por tipo de nave */
    inserir_ordenado_tipo_nave(&(relatorio.lst_naves_feitas));
    p = relatorio.lst_naves_feitas;

    /* Abre o arquivo de relatório para escrita */
    ptr_arq = fopen("docs/simulacao.log", "w");

    if (ptr_arq == NULL){  
        printf("Erro ao abrir o arquivo simulacao.log.\n");
        return;
    }

    /* Cabeçalho do relatório */
    fprintf(ptr_arq, "Relatório Geral da Simulação\n\n");
    fprintf(ptr_arq, "Quantidade de minutos simulados: %d minutos.\n", tempo_trabalho + (relatorio.servicos_feitos * descanso - descanso));
    fprintf(ptr_arq, "Tempo total de trabalho: %d minutos.\n", tempo_trabalho);
    fprintf(ptr_arq, "Quantidade de serviços feitos: %d.\n", relatorio.servicos_feitos);

    fprintf(ptr_arq, "\nNaves atendidas:\n");
    fprintf(ptr_arq, "NIN\tTipo_Nave\tTempo_Ancorada\n");

    while(p != NULL){  /* Percorre e imprime cada nave atendida */
        fprintf(ptr_arq, "%d\t%s\t%d\n", p->NIN, p->tipo_nave, p->tempo_total_ancorado);
        p = p -> prox;
    }

    p = relatorio.lst_naves_feitas;
    fprintf(ptr_arq, "\nMáximo, Mínimo e Média de Espera de cada tipo de nave\n");
    fprintf(ptr_arq, "Tipo_Nave\tMAX\tMIN\tMedia\n");

    /* Chama a função que insere o tempo de ancoragem máxima e mínima */
    max_min_relatorio(p, ptr_arq); 

    /* Calcula o tempo médio de ancoragem */
    p = relatorio.lst_naves_feitas;
    while(p != NULL){
        contador += p->tempo_total_ancorado;
        naves += 1;
        p = p->prox;
    }
    fprintf(ptr_arq, "\nO tempo médio de ancoragem é de: %d minutos.\n", contador / naves);

    /* Fecha o arquivo de relatório */
    fclose(ptr_arq);
}


/* Inicia variáveis do relatório */
void iniciar_relatorio(reg_relatorio *relatorio){

    relatorio -> tempo_total = 0;
    relatorio -> servicos_feitos = 0;
    relatorio -> custo_total = 0.00;
    relatorio -> lst_naves_feitas = NULL;

}


/* Responsável por adicionar o tempo max e min de aconragem no relatório */
void colocar_max_min(lst_R1 **r1, reg_lst_atendimento *atendimento, int descanso){

    lst_R1 *p = *r1;

    /* Itera pela lista R1 para encontrar o registro com o mesmo NIN da nave em atendimento */
    while(p != NULL){

        if(p->NIN == atendimento->NIN){
        
            /* Atualiza o tempo máximo de ancoragem se o atual for maior */
            if(p->max_ancorado < atendimento->tempo_ancorado_atual)
                p->max_ancorado = atendimento->tempo_ancorado_atual - descanso;
            
            /* Atualiza o tempo mínimo de ancoragem se o atual for menor ou se min_ancorado não estiver definido */
            if(p->min_ancorado > atendimento->tempo_ancorado_atual || p->min_ancorado == 0) 
                p->min_ancorado = atendimento->tempo_ancorado_atual - descanso;
            
            /* Atualiza o tempo total ancorado considerando múltiplas entradas na lista */
            if(p->qtd_vezes_na_lista >= 1)

                p->tempo_total_ancorado += p->tempo_ancoragem;
                
            else
                p->tempo_total_ancorado = p->tempo_ancoragem;
            
            /* Incrementa o contador de vezes que a nave entrou na lista */
            p->qtd_vezes_na_lista += 1;
        }  

        p = p->prox;
    }
}


/* Usada para imprimir os max, min e media no relatórios */
void max_min_relatorio(lst_R1 *R1, FILE *ptr_arq){

    lst_R1 *p = R1, *q;
    int max, min, media_total, contador, media;

    while(p != NULL){  /* Percorre cada tipo de nave */

        max = p->max_ancorado;
        min = p->min_ancorado;
        media_total = 0;
        contador = 0;

        fprintf(ptr_arq, "%s\t", p->tipo_nave); 

        q = p;
        while(q != NULL && strcmp(q->tipo_nave, p->tipo_nave) == 0){  /* Itera pelas naves do mesmo tipo */
            
            /* Vê se é maior */
            if (q->max_ancorado > max)
                max = q->max_ancorado;

            /* Vê se é menor */            
            if (q->min_ancorado < min)
                min = q->min_ancorado;

            media_total += q->tempo_total_ancorado / q->qtd_vezes_na_lista;
            contador += 1;

            q = q->prox;
        }

        fprintf(ptr_arq, "%d\t", max);  
        fprintf(ptr_arq, "%d\t", min);  

        media = media_total / contador;
        fprintf(ptr_arq, "%d\n", media);

        p = q;  /* Avança para o próximo tipo de nave */
    }
}
