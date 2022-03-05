#distutils:language=c++
#cython:language_level=3

from pyALDESIAdef cimport Aldesia
from libcpp.string cimport string
from libcpp cimport bool

cdef class ALDESIA:
    cdef Aldesia* AldesiaPtr

    def __cinit__(self):
        self.AldesiaPtr = NULL

    def __dealloc__(self):
        print("deallocation is performed")
        if self.AldesiaPtr != NULL:
            del self.AldesiaPtr

    def __init__(self, string inputFile):
        self.AldesiaPtr = new Aldesia(inputFile)

    def setDesignVariable(self, int oneDimensionalIndex, double val):
        self.AldesiaPtr.setDesignVariable(oneDimensionalIndex, val)

    def isInBox(self, int oneDimensionalIndex):
        return self.AldesiaPtr.isInBox(oneDimensionalIndex)

    def solvePrimal(self):
        self.AldesiaPtr.solvePrimal()

    def solveDA(self, string objName):
        self.AldesiaPtr.solveDA(objName)

    def calcObj(self, string objName):
        return self.AldesiaPtr.calcObj(objName)

    def writePrimal(self):
        self.AldesiaPtr.writePrimal()

    def printHeader(self):
        self.AldesiaPtr.printHeader()

    def getGrad(self, int i):
        return self.AldesiaPtr.getGrad(i)

    def updateSolveAdjoint(self):
        self.AldesiaPtr.updateSolveAdjoint()