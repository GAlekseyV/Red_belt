//
// Created by GAlekseyV on 02.10.2018.
//
#include "profile.h"
#include "paginator.h"

#include <vector>
#include <future>


using namespace std;

template <typename ContainerOfVectors>
void GenerateSingleThread(ContainerOfVectors& result, size_t first_row, size_t column_size){
    for(auto& row : result){
        row.reserve(column_size);
        for(size_t column = 0; column < column_size; ++column){
            row.push_back(first_row ^ column); // Побитовое XSOR, способ заполнения матрицы
        }
        ++first_row;
    }
}

vector<vector<int>> GenerateSingleThread(size_t size){
    vector<vector<int>> result(size);
    GenerateSingleThread(result, 0, size);
    return result;
}

template <typename ContainerOfVectors>
int64_t SumSingleThread(const ContainerOfVectors& matrix){
    int64_t sum = 0;
    for(const auto& row : matrix){
        for(auto item : row){
            sum += item;
        }
    }
    return sum;
}

vector<vector<int>> GenerateMultiThread(size_t size, size_t page_size){
    vector<vector<int>> result(size);
    vector<future<void>> futures;
    size_t first_row = 0;
    for(auto page : Paginate(result, page_size)){
        futures.push_back(
                async([page, first_row, size]{
                    GenerateSingleThread(page, first_row, size);
                })
                );
        first_row += page_size;
    }
    return result;
}

int main(){
    LOG_DURATION("Total");
    const size_t matrix_size = 7000;
    const size_t page_size = 2000;

    vector<vector<int>> matrix;
    {
        LOG_DURATION("Single thread generation");
        matrix = GenerateSingleThread(matrix_size);
    }
    {
        LOG_DURATION("Single thread sum");
        cout << SumSingleThread(matrix) << endl;
    }
    {
        LOG_DURATION("Multi thread generation");
        matrix = GenerateMultiThread(matrix_size, page_size);
    }
    {
        LOG_DURATION("Multi thread sum");
        cout << SumSingleThread(matrix) << endl;
    }
}
