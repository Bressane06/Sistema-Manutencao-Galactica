# Makefile para o Sistema de Simulação de Naves Espaciais
# Autor: Gabriel Fiorentino Bressane

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SRCDIR = src
CONFIGDIR = config
DOCSDIR = docs
TARGET = simulador

# Arquivos fonte
SOURCES = $(SRCDIR)/main.c $(SRCDIR)/funcoes.c $(SRCDIR)/relatorios.c
HEADERS = $(SRCDIR)/lib.h

# Regra principal
all: $(TARGET)

# Compilação do executável
$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) -I$(SRCDIR)

# Executar o programa
run: $(TARGET)
	@echo "Executando simulador..."
	@./$(TARGET)
	@echo "Simulação concluída! Verifique o relatório em $(DOCSDIR)/simulacao.log"

# Limpeza básica
clean:
	rm -f $(TARGET)

# Limpeza completa
distclean: clean
	rm -f $(DOCSDIR)/simulacao.log

# Testar compilação
test: $(TARGET)
	@echo "Compilação bem-sucedida!"
	@echo "Execute 'make run' para rodar o simulador"

# Ajuda
help:
	@echo "Sistema de Simulação de Naves Espaciais"
	@echo "========================================"
	@echo "Comandos disponíveis:"
	@echo "  make        - Compila o projeto"
	@echo "  make run    - Compila e executa o programa"
	@echo "  make test   - Testa apenas a compilação"
	@echo "  make clean  - Remove arquivos temporários"
	@echo "  make distclean - Remove todos os arquivos gerados"
	@echo "  make help   - Mostra esta ajuda"
	@echo ""
	@echo "Estrutura do projeto:"
	@echo "  $(SRCDIR)/     - Código fonte"
	@echo "  $(CONFIGDIR)/  - Arquivos de configuração"
	@echo "  $(DOCSDIR)/    - Documentação e logs"

.PHONY: all run clean distclean test help