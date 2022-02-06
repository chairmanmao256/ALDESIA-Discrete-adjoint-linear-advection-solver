#include <iostream>
class OpOv
{
    private:
    double*** arr = new double**[2];

    public: 
    OpOv(){
        for (int i = 0; i < 2; i++){
            arr[i] = new double*[5];
            for (int j = 0; j < 5; j++){
                arr[i][j] = new double[5];
            }
        }
        for (int i = 0; i < 2; i++){
            for (int j = 0; j < 5; j++){
                for (int k = 0; k < 5; k++){
                    arr[i][j][k] = 0.1;
                }
            }
        }
    }

    ~OpOv(){
        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 5; j++){
                delete [] arr[i][j];
            }
        }
    }
    
    double** operator[](int index){
        return arr[index];
    }
};

int main()
{
    OpOv T;
    std::cout<<T[0][1][2]<<"\n";
    T[0][1][2] = 2.0;
    std::cout<<T[0][1][2]<<"\n";
    return 0;
}