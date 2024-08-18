import numpy as np
import pandas as pd
from sklearn.cluster import KMeans

def preprocess_coordinates(df):
    # Substituir vírgulas por pontos e converter para float
    df['x'] = df['x'].str.replace(',', '.').astype(float)
    df['y'] = df['y'].str.replace(',', '.').astype(float)
    return df

def distribute_uniformly(df, num_points):
    # Pré-processar as coordenadas
    df = preprocess_coordinates(df)

    # Convert latitude and longitude to numpy array
    coords = df[['x', 'y']].values

    # Use KMeans clustering to find centroids
    kmeans = KMeans(n_clusters=num_points, random_state=0).fit(coords)
    centroids = kmeans.cluster_centers_

    # Find the closest original point to each centroid
    closest_points = []
    for centroid in centroids:
        distances = np.linalg.norm(coords - centroid, axis=1)
        closest_point_index = np.argmin(distances)
        closest_points.append(df.iloc[closest_point_index])

    return pd.DataFrame(closest_points)

def save_to_base_file(df, filename):
    # Save the DataFrame to a .base file
    num_rows = len(df)
    with open(filename, 'w', encoding='utf-8') as file:
        # Write the number of rows as the first line
        file.write(f"{num_rows}\n")
        # Write the data without headers
        df.to_csv(file, index=False, header=False, sep=';')

# Load data
df = pd.read_csv('../enderecobh.csv', delimiter=';')

# Print the column names
print(df.columns)

# Example usage: select 50 uniform points
uniform_points = distribute_uniformly(df, 100)

# Save the result to a .base file
save_to_base_file(uniform_points, 'uniform_points.base')
