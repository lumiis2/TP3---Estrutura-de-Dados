import sys
import os
import random

def read_input_file(filename):
    with open(filename, 'r') as file:
        num_addresses = int(file.readline().strip())
        data = [line.strip().split(';') for line in file.readlines()]
    return num_addresses, data

def generate_commands(num_commands, data, percentage, command_type):
    num_specific = int(num_commands * percentage)
    num_remaining = num_commands - num_specific
    
    commands = []

    if command_type == '-A':
        for _ in range(num_specific):
            idend = random.choice(data)[0]
            commands.append(f"A {idend}")
        command_options = ['-D', '-C']
    elif command_type == '-D':
        for _ in range(num_specific):
            idend = random.choice(data)[0]
            commands.append(f"D {idend}")
        command_options = ['-A', '-C']
    elif command_type == '-C':
        for _ in range(num_specific):
            x = random.uniform(6000000, 8000000)
            y = random.uniform(6000000, 8000000)
            n_max = min(len(data), 20)
            n = random.randint(1, n_max)
            commands.append(f"C {x:.6f} {y:.6f} {n}")
        command_options = ['-A', '-D']

    for _ in range(num_remaining):
        other_command = random.choice(command_options)
        if other_command == '-A':
            idend = random.choice(data)[0]
            commands.append(f"A {idend}")
        elif other_command == '-D':
            idend = random.choice(data)[0]
            commands.append(f"D {idend}")
        elif other_command == '-C':
            x = random.uniform(6000000, 8000000)
            y = random.uniform(6000000, 8000000)
            n_max = min(len(data), 20)
            n = random.randint(1, n_max)
            commands.append(f"C {x:.6f} {y:.6f} {n}")

    random.shuffle(commands)
    return commands, num_specific

def get_output_filename(base_filename):
    if not os.path.exists(base_filename):
        return base_filename
    base_name, ext = os.path.splitext(base_filename)
    i = 1
    while os.path.exists(f"{base_name}-{i}{ext}"):
        i += 1
    return f"{base_name}-{i}{ext}"

def write_output_file(filename, num_commands, commands):
    with open(filename, 'w') as file:
        file.write(f"{num_commands}\n")
        file.write("\n".join(commands))

def main():
    if len(sys.argv) < 5:
        print("uso: python3 EVDENSITY.py numero_de_comandos arquivo_entrada porcentagem(entre 0 e 1) -A/-D/-C")
        sys.exit(1)

    num_commands = int(sys.argv[1])
    input_file = sys.argv[2]
    percentage = float(sys.argv[3])
    command_type = sys.argv[4]

    num_addresses, data = read_input_file(input_file)

    commands, num_specific = generate_commands(num_commands, data, percentage, command_type)

    output_filename = get_output_filename("geracarga.ev")
    write_output_file(output_filename, num_commands, commands)

    print(f"Total de comandos: {num_commands}")
    print(f"Densidade bruta: {num_specific} comandos em {num_commands} comandos")
    print(f"Densidade relativa: {percentage * 100:.2f}%")

if __name__ == '__main__':
    main()
