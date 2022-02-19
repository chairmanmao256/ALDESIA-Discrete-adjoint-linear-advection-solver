from setuptools import setup
from distutils.extension import Extension
from Cython.Build import cythonize

ext = [
    Extension(
        "pyALDESIA",
        sources=["pyALDESIA.pyx"],
        include_dirs = 
        ["../CoDiPackModule/include",
         "../AdjointSolver",
         "../config",
         "../field",
         "../ObjFuncs",
         "../Residual",
         "../Solver",
         "../writer",
        ],
        extra_objects= 
        ['../AdjointSolver/AdjointSolver.o',
         '../config/INIReader.o',
         '../config/ini.o',
         '../config/ConfigMap.o',
         '../field/mesh.o',
         '../field/volScalarField.o',
         '../field/volVectorField.o',
         '../ObjFuncs/DerivObj.o',
         '../ObjFuncs/objFuncs.o',
         '../ObjFuncs/objFuncAvgT.o',
         '../ObjFuncs/objFuncSourceSum.o',
         '../Residual/CDSolverResidual.o',
         '../Solver/CDSolver.o',
         '../writer/writeJac.o',
         '../writer/writePlt.o'
        ],
        extra_compile_args=
        [
         '-fPIC',
         '-std=c++11'
        ]
    )
]

setup(
    name = 'pyALDESIA app',
    ext_modules=cythonize(ext, language_level = 3),
    zip_safe = False,
)