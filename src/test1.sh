#!/bin/bash
#SBATCH --job-name=test1
#SBATCH --output=test1.out
#SBATCH --ntasks=2            # 2 processos MPI
#SBATCH --cpus-per-task=8     # 8 threads por processo (OpenMP)
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

for i in {1..10}
do
    echo "-------- EXECUCAO $i --------"
    mpirun -np $SLURM_NTASKS ./main -q=128 -p=400000 -d=300 -k=1024 -t=$OMP_NUM_THREADS
done
