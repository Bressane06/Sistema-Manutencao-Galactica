/******************************************
*					                      *
* Gabriel Fiorentino Bressane		      *
* Trabalho Prático 			              *
* Disciplina: Algoritmos e Programaçãoo II*
* Professores: Ivone e Ronaldo            *
* 				                    	  *
******************************************/

/* SÃO 3 ARQUIVOS DE LINGUAGEM C E 1 DE BIBLIOTECA */
/* 1 RESPONSÁVEL PELA MAIN (bressane.gabriel.c) */
/* 1 RESPONSÁVEL PELAS FUNÇÕES DOS RELATÓRIOS (relatorios.c) */
/* 1 RESPONSÁVEL POR FUNÇÕES DIVERSAS, COMO BUSCA E INSERÇÃO (funcoes.c) */
/* 1 RESPONSÁVEL PELA BIBLIOTECA (lib.h)*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Biblioteca feita para esse trabalho */
#include "lib.h"


/* A estrutura de dados selecionada para esse trabalho foi a lista encadeada */
/* Criação de 4 registros, ou seja, as tabelas
* Cada registro, será uma lista encadeada *
* EXEMPLO DA LISTA ENCADEADA
                          ----------    ----------
           lst_servico -> |servico2| -> |servico1| -> NULL
                          ----------    ----------
               ^              ^              ^
            ponteiro        celula        celula 
*/



/* Funções */


/* Insere uma nave no atendimento */
void inserir_atendimento(reg_lst_atendimento **atendimento, reg_naves *naves, reg_servicos *servicos, reg_prioridade *prioridade, FILE *ptr_arq, int *ordem_chegada, int *tempo_trabalho);

/* Lê o arquivo e preenche as listas de naves, prioridades e serviços. */
void ler_arquivo(FILE *ptr_arq, reg_naves **nave, reg_prioridade **prioridade, reg_servicos **servicos);

/* Coloca a nave que deve ser feita no início da lista */
void selecionar_nave_para_fazer(reg_lst_atendimento **atendimento);

/* Simula o tempo informado */
int simular(reg_lst_atendimento **atendimento, int descanso, int tempo_simulacao, reg_relatorio *relatorio);

/* Aumenta a prioridade */
void muda_prioridade(reg_lst_atendimento **atendimento, int tempo_prioridade);


/* MAIN */

int main(void){

    int descanso, tempo_prioridade, ordem_chegada, aux;
    int tempo_simulacao, tipo_relatorio, tempo_sobrando, tempo_trabalho;

    char configuracao[] = "config/simulacao.cfg";
    char arquivo_comandos[] = "config/simulacao.cmd";
    char comando;
    
    FILE *ptr_arq;
    reg_naves *nave = NULL;
    reg_prioridade *prioridade = NULL;
    reg_servicos *servicos = NULL;
    reg_lst_atendimento *atendimento = NULL;
    reg_relatorio relatorio;

    
    /* ARQUIVO DE CONFIGURAÇÃO */
    ptr_arq = fopen(configuracao, "r");
    
    if(ptr_arq == NULL){
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    fscanf(ptr_arq, "%d", &tempo_prioridade);
    fscanf(ptr_arq, "%d", &descanso);
    
    /* Leitura do arquivo de config */
    ler_arquivo(ptr_arq, &nave, &prioridade, &servicos);
    
    fclose(ptr_arq);
    
    /* Arquivo de comandos*/
    
    ptr_arq = fopen(arquivo_comandos, "r");
        
    if(ptr_arq == NULL){
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    /* Inicializa as variáveis do relatório */
    iniciar_relatorio(&relatorio);

    /* Fazendo as iterações */
    ordem_chegada = 0;  
    tempo_sobrando = 0;
    tempo_trabalho = 0;
    aux = 0;
    tempo_simulacao = 0;
    tipo_relatorio = 0;
      
    while((fscanf(ptr_arq, "%c", &comando)) != EOF){ 
                  
	    switch(comando){
	    
		/* Colocar uma nave para serviço */
		case 'n':

		    inserir_atendimento(&atendimento, nave, servicos, prioridade, ptr_arq, &ordem_chegada, &tempo_trabalho);
		    break;
		    
        /* Simulação */
		case 's':

		    fscanf(ptr_arq, "%d", &tempo_simulacao);

		    /* Verificar qual item é o que deve ser feito, respeitando a ordem de preferencia *
		    * - MAIOR PRIORIDADE *
		    * - MENOR TEMPO DE SERVIÇO TOTAL *
		    * - QUEM CHEGOU PRIMEIRO */

            /* Aux controla o tempo de simulação + tempo de iterações anteriores */
		    aux = tempo_simulacao;
		    relatorio.tempo_total += tempo_simulacao;
		    aux -= tempo_sobrando;

		    /* Caso o valor de simulação seja menor que o tempo que estava de descanso */   
		    if(aux < 0)
		        tempo_sobrando = -aux;
		    
		    else
		        tempo_sobrando = simular(&atendimento, descanso, aux, &relatorio);
  
		    muda_prioridade(&atendimento, tempo_prioridade);   

		    break;
		    
        /* Fazer os relatórios */    
		case 'r':

			fscanf(ptr_arq, "%d", &tipo_relatorio);
				if(tipo_relatorio == 1)	
					criar_relatorio1(relatorio);
				
			    if(tipo_relatorio == 2)
			        criar_relatorio2(atendimento, descanso);
						
		    break;                                 

        /* Avança até o final da lista */
		case 'x':

            /* Nesse caso, aux é responsável por contar quantos serviços faltam a serem realizados */        
            tempo_simulacao = conta_minutos(atendimento, descanso, &aux);
            tempo_simulacao = tempo_simulacao + (descanso * (aux - 1));
		    
		    tempo_sobrando = simular(&atendimento, descanso, tempo_simulacao, &relatorio);

            relatorio.tempo_total = tempo_trabalho + (relatorio.servicos_feitos * descanso - descanso);

		    break;                                 
	    }
 
    }
          
    
    criar_relatorio_final(relatorio, tempo_trabalho, descanso);    
    fclose(ptr_arq);

    /* Fazer o free nas listas */    
    liberar_lista_naves(&nave);
    liberar_lista_prioridade(&prioridade);
    liberar_lista_servicos(&servicos);
    liberar_relatorio(&relatorio);

    
    return 0;
}

/* FUNÇÕES PRINCIPAIS */

/* Responsável por fazer a simulação do código */
int simular(reg_lst_atendimento **atendimento, int descanso, int tempo_simulacao, reg_relatorio *relatorio){

    int tempo_decorrido = 0;  
    int tempo_faltando;  
    int aux;  /* Variável auxiliar para armazenar tempo de ancoragem atual */
    reg_servicos *servico_atual; 

    /* Laço principal da simulação: executa enquanto houver atendimentos e o tempo total não for atingido */
    while(*atendimento != NULL && tempo_decorrido < tempo_simulacao){
        
        /* Seleciona a nave que deve ser atendida conforme as prioridades */
        selecionar_nave_para_fazer(&(*atendimento));
        
        /* Aponta para o serviço atual do primeiro atendimento da lista */
        servico_atual = (*atendimento)->servicos;

        if(servico_atual != NULL){

            /* Caso o serviço possa ser concluído dentro do tempo de simulação */
            if(servico_atual->tempo_servico <= (tempo_simulacao - tempo_decorrido)){
                
                tempo_decorrido += servico_atual->tempo_servico + descanso;

                relatorio->servicos_feitos += 1;              
                relatorio->custo_total += servico_atual->custo;
                (*atendimento)->qtd_servicos_a_fazer -= 1;

                /* Atualiza o tempo de espera dos atendimentos com o tempo do serviço atual */
                colocar_tempo_espera(&(*atendimento), servico_atual->tempo_servico);

                /* Armazena o tempo de ancoragem atual para ser registrado no relatório */
                aux = (*atendimento)->tempo_ancorado_atual;
                
                /* Insere o atendimento finalizado na lista de naves atendidas */
                inserir_ordenada_relatorio(&(relatorio->lst_naves_feitas), *atendimento, aux);

                (*atendimento)->tempo_total_servico -= servico_atual->tempo_servico;

                /* Libera o serviço atual do atendimento, já que foi finalizado */
                liberar_servico_atendimento(&(*atendimento));

                /* Adiciona o tempo de descanso no tempo de espera dos atendimentos */
                colocar_tempo_espera(&(*atendimento), descanso);

            } 
            else{

                /* Caso o serviço não possa ser concluído no tempo restante da simulação */
                tempo_faltando = tempo_simulacao - tempo_decorrido;

                /* Atualiza o tempo de espera dos atendimentos com o tempo faltante */
                colocar_tempo_espera(&(*atendimento), tempo_faltando);

                /* Reduz o tempo do serviço atual pelo tempo faltante */
                servico_atual->tempo_servico -= tempo_faltando;
                servico_atual->sendo_feito = 'S';
                (*atendimento)->tempo_total_servico -= tempo_faltando;
                tempo_decorrido = tempo_simulacao;
            }
        }

        /* Caso todos os serviços da nave atual estejam concluídos */
        if((*atendimento)->servicos == NULL)
        {
            /* Registra os tempos máximo e mínimo de espera no relatório */
            colocar_max_min(&(relatorio->lst_naves_feitas), *atendimento, descanso);

            *atendimento = (*atendimento)->prox;
        }
    }

    /* Retorna o tempo excedente, caso o tempo decorrido tenha ultrapassado o tempo de simulação */
    if (tempo_decorrido > tempo_simulacao)
        return tempo_decorrido - tempo_simulacao;

    /* Retorna 0 caso o tempo decorrido esteja dentro do limite da simulação */
    return 0;
}


/* Inserir atendimentos na lista de atendimento */
void inserir_atendimento(reg_lst_atendimento **atendimento, reg_naves *naves, reg_servicos *servicos, reg_prioridade *prioridades, FILE *ptr_arq, int *ordem_chegada, int *tempo_trabalho){
    int codigo_servico;
    int tempo_servico; 
    float custo;
    char servico;
    reg_lst_atendimento *existe, *novoatendimento;
    reg_naves *p;

    novoatendimento = (reg_lst_atendimento *) malloc(sizeof(reg_lst_atendimento));
    if(novoatendimento == NULL){
        printf("Erro ao alocar memória.\n");
        return;
    }

    /* Lê o NIN da nave e busca sua prioridade */
    fscanf(ptr_arq, "%d", &novoatendimento->NIN);
    novoatendimento->prioridade = buscar_prioridade(novoatendimento->NIN, naves, prioridades);

    /* Lê o código do serviço */
    fscanf(ptr_arq, " %c", &servico);
    fscanf(ptr_arq, "%d", &codigo_servico);

    /* Busca custo */
    custo = busca_custo_servico(servicos, codigo_servico);

    /* Busca o tempo do serviço */
    tempo_servico = busca_tempo_servico(servicos, codigo_servico);
    *tempo_trabalho += tempo_servico;
    
    /* Busca o tipo da nave */
    p = busca_tipo_nave(novoatendimento->NIN, naves);

    /* Verifica se já existe um atendimento de uma mesma nave */
    existe = busca_nin_atendimento(*atendimento, novoatendimento->NIN);

    if(existe == NULL){
        /* Caso não exista, inicializa uma sublista de serviços e insere o novo serviço */
        novoatendimento->servicos = NULL;
        inserir_servico_ordenado(&novoatendimento->servicos, codigo_servico, tempo_servico, custo);


        novoatendimento->tempo_total_servico = tempo_servico;
        novoatendimento->qtd_servicos_a_fazer = 1;
        strcpy(novoatendimento->tipo_nave, p->tipo_nave);
        novoatendimento->ordem_chegada = *ordem_chegada + 1;
        novoatendimento->tempo_espera_troca_prioridade = 0;
        *ordem_chegada += 1;

        /* Insere o novo atendimento na lista de atendimentos */
        inserir_nave_ordenado(&(*atendimento), novoatendimento);
    } 
    else{
                
        /* Caso já exista essa nave, insere o novo serviço na lista de serviços */
        inserir_servico_ordenado(&existe->servicos, codigo_servico, tempo_servico, custo);
        existe->tempo_total_servico += tempo_servico;
        existe->qtd_servicos_a_fazer += 1;

        /* Libera, já que não vai ser necessário */
        free(novoatendimento);
    }
}


/* Responsável por mudar as prioridades */
void muda_prioridade(reg_lst_atendimento **atendimento, int tempo_prioridade){

    reg_lst_atendimento *p;
    int divisao;
    
    p = *atendimento;

    /* Percorre cada atendimento para ajustar a prioridade */
    while(p != NULL){

        /* Calcula quantas vezes o tempo de troca de prioridade cabe no tempo de espera */
        divisao = p->tempo_espera_troca_prioridade / tempo_prioridade;

        if(divisao > 0){

            /* Reduz a prioridade com base na divisão calculada */
            p->prioridade -= divisao;
            
            /* Garante que a prioridade não seja negativa */
            if(p->prioridade < 0)
                p->prioridade = 0;
        }
          
        /* Atualiza o tempo de espera para o resto da divisão */
        p->tempo_espera_troca_prioridade = p->tempo_espera_troca_prioridade % tempo_prioridade;
        
        /* Avança para o próximo atendimento */
        p = p->prox;
    }
}


/* Lê o arquivo de configuração e insere serviços, prioridades e naves nas listas */
void ler_arquivo(FILE *ptr_arq, reg_naves **nave, reg_prioridade **prioridade, reg_servicos **servicos){
    
    int valor, i;

    reg_servicos *novo_servico;        
    reg_prioridade *nova_prioridade;    
    reg_naves *nova_nave;    
        
    /* Lê a quantidade de serviços */
    fscanf(ptr_arq, "%d", &valor);

    /* Lê os serviços */
    for (i = 0; i < valor; i++){

        novo_servico = (reg_servicos *) malloc(sizeof(reg_servicos));

        if (novo_servico == NULL)
            printf("Erro ao alocar memória para novo serviço\n");
        
        else{

            fscanf(ptr_arq, "%d %s %d %f", &novo_servico->codigo, novo_servico->descricao, &novo_servico->tempo_servico, &novo_servico->custo);
            insere_servico(servicos, novo_servico);
        }
    }

    /* Lê a quantidade de prioridades */
    fscanf(ptr_arq, "%d", &valor);

    /* Lê as prioridades */
    for(i = 0; i < valor; i++){

        nova_prioridade = (reg_prioridade *) malloc(sizeof(reg_prioridade));

        if(nova_prioridade == NULL)
            printf("Erro ao alocar memória para nova prioridade\n");
        
        else{

            fscanf(ptr_arq, " %s %d", nova_prioridade->tipo_nave, &nova_prioridade->numero_prioridade);
            insere_prioridade(prioridade, nova_prioridade);
        }
    }

    /* Lê as naves */
    while(fscanf(ptr_arq, "%d", &valor) != EOF){

        nova_nave = (reg_naves *) malloc(sizeof(reg_naves));

        if(nova_nave == NULL)
            printf("Erro ao alocar memória para nova nave\n");
        
        else{

            nova_nave->NIN = valor;
            fscanf(ptr_arq, " %s", nova_nave->tipo_nave);
            insere_nave(nave, nova_nave);
        }
    }
}


void selecionar_nave_para_fazer(reg_lst_atendimento **atendimento){

    reg_lst_atendimento *p, *maior, *anterior, *anterior_maior; 
    int maior_prioridade;
    
    maior_prioridade = (*atendimento)->prioridade;  
    p = *atendimento; 
    maior = *atendimento;
    anterior = NULL;
    anterior_maior = NULL;
    
    /* Não fazer quando não finalizou o servico anterior */
    if((*atendimento)->servicos != NULL && (*atendimento)->servicos->sendo_feito == 'S')
        return;
    
    /* Verifica qual nave tem a maior prioridade, seguindo os critérios de desempate */
    while(p != NULL){

        /* Casos onde o maior prioridade se encontra fora da posição */
        if(p->prioridade < maior_prioridade){

            maior_prioridade = p->prioridade;
            maior = p;
            anterior_maior = anterior;
        }
        else{ 

            if(p->prioridade == maior_prioridade){

                /* Desempate pelo maior tempo total de serviço */
                if(p->tempo_total_servico > maior->tempo_total_servico){
                    maior = p;
                    anterior_maior = anterior;
                } 
                else{ 
                    if(p->tempo_total_servico == maior->tempo_total_servico){

                        /* Desempate pela ordem de chegada */
                        if(p->ordem_chegada < maior->ordem_chegada){

                            maior = p;
                            anterior_maior = anterior;
                        }
                    }
                }
            }
        }

        anterior = p;
        p = p->prox;
    }

    /* Manda a nave com maior prioridade para o início */
    if(maior != *atendimento){

        if(anterior_maior != NULL)
            anterior_maior->prox = maior->prox;  
        
        maior->prox = *atendimento;  
        *atendimento = maior;
    }
}
