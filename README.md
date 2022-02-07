# AeroDAT

## Outline

AeroDAT is a tiny toy software developed by Chenyu Wu of AeroLab, School of Aerospace Engineering, Tsinghua University. The software aims to provide the undergraduate students who want to join our lab with a simple way to understand the basic work flow of DA (Discrete Adjoint method) in CFD. Now AeroDAT is still under construction.

## Capabilities

AeroDAT can now solve convection diffusion problem in a 2-D rectangular domain using rectangular grid with mixed 2nd-order central and 1st-order upwind algorithm (the primal solver). The equation solved can be written as:
$$
\nabla\cdot(\mathbf{u}\phi)=\nabla\cdot(\nu\nabla\phi) + S
$$
It can also derive the Jacobian of the primal solver. 

The picture below is the $\phi$ distribution in a square. The boundary value of $\phi$ is set to zero and there's a source located at the center of the domain. The velocity points in south-east direction.

<img src="./RecSource.jpg" style="zoom:33%;" />

The Jacobian of the primal solver is computed by forward mode AD of CoDiPack. Now the full Jacobian can be written in an ASCII file `dRdTJac.dat` (which would be too huge to afford when the number of grid points is large). The Jacobian of the primal solver using a $3\times3$ grid when the convection velocity is set to zero is:

```
-0.04000 0.01000 0.00000 0.01000 0.00000 0.00000 0.00000 0.00000 0.00000 
0.01000 -0.04000 0.01000 0.00000 0.01000 0.00000 0.00000 0.00000 0.00000 
0.00000 0.01000 -0.04000 0.00000 0.00000 0.01000 0.00000 0.00000 0.00000 
0.01000 0.00000 0.00000 -0.04000 0.01000 0.00000 0.01000 0.00000 0.00000 
0.00000 0.01000 0.00000 0.01000 -0.04000 0.01000 0.00000 0.01000 0.00000 
0.00000 0.00000 0.01000 0.00000 0.01000 -0.04000 0.00000 0.00000 0.01000 
0.00000 0.00000 0.00000 0.01000 0.00000 0.00000 -0.04000 0.01000 0.00000 
0.00000 0.00000 0.00000 0.00000 0.01000 0.00000 0.01000 -0.04000 0.01000 
0.00000 0.00000 0.00000 0.00000 0.00000 0.01000 0.00000 0.01000 -0.04000 
```

## Features

The solver is written by `C++` and the author has tried his best to make the code object-oriented. The program depends on `CoDiPack` AD tool kit. `Makefile` is used to control the compilation and the compilation is carried out by `g++` on `Ubuntu 20.04 LTS`. 

## TODOs

The adjoint solver is now still incomplete since the objective function is not implemented. The Jacobian with respect to design variables is also not implemented yet. 

The linear equation systems in AeroDAT are solved by SOR algorithm. GMRES method might be implemented in the future for both primal and adjoint solvers.

The optimization problem will be solved by using `pyOptSparse` library.

