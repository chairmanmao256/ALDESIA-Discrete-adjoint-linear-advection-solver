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

# link all the objs and compile the main program
make all