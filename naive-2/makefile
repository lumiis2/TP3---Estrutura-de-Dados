# Nome do executável
EXEC = naive2

# Nome dos arquivos gerados
OUTPUT = out.eps
GNUPLOT_SCRIPT = out.gp

# Flags do compilador
CXXFLAGS = -Wall

# Alvo padrão
all: $(EXEC)
	./$(EXEC) $(PARAMS)
	gnuplot $(GNUPLOT_SCRIPT)
	evince $(OUTPUT)

# Compilação do código
$(EXEC): $(EXEC).cpp
	g++ $(CXXFLAGS) -o $(EXEC) $(EXEC).cpp -lm

# Limpeza dos arquivos gerados
clean:
	rm -rf $(EXEC) $(OUTPUT) $(GNUPLOT_SCRIPT) *.gpdat
