# Sistema de Simulação de Atendimento de Naves Espaciais

**Disciplina:** Algoritmos e Programação II  

## Descrição

Este projeto implementa um sistema de simulação para atendimento de naves espaciais em uma estação de serviços. O sistema gerencia filas de atendimento baseadas em prioridades, tipos de serviços e ordem de chegada das naves.

## Estrutura do Projeto

```
├── src/                    # Código fonte
│   ├── main.c             # Arquivo principal com a função main
│   ├── funcoes.c          # Funções auxiliares (busca, inserção, liberação de memória)
│   ├── relatorios.c       # Funções para geração de relatórios
│   └── lib.h              # Biblioteca com definições de estruturas e protótipos
├── config/                # Arquivos de configuração
│   ├── simulacao.cfg      # Configurações do sistema (serviços, prioridades, naves)
│   └── simulacao.cmd      # Comandos de simulação
├── docs/                  # Documentação e logs
└── README.md              # Este arquivo
```

## Estruturas de Dados

O sistema utiliza **listas encadeadas** como estrutura principal:

- **reg_servicos**: Lista de serviços disponíveis
- **reg_naves**: Lista de naves cadastradas
- **reg_prioridade**: Lista de prioridades por tipo de nave
- **reg_lst_atendimento**: Lista de atendimentos em andamento
- **lst_R1**: Lista auxiliar para relatórios

## Funcionalidades

### Comandos Disponíveis

- **`n`**: Adicionar nave para atendimento
- **`s`**: Executar simulação por tempo determinado
- **`r`**: Gerar relatórios (tipo 1 ou 2)
- **`x`**: Executar todos os serviços pendentes

### Sistema de Prioridades

O atendimento segue a ordem de prioridade:
1. **Maior prioridade** (menor número = maior prioridade)
2. **Menor tempo total de serviço**
3. **Ordem de chegada** (primeiro a chegar)

### Tipos de Relatórios

1. **Relatório 1**: Naves já atendidas
2. **Relatório 2**: Naves pendentes de atendimento
3. **Relatório Final**: Estatísticas gerais (salvo em `simulacao.log`)

## Compilação e Execução

### Usando o Makefile (Recomendado):
```bash
# Compilar o projeto
make

# Compilar e executar automaticamente
make run

# Limpar arquivos gerados
make clean

# Ver todos os comandos disponíveis
make help
```

### Compilação manual (alternativa):
```bash
cd src
gcc -o simulador main.c funcoes.c relatorios.c -I.
```

**Nota:** O Makefile automaticamente copia os arquivos de configuração necessários (`simulacao.cfg` e `simulacao.cmd`) do diretório `config/` para execução.

## Arquivos de Configuração

### simulacao.cfg
```
[tempo_prioridade]
[tempo_descanso]
[quantidade_servicos]
[codigo] [descricao] [tempo] [custo]
...
[quantidade_tipos_prioridade]
[tipo_nave] [prioridade]
...
[NIN] [tipo_nave]
...
```

### simulacao.cmd
Sequência de comandos para execução:
```
n [NIN] s [codigo_servico]
s [tempo_simulacao]
r [tipo_relatorio]
x
```

## Exemplo de Uso

1. Configure os serviços e naves em `simulacao.cfg`
2. Defina a sequência de comandos em `simulacao.cmd`
3. Execute o simulador
4. Consulte os relatórios gerados
5. Sem o `.cfg` e o `.cmd` o código não compilará!
6. Deixei exemplos dos arquivos
    
## Características Técnicas

- **Linguagem**: C
- **Estrutura de dados**: Listas encadeadas
- **Gerenciamento de memória**: Alocação dinâmica com liberação adequada
- **Arquivos**: Leitura de configuração e geração de logs
- **Algoritmos**: Ordenação por prioridade e busca em listas

## Funcionalidades Principais

- Gerenciamento de filas por prioridade
- Simulação temporal de atendimento
- Controle de tempo de descanso entre serviços
- Mudança dinâmica de prioridades
- Geração de relatórios detalhados
- Cálculo de custos e tempos de ancoragem

## Autor

Desenvolvido como trabalho prático para a disciplina de Algoritmos e Programação II.