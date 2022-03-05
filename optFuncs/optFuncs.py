import numpy as np

def evalFunc(xdict):
    x = xdict["xvar"]
    funcs = {}
    funcs['obj'] = 0.0
    funcs['con'] = 0.0

    # set the source term according to the design variable
    for idx, val in enumerate(x):
        solver.setDesignVariable(idx, val)

    # solve the primal
    solver.solvePrimal()

    # calculate the objs in the dictionary
    objs = objFuncDict["objectives"]
    for key in objs.keys():
        funcs['obj'] += objs[key]["scale"] * solver.calcObj(objs[key]["name"].encode())

    # calculate the cons in the dictionary
    cons = objFuncDict["constraints"]
    for key in cons.keys():
        funcs['con'] += cons[key]["scale"] * solver.calcObj(cons[key]["name"].encode())

    fail = False

    return funcs, fail

def sens(xdict, funcs):
    nDV = len(xdict["xvar"])
    sensitivityObj = np.zeros(nDV, dtype = np.float32)
    sensitivityCon = np.zeros(nDV, dtype = np.float32)

    # calculate the gradient of objs in the dictionary
    objs = objFuncDict["objectives"]
    for key in objs.keys():
        solver.solveDA(objs[key]["name"].encode())
        for i in range(nDV):
            sensitivityObj[i] += objs[key]["scale"] * solver.getGrad(i)


    # calculate the gradient of objs in the dictionary
    cons = objFuncDict["constraints"]
    for key in cons.keys():
        solver.solveDA(cons[key]["name"].encode())
        for i in range(nDV):
            sensitivityCon[i] += cons[key]["scale"] * solver.getGrad(i)

    # assemble the sensitivity dictionary
    funcsSens = {
        "obj":{
            "xvar":sensitivityObj,
        },
        "con":{
            "xvar":sensitivityCon,
        },
    }

    # write primal
    solver.updateSolveAdjoint()
    solver.writePrimal()

    return funcsSens, False
