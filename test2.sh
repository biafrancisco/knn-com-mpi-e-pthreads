#!/bin/bash
#SBATCH --output=test2.out
#SBATCH --ntasks=8
#SBATCH --exclusive

# Calcula os 1024 vizinhos mais próximos de 128 pontos de 300 dimensoes em base de dados de 400mil pontos
#Experiencia 2:
# Rodar o programa na configuracao 2: (também usando 16 nucleos no total) com 4 processos MPI em hosts diferentes e 4 threads por processo 

for i in {1..10}  # 10 vezes cada
do
    echo "-------- EXECUCAO $i --------"
    mpirun -np 4 ./main -q=128 -p=400000 -d=300 -k=1024 -t=4
