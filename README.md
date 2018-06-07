# 程式語言 HW4
## 目標
- [ ] 實作平行架構之傳統Matrix Multiplication與Strassen‘s
Matrix Multiplication 
- [ ] 改良或設計其他加速方法(亦須實作Strassen‘s Matrix
Multiplication以便與之比較)
## 執行方式
* 編譯
    ```
    make
    ```
* 執行
    ```
    make run
    ```
* 清除執行檔及 txt 檔
    ```
    make clean
    ```
* 清除 cache
    ```
    make cache-clean
    ```
* 亂數矩陣 
    * SIZE 及 FILE_NAME 分別填入矩陣大小及輸出檔名
    * default size 10
    * default name "matrix.txt"
    ```
    ./random <SIZE> <FILE_NAME>
    ```

## 實作方式
* 使用 openmp 將範圍內程式碼拆成多個 thread ，同時在多個 CPU 上執行
* 需加上 `<omp.h>` 之標頭檔
* 編譯時需加上 `-fopenmp` 之參數

## 參考資料
* [Toward concurrency](https://hackmd.io/s/Skh_AaVix)
* [Strassen's matrix multiply](https://medium.com/human-in-a-machine-world/strassen-s-algorithm-for-matrix-multiplication-8aada6cda2fd)
* [Strassen's multiply that size isn't $2^n*2^n$](https://www.eecis.udel.edu/~saunders/courses/621/03f/modelV.pdf)