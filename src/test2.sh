#!/bin/bash
#SBATCH --output=test2.out
#SBATCH --ntasks=4            
#SBATCH --cpus-per-task=4     
#SBATCH --nodes=4             
#SBATCH --exclusive

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

for i in {1..10}   
do
    echo "-------- EXECUÇÃO $i --------"
    mpirun -np 4 ./main -q=128 -p=400000 -d=300 -k=1024 -t=$OMP_NUM_THREADS
    echo ""
done

