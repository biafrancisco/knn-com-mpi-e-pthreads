#!/bin/bash
#SBATCH --output=test1.out


# Calcula os 1024 vizinhos mais próximos de 128 pontos de 300 dimensoes em base de dados de 400mil pontos
#Experiencia 1:
# Rodar o programa na configuracao 1: (usando 16 nucleos no total) com 2 processos MPI apenas e 8 threads por processo     

for i in {1..10}  # 10 vezes cada
do
    echo "-------- EXECUCAO $i --------"
    mpirun -np 2 ./main -q=128 -p=400000 -d=300 -k=1024 -t=8
done