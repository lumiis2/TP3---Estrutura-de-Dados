import sys
import os
import subprocess

def create_eventos_directory():
    base_dir = 'eventos'
    if not os.path.exists(base_dir):
        os.makedirs(base_dir)
        return base_dir
    i = 1
    while os.path.exists(f"{base_dir}-{i}"):
        i += 1
    os.makedirs(f"{base_dir}-{i}")
    return f"{base_dir}-{i}"

def generate_files(max_commands, percentage, command_type, output_dir):
    step = 50
    current_commands = 50
    
    while current_commands <= max_commands:
        output_filename = f"{output_dir}/geracarga-{current_commands}.ev"
        command = [
            'python3', 'EVDENSITY.py', str(current_commands), 'geracarga.base', str(percentage), command_type
        ]
        
        subprocess.run(command)
        
        if os.path.exists("geracarga.ev"):
            os.rename("geracarga.ev", output_filename)
        
        current_commands += step

def main():
    if len(sys.argv) != 4:
        print("uso: python3 ITERATOR.py numero_maximo_de_comandos porcentagem -A/-D/-C")
        sys.exit(1)

    max_commands = int(sys.argv[1])
    percentage = float(sys.argv[2])
    command_type = sys.argv[3]

    if command_type not in ['-A', '-D', '-C']:
        print("Tipo de comando invalido")
        sys.exit(1)

    output_dir = create_eventos_directory()
    generate_files(max_commands, percentage, command_type, output_dir)

if __name__ == '__main__':
    main()
