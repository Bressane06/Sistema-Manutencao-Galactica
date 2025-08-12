/******************************************
*					                      *
* Gabriel Fiorentino Bressane		      *
* Trabalho Prático 			              *
* Disciplina: Algoritmos e Programaçãoo II*
* Professores: Ivone e Ronaldo            *
* 				                    	  *
******************************************/

/* Registro de informações sobre os serviços */
typedef struct cel_servicos{
    int codigo;  /* Código do serviço */
    int tempo_servico;  /* Tempo necessário para realizar o serviço */
    char descricao[30];  /* Descrição do serviço */
    char sendo_feito;  /* Indicador se o serviço está sendo feito ('S' ou 'N') */
    float custo;  /* Custo do serviço */
    struct cel_servicos *prox;  /* Ponteiro para o próximo serviço na lista */
} reg_servicos;

/* Registro de informações sobre as naves */
typedef struct cel_naves{
    int NIN;  /* Número de Identificação da Nave */
    char tipo_nave[30];  /* Tipo da nave */
    struct cel_naves *prox;  /* Ponteiro para a próxima nave na lista */
} reg_naves;

/* Registro de informações sobre as prioridades */
typedef struct cel_prioridades{
    int numero_prioridade;  /* Número da prioridade, indicando a ordem de atendimento */
    char tipo_nave[30];  /* Tipo de nave associado a essa prioridade */
    struct cel_prioridades *prox;  /* Ponteiro para a próxima prioridade na lista */
} reg_prioridade;

/* Registro que representa a lista de atendimento */
typedef struct cel_atendimento{
   reg_servicos *servicos;  /* Ponteiro para a lista de serviços associados ao atendimento */
   int tempo_total_servico;  /* Tempo total necessário para completar todos os serviços do atendimento */
   int qtd_servicos_a_fazer;  /* Quantidade de serviços pendentes para essa nave */
   int NIN;  /* Número de Identificação da Nave */
   int prioridade;  /* Prioridade do atendimento para a nave */
   int ordem_chegada;  /* Ordem de chegada da nave, usado para controle de lista */
   int tempo_espera_troca_prioridade;  /* Tempo que a nave aguardou parar trocar de prioridade */
   int tempo_ancorado_atual;  /* Tempo atual de ancoragem */
   char tipo_nave[30];  /* Tipo da nave sendo atendida */
   struct cel_atendimento *prox;  /* Ponteiro para o próximo atendimento na lista */
} reg_lst_atendimento;

/* Registro que auxilia o relatório */
typedef struct cel_r1{
    int qtd_servico;  /* Quantidade de serviços realizados para essa nave */
    int NIN;  /* Número de Identificação da Nave */
    int max_ancorado;  /* Máximo de tempo que a nave ficou ancorada */
    int min_ancorado;  /* Mínimo de tempo que a nave ficou ancorada */
    int tempo_ancoragem;  /* Tempo de ancoragem atual da nave */
    int tempo_total_ancorado;  /* Tempo total que a nave ficou ancorada */
    int qtd_vezes_na_lista;  /* Quantidade de vezes que a nave entrou na lista de ancoragem */
    char tipo_nave[30];  /* Tipo da nave */
    float custo_nave;  /* Custo total acumulado para essa nave */
    struct cel_r1 *prox;  /* Ponteiro para a próxima nave na lista R1 */
} lst_R1;

/* Registro do relatório */
typedef struct{
    lst_R1 *lst_naves_feitas;  /* Lista das naves que já foram atendidas e estão finalizadas */
    int tempo_total;  /* Tempo total acumulado para todos os atendimentos */
    int servicos_feitos;  /* Quantidade total de serviços realizados */
    float custo_total;  /* Custo total de todos os serviços realizados */
} reg_relatorio;


/*********************LIBERAR MEMÓRIA*******************/

void liberar_lista_servicos(reg_servicos **servicos);

void liberar_lista_naves(reg_naves **naves);

void liberar_lista_prioridade(reg_prioridade **prioridade);

void liberar_servico_atendimento(reg_lst_atendimento **atendimento);

void liberar_relatorio(reg_relatorio *relatorio);


/********************INSERIR**********************/


void insere_servico(reg_servicos **servicos, reg_servicos *novo_servico);

void insere_prioridade(reg_prioridade **prioridade, reg_prioridade *nova_prioridade);

void insere_nave(reg_naves **nave, reg_naves *nova_nave);

void inserir_nave_ordenado(reg_lst_atendimento **atendimento, reg_lst_atendimento *novoatendimento);

void inserir_ordenada_relatorio(lst_R1 **R1, reg_lst_atendimento *atendimento, int aux);

void inserir_servico_ordenado(reg_servicos **servicos, int codigo_servico, int tempo_servico, float custo);

void inserir_ordenado_tipo_nave(lst_R1 **relatorio);


/*************************BUSCA**************************/


int busca_tempo_servico(reg_servicos *servicos, int codigo_servico);

float busca_custo_servico(reg_servicos *servicos, int codigo_servico);

reg_naves* busca_tipo_nave(int NIN, reg_naves *naves);

reg_lst_atendimento* busca_nin_atendimento(reg_lst_atendimento *atendimento, int NIN);

int buscar_prioridade(int nin, reg_naves *naves, reg_prioridade *prioridades);


/*************************Relatórios**************************/
/* As funções de relatório estão no arquivo relatorios.c */


void iniciar_relatorio(reg_relatorio *relatorio);

void max_min_relatorio(lst_R1 *R1, FILE *ptr_arq);

void colocar_max_min(lst_R1 **r1, reg_lst_atendimento *atendimento, int descanso);

void criar_relatorio1(reg_relatorio relatorio);

void criar_relatorio2(reg_lst_atendimento *atendimento, int descanso);

void criar_relatorio_final(reg_relatorio relatorio, int tempo_trabalho, int descanso);



/*************************OUTRAS FUNÇÕES**************************/


int conta_minutos(reg_lst_atendimento *atendimento, int descanso, int *aux);

void colocar_tempo_espera(reg_lst_atendimento **atendimento, int tempo_simulacao);
