# Nome do executável
EXEC = naive1

# Parâmetros de entrada
PARAMS = 610167.846027439 7795602.43183451

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
