import subprocess
import sys
import os

def run_cpp_program(basefile, evfile):
    # Caminho para o executável
    executable_path = os.path.join("..", "PA-TP3", "bin", "tp3.out")
    
    # Comando para executar o programa C++
    command = [executable_path, "-b", basefile, "-e", evfile]
    
    # Executar o programa C++ e capturar a saída
    result = subprocess.run(command, capture_output=True, text=True)
    
    # Captura a saída e o erro
    output = result.stdout
    error = result.stderr
    
    return output, error

def parse_and_log_times(output):
    # Abre o arquivo de log para escrita
    with open("logs/log_times.txt", "w") as log_file:
        # Divida a saída em linhas
        lines = output.splitlines()
        
        for line in lines:
            # Verifique se a linha contém um tempo de processamento
            if "Tempo de processamento:" in line:
                log_file.write(line + "\n")
            elif "Tempo de consulta:" in line:
                log_file.write(line + "\n")
            elif "Tempo de ativação:" in line:
                log_file.write(line + "\n")
            elif "Tempo de desativação:" in line:
                log_file.write(line + "\n")

def main():
    if len(sys.argv) != 3:
        print("Uso: python SCRIPTS/scripty.py <arquivo_base> <arquivo_ev>")
        sys.exit(1)

    basefile = sys.argv[1]
    evfile = sys.argv[2]
    
    # Executa o programa C++ e obtém a saída
    output, error = run_cpp_program(basefile, evfile)
    
    # Verifica se ocorreu algum erro
    if error:
        print(f"Erro ao executar o programa C++: {error}")
        sys.exit(1)
    
    # Analisa e registra os tempos de execução
    parse_and_log_times(output)
    
    print("Tempos de execução registrados em 'SCRIPTS/log_times.txt'.")

if __name__ == "__main__":
    main()
