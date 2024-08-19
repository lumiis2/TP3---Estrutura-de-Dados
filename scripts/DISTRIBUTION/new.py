import numpy as np
import pandas as pd
import subprocess
import re
import time
from sklearn.cluster import MiniBatchKMeans
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter


def distribute_uniformly(df, num_points):
    # Pré-processar as coordenadas
    df = preprocess_coordinates(df)

    # Converter latitude e longitude para array numpy
    coords = df[['x', 'y']].values

    # Usar MiniBatchKMeans em vez de KMeans
    start_time = time.time()
    kmeans = MiniBatchKMeans(n_clusters=num_points, random_state=0, batch_size=100).fit(coords)
    centroids = kmeans.cluster_centers_
    end_time = time.time()
    # print(f"MiniBatchKMeans clustering took {end_time - start_time:.6f} seconds")

    # Encontrar o ponto original mais próximo de cada centróide
    start_time = time.time()
    closest_points = []
    for centroid in centroids:
        distances = np.linalg.norm(coords - centroid, axis=1)
        closest_point_index = np.argmin(distances)
        closest_points.append(df.iloc[closest_point_index])
    end_time = time.time()
    # print(f"Finding closest points to centroids took {end_time - start_time:.6f} seconds")

    return pd.DataFrame(closest_points)


def preprocess_coordinates(df):
    start_time = time.time()  # Início do timer

    # Verificar se as colunas 'x' e 'y' estão em formato de string
    if df['x'].dtype != 'object':
        df['x'] = df['x'].astype(str)
    if df['y'].dtype != 'object':
        df['y'] = df['y'].astype(str)

    # Substituir vírgulas por pontos e converter para float
    df['x'] = df['x'].str.replace(',', '.').astype(float)
    df['y'] = df['y'].str.replace(',', '.').astype(float)
    
    end_time = time.time()  # Fim do timer
    # print(f"Preprocessing coordinates took {end_time - start_time:.6f} seconds")
    return df

def save_to_base_file(df, filename):
    start_time = time.time()
    # Save the DataFrame to a .base file
    num_rows = len(df)
    with open(filename, 'w', encoding='utf-8') as file:
        # Write the number of rows as the first line
        file.write(f"{num_rows}\n")
        # Write the data without headers
        df.to_csv(file, index=False, header=False, sep=';')
    end_time = time.time()
    # print(f"Saving to base file took {end_time - start_time:.6f} seconds")

def generate_queries(df, query_type):
    start_time = time.time()
    # Generate queries based on the specified query type
    # num_points = len(df)
    # num_samples = innt(num_points * 0.1)
    num_points = 10
    num_samples = 10
    selected_points = df.sample(n=num_samples, random_state=1)  # Randomly select half the points

    queries = []
    
    if query_type == 'C':
        # Generate 'C' queries
        for _, point in selected_points.iterrows():
            x, y = point['x'], point['y']
            k = int(0.1 * num_points)  # Number of closest points to find
            queries.append(f"C {x} {y} {k}")
    
    elif query_type == 'A':
        # Generate 'A' queries
        for _, point in selected_points.iterrows():
            id_point = point['idend']  # Use the index as ID
            queries.append(f"A {id_point}")
    
    elif query_type == 'D':
        # Generate 'D' queries
        for _, point in selected_points.iterrows():
            id_point = point['idend']  # Use the index as ID
            queries.append(f"D {id_point}")

    end_time = time.time()
    # print(f"Generating queries took {end_time - start_time:.6f} seconds")

    # Return queries and their count
    return queries

def save_queries_with_base(df, query_type, base_filename, query_filename):
    start_time = time.time()
    queries = generate_queries(df, query_type)
    
    # Number of queries
    num_queries = len(queries)
    
    with open(base_filename, 'r', encoding='utf-8') as base_file:
        base_content = base_file.read()
    
    with open(query_filename, 'w', encoding='utf-8') as file:
        # Write the base file content
        file.write(base_content)
        # Write the number of queries
        file.write(f"{num_queries}\n")
        # Write the queries
        for query in queries:
            file.write(f"{query}\n")
    end_time = time.time()
    # print(f"Saving queries with base took {end_time - start_time:.6f} seconds")

def run_cpp_program(executable, input_file):
    start_time = time.time()
    # Run the C++ program and capture its output
    with open(input_file, 'r') as f:
        input_data = f.read()
    
    result = subprocess.run(
        [executable],
        input=input_data,
        text=True,
        capture_output=True
    )
    end_time = time.time()
    # print(f"Running C++ program took {end_time - start_time:.6f} seconds")
    
    return result.stdout

def parse_times(output):
    start_time = time.time()
    # Extract specific time measurements based on the provided output
    times = {
        'processamento': None,
        'consulta': None,
        'ativacao': None,
        'desativacao': None
    }

    # Extract processing time
    processing_time = re.search(r'Tempo de processamento: (\d+\.\d+) s', output)
    if processing_time:
        times['processamento'] = float(processing_time.group(1))
    
    # Extract activation time
    activation_times = re.findall(r'Tempo de ativação: (\d+\.\d+) s', output)
    if activation_times:
        times['ativacao'] = activation_times
    
    # Extract deactivation time
    desactivation_times = re.findall(r'Tempo de desativação: (\d+\.\d+) s', output)
    if desactivation_times:
        times['desativacao'] = desactivation_times
    
    # Extract query time
    query_times = re.findall(r'Tempo de consulta: (\d+\.\d+) s', output)
    if query_times:
        times['consulta'] = query_times

    end_time = time.time()
    # print(f"Parsing times took {end_time - start_time:.6f} seconds")
    
    return times

def save_specific_times(times, filename, query_type):
    start_time = time.time()
    # Save the extracted times to a file
    with open(filename, 'w') as f:
        if times['processamento'] is not None:
            f.write(f"Tempo de processamento: {times['processamento']:.6f}\n")
        if query_type == 'A' and times['ativacao'] is not None:
            # for t in times['ativacao']:
            #     f.write(f"{t}\n")
            total_ativacao = sum(float(t) for t in times['ativacao'])
            f.write(f"Total de ativação: {total_ativacao:.6f}\n")
        elif query_type == 'C' and times['consulta'] is not None:
            # for t in times['consulta']:
            f.write(f"Uma consulta: {float(times['consulta'][0]):.6f}\n")
            total_query = sum(float(t) for t in times['consulta'])
            f.write(f"Total de consulta: {total_query:.6f}\n")
        elif query_type == 'D' and times['desativacao'] is not None:
            # for t in times['desativacao']:
            #     f.write(f"{t}\n")
            total_desativacao = sum(float(t) for t in times['desativacao'])
            f.write(f"Total de desativação: {total_desativacao:.6f}\n")
    end_time = time.time()
    # print(f"Saving specific times took {end_time - start_time:.6f} seconds")


def run_experiments():
    # Inicializa as listas de acumulação
    process_time = []

    process_activation = []
    process_desactivation = []
    process_single_query = []
    process_all_query = []

    # Defina o intervalo e passo para N
    start_N = 10
    end_N = 10**3  # ou 10**6
    step_N = 25

    # Carregar os dados originais
    df = pd.read_csv('../enderecobh.csv', delimiter=';')

    # Loop sobre os diferentes tamanhos N
    for N in range(start_N, end_N + 1, step_N):
        # Seleciona N pontos de forma uniforme
        uniform_points = distribute_uniformly(df, N)

        # Salvar os pontos selecionados em um arquivo base
        base_filename = 'uniform_points.base'
        save_to_base_file(uniform_points, base_filename)

        # Gerar os três arquivos de consulta
        for query_type, filename in [('C', 'entradaC.in'), ('A', 'entradaA.in'), ('D', 'entradaD.in')]:
            save_queries_with_base(uniform_points, query_type, base_filename, filename)

        # Caminho para o executável C++
        executable_path = '../../PA-TP3/bin/tp3.out'

        # Processar cada arquivo de entrada
        for input_file in ['entradaA.in', 'entradaC.in', 'entradaD.in']:
            # Executar o programa C++ e capturar a saída
            output = run_cpp_program(executable_path, input_file)

            # Analisar os tempos na saída
            times = parse_times(output)

            # Acumular os tempos de processamento nas listas
            if input_file == 'entradaA.in' and times['processamento'] is not None:
                process_time.append(times['processamento'])

            if input_file == 'entradaA.in' and times['ativacao'] is not None:
                process_activation.append(sum(float(t) for t in times['ativacao']))

            if input_file == 'entradaD.in' and times['desativacao'] is not None:
                process_desactivation.append(sum(float(t) for t in times['desativacao']))

            if input_file == 'entradaC.in' and times['consulta'] is not None:
                process_single_query.append(float(times['consulta'][0]))
                process_all_query.append(sum(float(t) for t in times['consulta']))

        print(f"Finished processing for N = {N}")

    # Aqui você pode salvar os resultados acumulados em um arquivo ou processá-los como desejar
    np.save('process_time.npy', process_time)
    np.save('process_activation.npy', process_activation)
    np.save('process_desactivation.npy', process_desactivation)
    np.save('process_single_query.npy', process_single_query)
    np.save('process_all_query.npy', process_all_query)

    print("All experiments completed and data saved.")



run_experiments()


# Carregar os dados dos arquivos .npy
process_time = np.load('process_time.npy')
process_activation = np.load('process_activation.npy')
process_desactivation = np.load('process_desactivation.npy')
process_single_query = np.load('process_single_query.npy')
process_all_query = np.load('process_all_query.npy')

# Definir o intervalo de N
start_N = 10
end_N = 10**3  # ou 10**6
step_N = 25
num_points = list(range(start_N, end_N + 1, step_N))

# Verificar se todos os vetores têm o mesmo comprimento

assert len(process_time) == len(num_points)
assert len(process_activation) == len(num_points)
assert len(process_desactivation) == len(num_points)
assert len(process_single_query) == len(num_points)
assert len(process_all_query) == len(num_points)

# Função para formatar os rótulos dos eixos
def decimal_formatter(x, pos):
    """
    Formata os rótulos dos eixos em notação decimal.
    """
    return f'{x:.2f}'


# Criar o gráfico
plt.figure(figsize=(12, 8))

# Plotar cada tipo de tempo como scatter
plt.scatter(num_points, process_time, label='Tempo de Processamento', color='blue', marker='o')
plt.scatter(num_points, process_all_query, label='Total de Consultas', color='green', marker='s')
plt.scatter(num_points, process_single_query, label='Consulta Única', color='red', marker='^')
plt.scatter(num_points, process_activation, label='Tempo de Ativação', color='orange', marker='D')
plt.scatter(num_points, process_desactivation, label='Tempo de Desativação', color='purple', marker='x')

# Adicionar título e rótulos aos eixos
plt.title('Tempo x Número de Pontos')
plt.xlabel('Número de Pontos')
plt.ylabel('Tempo (segundos)')

# Definir escalas logarítmicas para os eixos
plt.xscale('log')
plt.yscale('log')

# Aplicar formatação decimal aos rótulos dos eixos
plt.gca().xaxis.set_major_formatter(FuncFormatter(decimal_formatter))
plt.gca().yaxis.set_major_formatter(FuncFormatter(decimal_formatter))

# Adicionar legenda e grade
plt.legend()
plt.grid(True, which="both", ls="--", linewidth=0.5)

# Salvar o gráfico em um arquivo
plt.savefig('grafico_tempos.png')

# Mostrar o gráfico
plt.show()


# # Load data
# start_time = time.time()
# df = pd.read_csv('../enderecobh.csv', delimiter=';')
# end_time = time.time()
# print(f"Loading data took {end_time - start_time:.6f} seconds")

# # Example usage: select 100 uniform points
# uniform_points = distribute_uniformly(df, 100)

# # Save the result to a .base file
# base_filename = 'uniform_points.base'
# save_to_base_file(uniform_points, base_filename)

# # Generate the three query files, combining them with the base file
# for query_type, filename in [('C', 'entradaC.in'), ('A', 'entradaA.in'), ('D', 'entradaD.in')]:
#     save_queries_with_base(uniform_points, query_type, base_filename, filename)

# # Paths to your C++ executable and input files
# executable_path = '../../PA-TP3/bin/tp3.out'
# input_files = ['entradaA.in', 'entradaC.in', 'entradaD.in']

# # Process each input file
# for input_file in input_files:
#     # Run the C++ program
#     output = run_cpp_program(executable_path, input_file)
    
#     # Parse the output to extract times
#     times = parse_times(output)
    
#     # Save the times to a file named after the input file
#     output_filename = f"{input_file}_times.txt"
#     save_specific_times(times, output_filename, input_file[7])

#     print(f"Times for {input_file} saved to {output_filename}")
