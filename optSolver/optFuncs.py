import numpy as np

def evalFunc(xdict):
    x = xdict["xvar"]
    funcs = {}

    # set the source term according to the design variable
    for idx, val in enumerate(x):
        solver.setDesignVariable(idx, val)

    # solve the primal
    solver.solvePrimal()

    funcs['obj'] = -1.0 * solver.calcObj(b"averageTempreture")
    funcs['con'] = solver.calcObj(b"sourceSum")

    fail = False

    return funcs, fail

def sens(xdict, funcs):
    nDV = len(xdict["xvar"])
    sensitivityObj = np.zeros(nDV, dtype = np.float32)
    sensitivityCon = np.zeros(nDV, dtype = np.float32)

    # solve the DA to get gradient
    solver.solveDA(b"averageTempreture")

    for i in range(nDV):
        sensitivityObj[i] = -1.0 * solver.getGrad(i)

    # solve the DA to get gradient
    solver.solveDA(b"sourceSum")

    for i in range(nDV):
        sensitivityCon[i] = solver.getGrad(i)

    funcsSens = {
        "obj":{
            "xvar":sensitivityObj,
        },
        "con":{
            "xvar":sensitivityCon,
        },
    }

    return funcsSens, False
