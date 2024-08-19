import subprocess
import os
import pandas as pd
import numpy as np
from sklearn.cluster import KMeans


def preprocess_coordinates(df):
    # Verifique se as colunas são do tipo string e converta se necessário
    if df['x'].dtype == object:
        df['x'] = df['x'].str.replace(',', '.').astype(float)
    else:
        df['x'] = df['x'].astype(float)
    
    if df['y'].dtype == object:
        df['y'] = df['y'].str.replace(',', '.').astype(float)
    else:
        df['y'] = df['y'].astype(float)
    
    return df

def distribute_uniformly(df, num_points):
    df = preprocess_coordinates(df)
    coords = df[['x', 'y']].values
    kmeans = KMeans(n_clusters=num_points, random_state=0).fit(coords)
    centroids = kmeans.cluster_centers_
    
    closest_points = []
    for centroid in centroids:
        distances = np.linalg.norm(coords - centroid, axis=1)
        closest_point_index = np.argmin(distances)
        closest_points.append(df.iloc[closest_point_index])
    
    return pd.DataFrame(closest_points)

def save_to_base_file(df, filename):
    num_rows = len(df)
    with open(filename, 'w', encoding='utf-8') as file:
        file.write(f"{num_rows}\n")
        df.to_csv(file, index=False, header=False, sep=';')

def generate_ev_file(filename):
    # Gera um arquivo .ev vazio ou com dados de exemplo
    with open(filename, 'w') as file:
        pass  # Aqui você pode adicionar dados conforme necessário

def run_cpp_program(basefile, evfile):
    executable_path = os.path.join("..", "PA-TP3", "bin", "tp3.out")
    command = [executable_path, "-b", basefile, "-e", evfile]
    result = subprocess.run(command, capture_output=True, text=True)
    return result.stdout, result.stderr

def parse_and_log_times(output, size, name):
    with open(f'logs/log_times{name}.txt', "a") as log_file:
        lines = output.splitlines()
        log_file.write(f"Dados para tamanho {size}:\n")
        for line in lines:
            if "Tempo de processamento:" in line or "Tempo de consulta:" in line or "Tempo de ativação:" in line or "Tempo de desativação:" in line:
                log_file.write(line + "\n")
        log_file.write("\n")

def main():
    df = pd.read_csv('enderecobh.csv', delimiter=';')
    name = "numXtime_processamento"
    
    sizes = list(range(10, 10010, 50))
    for size in sizes:
        print(".")
        uniform_points = distribute_uniformly(df, size)
        basefile = f'uniform_points_{size}.base'
        evfile = f'uniform_points_{size}.ev'
        
        save_to_base_file(uniform_points, basefile)
        generate_ev_file(evfile)  # Gera um arquivo .ev vazio ou com dados
    
        output, error = run_cpp_program(basefile, evfile)
        
        if error:
            print(f"Erro ao executar o programa C++: {error}")
            continue
        
        parse_and_log_times(output, size, name)
        
        os.remove(basefile)  # Remove o arquivo .base após o teste
        os.remove(evfile)  # Remove o arquivo .ev após o teste

if __name__ == "__main__":
    main()
