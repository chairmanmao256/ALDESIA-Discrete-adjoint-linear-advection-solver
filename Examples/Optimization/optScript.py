import pyALDESIA
import optFuncs
import numpy as np
from pyoptsparse import SNOPT, Optimization

solver = pyALDESIA.ALDESIA(b'input.ini')

# specify the number of design variables
nDV = 6400
DVList = np.zeros(nDV, dtype = np.float32)

# set the design variable according to the 
# input specification
for i in range(nDV):
    if solver.isInBox(i):
        DVList[i] = 1.0
xdict = {}
xdict["xvar"] = DVList

# the objective function dictionary
objFuncDict = {
    "objectives":{
        "part1":{
            "name": "averageTempreture",
            "scale": -1.0,
        },
        "part2":{
            "name": "tempretureVariance",
            "scale": 6.0,
        }
    },
    "constraints":{
        "part1":{
            "name": "sourceSum",
            "scale": 1.0,
        }
    }
}

optFuncs.solver = solver
optFuncs.objFuncDict = objFuncDict

# define the optimization object
optProb = Optimization("ALDESIA's Test", optFuncs.evalFunc)

# design variables
optProb.addVarGroup("xvar", nDV, "c", value = DVList, lower = 0.0, upper = 2.0)

# add constraints
optProb.addConGroup("con", 1, lower = 0.04, upper = 0.04)

# add objective function
optProb.addObj("obj")

# check optimization problem
print(optProb)

# set the optimizaer
optOptions = {
    "Major feasibility tolerance": 1.0e-7,
    "Major optimality tolerance": 1.0e-7,
    "Minor feasibility tolerance": 1.0e-7,
    "Verify level": -1,
    "Function precision": 1.0e-6,
    "Hessian updates": 10,
    "Nonderivative linesearch": None,
    "Print file": "opt_SNOPT_print.txt",
    "Summary file": "opt_SNOPT_summary.txt",
}
opt = SNOPT(options = optOptions)

#solve the problem
sol = opt(optProb, sens = optFuncs.sens, storeHistory="snopt_hist.hst")

#check the solution
print(sol)