import sys
import csv
import os

# Funcao    : file_name
# Decricao  : gerencia nome do arquivo de saida
def file_name(base_name):
    if not os.path.exists(base_name):
        return base_name

    i = 1
    while True:
        name = f"{base_name.split('.')[0]}-{i}.base"
        if not os.path.exists(name):
            return name
        i += 1

# Funcao    : main
# Descricao : DPPC
def main():
    if len(sys.argv) < 3:
        print("Uso: python3 DPPC.py num_keywords keyword1 \"keyword com espaço\" keyword3 ")
        sys.exit(1)
    
    num_keywords = int(sys.argv[1])
    keywords = [keyword.strip() for keyword in sys.argv[2:]]

    if len(keywords) != num_keywords:
        print("Número de palavras-chave incorreto")
        sys.exit(1)

    infile = '../enderecobh.csv'
    base_name = 'geracarga.base'
    outfile = file_name(base_name)

    try:
        with open(infile, mode='r', encoding='utf-8') as csvfile:
            reader = csv.reader(csvfile, delimiter=';')

            count = 0
            filtered_lines = []

            for line in reader:
                line_str = " ".join(line[:-2])
                x = line[-2].replace(',', '.')
                y = line[-1].replace(',', '.')

                updated_line = line[:-2] + [x, y]
                
                if any(keyword in line_str for keyword in keywords):
                    filtered_lines.append(updated_line)
                    count += 1

            with open(outfile, mode='w', encoding='utf-8') as out_csvfile:
                writer = csv.writer(out_csvfile, delimiter=';')

                writer.writerow([f"{count}"])
                writer.writerows(filtered_lines)

    except FileNotFoundError:
        print(f"O arquivo {infile} deve estar no diretório ..")
        sys.exit(1)

if __name__ == "__main__":
    main()

