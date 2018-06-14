# 程式語言 HW4
## 目標
* 實作平行架構之傳統Matrix Multiplication與Strassen‘s
Matrix Multiplication 
* 改良或設計其他加速方法(亦須實作Strassen‘s Matrix
Multiplication以便與之比較)
## 檔案說明
* serial.c : 傳統矩陣乘法，用來確認其他幾種版本結果是否正確
* parallel.c : 平行化傳統矩陣乘法
* strassen_parallel.c : 平行化 strassen's 矩陣乘法
* opt.c : 最佳化矩陣乘法
* random.c : 亂數產生指定大小及檔名的矩陣
## 執行方式
* 編譯
    ```
    make
    ```
* 執行
    ```
    make run
    ```
    * 檔名限制必須為 input.txt
    * 列出四個執行檔在「乘法運算」所花的時間
    * 四個執行檔的運算結果會分別存入相對應名稱的 txt 檔
    * 若比對結果不同會輸出相異處，若相同則不會有輸出
* 清除執行檔及 txt 檔
    ```
    make clean
    ```
* 清除 cache
    ```
    make cache-clean
    ```
* 亂數矩陣 
    ```
    ./random <SIZE> <FILE_NAME>
    ```
    * SIZE 及 FILE_NAME 分別填入矩陣大小及輸出檔名
    * default size 16
    * default name "input.txt"

## 實作功能
* 使用 openmp 將範圍內程式碼拆成多個 thread ，同時在多個 CPU 上執行，以達到平行化
* 需加上 `<omp.h>` 之標頭檔
* 編譯時需加上 `-fopenmp` 之參數
* 若輸入的矩陣尺寸非 2 的 n 次方，在跑 strassen's 前會自動補零，成為 2 的 n 次方，以符合 strassen's 的要求

## 參考資料
* [Toward concurrency](https://hackmd.io/s/Skh_AaVix)
* [Strassen's matrix multiply](https://medium.com/human-in-a-machine-world/strassen-s-algorithm-for-matrix-multiplication-8aada6cda2fd)
* [Strassen's multiply that size isn't $2^n*2^n$](https://www.eecis.udel.edu/~saunders/courses/621/03f/modelV.pdf)
* [perf 效能分析](http://wiki.csie.ncku.edu.tw/embedded/perf-tutorial)
