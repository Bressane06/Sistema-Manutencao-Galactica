# Documentação Técnica

## Estruturas de Dados

### reg_servicos
```c
typedef struct cel_servicos {
    int codigo;              // Código único do serviço
    int tempo_servico;       // Tempo necessário em minutos
    char descricao[30];      // Descrição do serviço
    char sendo_feito;        // Status: 'S' = em execução, 'N' = pendente
    float custo;             // Custo em unidades galácticas
    struct cel_servicos *prox;
} reg_servicos;
```

### reg_naves
```c
typedef struct cel_naves {
    int NIN;                 // Número de Identificação da Nave
    char tipo_nave[30];      // Tipo da nave
    struct cel_naves *prox;  // Ponteiro para a próxima nave
} reg_naves;
```

### reg_prioridade
```c
typedef struct cel_prioridades {
    int numero_prioridade;           // Número da prioridade (menor = maior prioridade)
    char tipo_nave[30];              // Tipo de nave associado
    struct cel_prioridades *prox;    // Ponteiro para a próxima prioridade
} reg_prioridade;
```

### reg_lst_atendimento
```c
typedef struct cel_atendimento {
    reg_servicos *servicos;           // Lista de serviços da nave
    int tempo_total_servico;          // Tempo total pendente
    int qtd_servicos_a_fazer;         // Quantidade de serviços pendentes
    int NIN;                          // Número de Identificação da Nave
    int prioridade;                   // Prioridade atual (0 = maior prioridade)
    int ordem_chegada;                // Ordem de chegada para desempate
    int tempo_espera_troca_prioridade; // Tempo para mudança de prioridade
    int tempo_ancorado_atual;         // Tempo atual de ancoragem
    char tipo_nave[30];               // Tipo da nave
    struct cel_atendimento *prox;
} reg_lst_atendimento;
```

### lst_R1 (Lista de Relatório)
```c
typedef struct cel_r1 {
    int qtd_servico;              // Quantidade de serviços realizados
    int NIN;                      // Número de Identificação da Nave
    int max_ancorado;             // Tempo máximo de ancoragem
    int min_ancorado;             // Tempo mínimo de ancoragem
    int tempo_ancoragem;          // Tempo de ancoragem atual
    int tempo_total_ancorado;     // Tempo total ancorado
    int qtd_vezes_na_lista;       // Quantas vezes entrou na lista
    char tipo_nave[30];           // Tipo da nave
    float custo_nave;             // Custo total acumulado
    struct cel_r1 *prox;          // Ponteiro para próxima nave
} lst_R1;
```

### reg_relatorio
```c
typedef struct {
    lst_R1 *lst_naves_feitas;     // Lista de naves atendidas
    int tempo_total;              // Tempo total de simulação
    int servicos_feitos;          // Total de serviços realizados
    float custo_total;            // Custo total acumulado
} reg_relatorio;
```

## Algoritmos Principais

### Seleção de Nave para Atendimento
1. Verifica se há serviço em andamento
2. Busca nave com menor número de prioridade
3. Em caso de empate: menor tempo total de serviço
4. Em caso de empate: ordem de chegada (FIFO)

### Simulação Temporal
1. Seleciona nave prioritária
2. Executa serviço se tempo suficiente
3. Atualiza tempos de espera de todas as naves
4. Remove serviços concluídos
5. Aplica tempo de descanso

### Mudança de Prioridade
- A cada intervalo configurado, diminui a prioridade das naves em espera
- Evita que naves fiquem indefinidamente na fila

## Fluxo de Execução

1. **Inicialização**
   - Lê configurações (`simulacao.cfg`)
   - Carrega serviços, prioridades e naves
   - Inicializa estruturas de relatório

2. **Processamento de Comandos**
   - Lê comandos de `simulacao.cmd`
   - Processa cada comando sequencialmente

3. **Finalização**
   - Gera relatório final
   - Libera memória alocada

## Comandos Detalhados

### Comando 'n' (Nova Nave)
```
n [NIN] s [codigo_servico]
```
- Adiciona serviço à nave existente ou cria nova entrada
- Mantém ordenação por prioridade na lista

### Comando 's' (Simulação)
```
s [tempo_minutos]
```
- Executa simulação pelo tempo especificado
- Atualiza todos os tempos de espera
- Aplica mudanças de prioridade

### Comando 'r' (Relatório)
```
r [tipo]
```
- Tipo 1: Naves atendidas
- Tipo 2: Naves pendentes

### Comando 'x' (Executar Tudo)
- Calcula tempo necessário para todos os serviços
- Executa simulação até conclusão

## Gerenciamento de Memória

- Todas as estruturas usam alocação dinâmica
- Liberação adequada ao final do programa
- Remoção de nós conforme serviços são concluídos

## Arquivos de Saída

### simulacao.log
- Relatório geral da simulação
- Estatísticas por tipo de nave
- Tempos máximo, mínimo e médio de ancoragem
