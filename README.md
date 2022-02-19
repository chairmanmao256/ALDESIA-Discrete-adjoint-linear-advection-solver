![](./pictures/ALDESIA.svg)

## About

ALDESIA is a toy program written by `C++` that aims to guide the undergraduate students into the colorful world of CFD and Discrete-Adjoint method. ALDESIA can solve linear convection-diffusion problem and its adjoint. ALDESIA relies on `CodiPack` and `IniReader`.

As far as the author know, mastering CFD is not simple, not to mention understanding the Discrete-Adjoint algorithm that generate the gradient of a function defined by CFD. Popular software that deals with CFD and Discrete-Adjoint method appears to be to difficult for beginners to understand due to the huge volume of its code and the complexity of the problem it aims to solve. 

Different from the software mentioned above, ALDESIA solves simple problem (convection-diffusion problem, definitely not the kind of problem that can be written into an academic journal) and has only about 1600 lines of code (codes in `CodiPack`and `iniReader` are not counted). The author of ALDESIA believes that its simplicity can help the students who have weak or no background in CFD and Discrete-Adjoint take the very first steps in their scientific career and cultivate their interest in CFD and Aerodynamic Optimization.

ALDESIA is written by Wcy, School of Aerospace engineering, Tsinghua University. ALDESIA is certainly not perfect and bunch of flaws definitely exist since the author is also new in CFD and Discrete-Adjoint. Contact the author by sending emails to wucy18@mails.tsinghua.edu.cn if you have any suggestions to improve the performance of ALDESIA. 

## Features

### Problem and algorithm description

ALDESIA can now solve convection-diffusion problem in a 2-D rectangular domain using rectangular grid with mixed 2nd-order central and 1st-order upwind algorithm (the primal solver). The equation solved can be written as:

![](./pictures/c-dEq.svg)

The Jacobian of the convection-diffusion problem solver (primal) solver is derived by using `CoDiPack` auto-differentiation tool-kit with graph-coloring. The partial derivative of the objective function is also computed by the reverse-mode auto-differentiation of `CoDiPack`.  ALDESIA can solve the adjoint of the primal solver using the Jacobians and the derivatives derived:

![](./pictures/AdjEq.svg)

Note that currently ALDESIA treats the distribution of the source term (S) as the design variable.

All linear equations in ALDESIA are now solved by naïve SOR algorithm.

### Programming features

ALDESIA is objective-oriented. All functions are integrated into the class `Aldesia` as its methods. An input file that defines the problem the user want to solve is needed to initialize an object of `Aldesia` (examples of the input file can be found in the `Example` directory):

```c++
std::string filename = {"input.ini"};
Aldesia solver(filename);
```

Then the user can solve the primal and adjoint by simply calling:

```C++
// solve primal
solver.solvePrimal(); 
// solve adjoint, the string is the name of the obj
solver.solveDA("averageTempreture"); 
```

For more details, please examine the source file of class `Aldesia`.

### `python` Wrapper

`pyALDESIAdef.pxd` and `pyALDESIA.pyx` are two `cython` files that define the `python ` wrapper of ALDESIA. After correct installation of ALDESIA as an extension of `python`, one can use ALDESIA in a `python ` script by importing the package and instantiation an object of ALDESIA: (note that the construction of ALDESIA object in python also uses an `.ini` file)

```python
import pyALDESIA
solver = pyALDESIA.ALDESIA(b'input.ini')
```

## Installation

### Step 1: Compile the `C++` code

It is tested that ALDESIA can be compiled in `Ubuntu-20.04 LTS` using `g++ 9.3.0`. After downloading the zip file of this repository and unzip it in a Linux system, the user can compile ALDESIA by typing the following command in your terminal:

```shell
./Allmake.sh
```

After a successful compilation, the user can find `interface.out` in the directory.

### Step 2: Install ALDESIA as an extension of `python`

After activating the python virtual environment where you want to install ALDESIA, run the following command in the main directory of ALDESIA:

```shell
python setup.py install
```

To check if ALDESIA is correctly installed as an extension of `python`, run `python` in the terminal and type the following command:

```python
>>> import pyALDESIA
>>> solver = pyALDESIA.ALDESIA(b"input.ini")
```

If the header and the problem description is correctly printed, ALDESIA is correctly installed.

## Examples

### Convection of heat generated by a rectangular source

Copy `interface.out` to the directory: `Examples/PrimalSolver` and switch to directory `Examples/PrimalSolver`. Then type the following command:

```shell
./interface.out input.ini 10000
```

After pressing the enter key, ALDESIA will run the primal solver. The result will be stored in the file `result.plt`. Open `postProcessing.lay` using Tecplot for post-processing.

The picture below is the phi distribution generated by ALDESIA's primal solver. The boundary value of phi is set to zero and there's a source located at the center of the domain. The velocity points in south-east direction.

<img src="./pictures/RecSource.jpg" style="zoom:33%;" />

### Gradient of average temperature

Copy `interface.out` to the directory: `Examples/GradientAndJacobian` and switch to directory `Examples/GradientAndJacobian`. Then type the following command:

```shell
./interface.out input.ini 16
```

Then ALDESIA will solve both the primal and the adjoint on a 4-times-4 grid. The velocity is set to zero and the source term is set to 1.0 on the center 4 grid cells of the domain. The grid size is kept small for convenient check of the gradient calculated. After ALDESIA finishes its job, you will see the following files are generated in the current directory:

```
- dFdW.dat
- dFdx.dat
- dRdWColored.dat
- dRdXColored.dat
- gradient.dat
- result.plt
```

The first four files are the Jacobians and the last `.dat` file is the gradient of average temperature with respect to the distribution of the source. The `.plt` file stores the outcome of the primal solver.