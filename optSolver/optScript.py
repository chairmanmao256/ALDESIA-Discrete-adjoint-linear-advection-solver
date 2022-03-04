import pyALDESIA
import optFuncs
import numpy as np
from pyoptsparse import SNOPT, Optimization

solver = pyALDESIA.ALDESIA(b'input.ini')

# specify the number of design variables
nDV = 2500
DVList = np.zeros(nDV, dtype = np.float32)

# set the design variable according to the 
# input specification
# for i in range(nDV):
#     if solver.isInBox(i):
#         DVList[i] = 1.0

xdict = {}
xdict["xvar"] = DVList

optFuncs.solver = solver

# define the optimization object
optProb = Optimization("ALDESIA's Test", optFuncs.evalFunc)

# design variables
optProb.addVarGroup("xvar", nDV, "c", value = DVList, lower = 0.0, upper = 2.0)

# add constraints
optProb.addConGroup("con", 1, lower = 0.0, upper = 0.04)

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
sol = opt(optProb, sens = optFuncs.sens)

#check the solution
print(sol)