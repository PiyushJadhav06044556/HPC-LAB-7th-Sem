git clone https://github.com/PiyushJadhav06044556/HPC-LAB-7th-Sem.git

Commands for CUDA Programs :-

1. Run this cell to confirm GPU and CUDA are available:

!nvidia-smi
!nvcc --version

2. Upload the CUDA program file and check

!ls

3. Compile the CUDA Program

!nvcc your_program_name.cu -o your_program_name

4.Run the Compiled Program

!./your_program_name

Alternate way to run CUDA Programs :-

1. Paste your CUDA code in a Colab cell

Use the special %%writefile magic command at the top of your cell.
This tells Colab to save the pasted code as a .cu file.

2. Compile your CUDA program

!nvcc device_info.cu -o device_info

3. Run it

!./device_info

Commands for OpenMP Code:-

Compile :
gcc -fopenmp file_name.c -o file_name

Run:
./file_name

Commands for MPI Code :-
Compile:
mpicc filename.c -o file_name

Run:
mpirun -np <number_of_processes> ./file_name
