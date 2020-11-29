#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <set>
#include <cmath>
#include <algorithm>
#include <omp.h>

using namespace std;

mutex mtx;
/**
 * Сожержит ли number * m все те же цифры что и number
 * Возвращает true, если да и false, если нет
 * @param number
 * @param m
 * @return
 */
bool Fits(int number, int m){
    long long multiplied = (long long)number * (long long)m;
    set<int> digits1;
    set<int> digits2;
    while (number > 0){
        digits1.insert(number % 10);
        number /= 10;
    }
    while (multiplied > 0){
        digits2.insert(multiplied % 10);
        multiplied /= 10;
    }
    for (int num : digits1)
        if (digits2.find(num) == digits2.end())
            return false;
    return true;
}
/**
 * Для всех чисел в промежутке [lower; upper] выполняется функция Fits
 * Все подходящие числа записываются в вектор Result, переданный по ссылке
 * @param lower
 * @param upper
 * @param n
 * @param Result
 */
void Func(int lower, int upper, int n, vector<int> &Result){

    for (int i = lower; i <= upper; ++i) {
        if(Fits(i, n)){
            #pragma omp critical
            Result.push_back(i);
        }
    }
}
/**
 * Ввод int значения из промежутку [lower; upper]
 * При неправильном вводе выведется соответствующее сообщение и запрос на ввод повторится
 * @param lower
 * @param upper
 * @return
 */
int GetInt(int lower, int upper){
    int res;
    cout << "Please input a number between " << lower << " and " << upper << "." << endl;
    while (!(cin >> res) || res < lower || res > upper){
        cout << "Wrong input, please, try again.";
        cin.clear();
        cin.sync();
    }
    return res;
}

int main() {
    int n, NumOfThreads;
    cout << "Input n (multiplier)." << endl;
    n = GetInt(2, 9);
    cout << "Input number of threads. ";
    cout << "Best number of threads for your machine: " << std::thread::hardware_concurrency() << endl;
    NumOfThreads = GetInt(1, 100);

    //Запуск таймера
    auto start = chrono::high_resolution_clock::now();
    const double max = 9999999;
    vector<int> Results;
    //Запуск Thread-ов
    //vector<thread> Threads;
    #pragma omp parallel num_threads(NumOfThreads)
    {
        int i = omp_get_thread_num();
        if (i == NumOfThreads - 1)
            Func((int)floor(max/NumOfThreads) * i, max, n, ref(Results));
        else
            Func((int)floor(max/NumOfThreads) * i,
                                     (int)ceil(max/NumOfThreads) * (i+1), n, ref(Results));
    }
    Func(1000, (int)ceil(max/NumOfThreads), n, ref(Results));
    //Сортировка вектора с ответами
    sort(Results.begin(), Results.end());

    //Вывод последних двух сотен ответов из списка
    /*vector<int> :: iterator it = Results.end();
    it--;
    for (int i = 0; i < 200; ++i) {
        cout << *it << " ";
        it--;
    }*/

    // Вывод всех ответов
    for(int temp : Results)
        cout << temp << " ";

    auto elapsed = chrono::high_resolution_clock::now() - start;
    long long nanosec = chrono::duration_cast<chrono::seconds>(elapsed).count();

    cout << endl << "Time: " << nanosec << " seconds." << endl;

    return 0;
}