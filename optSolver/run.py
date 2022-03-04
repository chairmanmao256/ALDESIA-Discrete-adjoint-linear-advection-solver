import pyALDESIA

# specify the number of design variables
nDV = 2500
DVList = [0.0] * 2500

# initialize ALDESIA
solver = pyALDESIA.ALDESIA(b'input.ini')

for j in range(1): 
    # set the design variable according to the 
    # input specification
    for i in range(nDV):
        if solver.isInBox(i):
            DVList[i] = 1.0 + j * 0.2
        solver.setDesignVariable(i,DVList[i])

    # solve primal problem
    solver.solvePrimal()

    # get the objVal
    objVal = solver.calcObj(b"averageTempreture")

    # write the primal solution
    solver.writePrimal()

    solver.solveDA(b"averageTempreture")

# solve the Discrete-Adjoint
# solver.solveDA(b"averageTempreture")