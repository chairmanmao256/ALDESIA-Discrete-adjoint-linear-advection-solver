# compile the class: ConfigMap
cd config
make all
cd ..

# compile the class: volScalarField, volVectorField, mesh
cd field
make all
cd ..

# compile the Solvers
cd Solver
make all
cd ..

# compile the file writer
cd writer
make all
cd ..

# compile the residual computation
cd Residual
make all
cd ..

# compile the objective functions
cd ObjFuncs
make all
cd ..

# compile the AdjointSolver
cd AdjointSolver
make all
cd ..

# link all the objs and compile the main program
# cythonize Aldesia
cd Aldesia
make all
python setup.py install
cd ..

# install the optimization utilities
cd optFuncs
python setup.py install
cd ..