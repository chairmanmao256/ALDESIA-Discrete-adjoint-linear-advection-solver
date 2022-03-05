cd config
make clean
cd ..

cd field
make clean
cd ..

cd Solver
make clean
cd ..

cd writer
make clean
cd ..

cd Residual
make clean
cd ..

cd ObjFuncs
make clean
cd ..

cd AdjointSolver
make clean
cd ..

cd Aldesia
make clean
rm -r build
rm -r dist
rm -r pyALDESIA_app.egg-info
rm -r pyALDESIA.cpp
cd ..

cd optFuncs
rm -r build
rm -r dist
rm -r optFuncs.egg-info
cd ..