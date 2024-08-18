import sys
import os
import matplotlib.pyplot as plt

def read_coordinates(filename):
    coords = []
    bairros = []
    with open(filename, 'r') as file:
        n = int(file.readline().strip())

        for _ in range(n):
            line = file.readline().strip()
            parts = line.split(';')
            x = float(parts[-2])
            y = float(parts[-1])
            bairro = parts[5]
            
            coords.append((x, y))
            bairros.append(bairro)

    return coords, bairros

def image_plot(coords, bairros, outfile, show_names=False):
    xs, ys = zip(*coords)

    plt.figure(figsize=(10, 10))
    plt.scatter(xs, ys, color='gray', marker='+')
    plt.title('Coordenadas geográficas')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.grid(False)

    if show_names:
        unique_bairros = {}
        for x, y, bairro in zip(xs, ys, bairros):
            if bairro not in unique_bairros:
                plt.text(x + 50, y + 50, bairro, fontsize=12, fontweight='bold', ha='right')
                unique_bairros[bairro] = True

    if os.path.exists(outfile):
        name_base, ext = os.path.splitext(outfile)
        i = 1
        while os.path.exists(f"{name_base}-{i}{ext}"):
            i += 1
        outfile = f"{name_base}-{i}{ext}"

    plt.savefig(outfile)

def main():
    if len(sys.argv) < 2:
        print("uso: python3 script.py nome_do_arquivo (opcional: nome_imagem) [-name]")
        sys.exit(1)

    infile = sys.argv[1]
    outfile = sys.argv[2] if len(sys.argv) > 2 and not sys.argv[2].startswith('-') else 'mapa.png'
    show_names = '-name' in sys.argv

    coords, bairros = read_coordinates(infile)

    image_plot(coords, bairros, outfile, show_names)

if __name__ == '__main__':
    main()

