#cython language level = 3
cdef extern from "Aldesia.cpp":
    pass

from libcpp.string cimport string
from libcpp cimport bool

cdef extern from "Aldesia.h":
    cdef cppclass Aldesia:
        Aldesia(string inputFile) except+
        void setDesignVariable(int oneDimensionalIndex, double val) except+
        bool isInBox(int oneDimensionalIndex) except+
        void solvePrimal() except+
        void solveDA(string objName) except+
        double calcObj(string objName) except+
        void writePrimal() except+
        void printHeader() except+