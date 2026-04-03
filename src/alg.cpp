// Copyright 2024 <Student Name>

#include <iostream>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <cmath>

using std::cout;
using std::vector;
using std::ofstream;
using std::sort;
using std::chrono::high_resolution_clock;
using std::chrono::duration;

int countPairs1(int *arr, int len, int value) {
    int count = 0;
    for (int i = 0; i < len; ++i) {
        for (int j = i + 1; j < len; ++j) {
            if (arr[i] + arr[j] == value) {
                ++count;
            }
        }
    }
    return count;
}

int countPairs2(int *arr, int len, int value) {
    int count = 0;
    int left = 0;
    int right = len - 1;
    
    while (left < right) {
        int sum = arr[left] + arr[right];
        if (sum == value) {
            if (arr[left] == arr[right]) {
                int n = right - left + 1;
                count += n * (n - 1) / 2;
                break;
            } else {
                int leftVal = arr[left];
                int rightVal = arr[right];
                int leftCount = 0;
                int rightCount = 0;
                
                while (left < len && arr[left] == leftVal) {
                    ++leftCount;
                    ++left;
                }
                while (right >= 0 && arr[right] == rightVal) {
                    ++rightCount;
                    --right;
                }
                count += leftCount * rightCount;
            }
        } else if (sum < value) {
            ++left;
        } else {
            --right;
        }
    }
    return count;
}

int countPairs3(int *arr, int len, int value) {
    int count = 0;
    
    for (int i = 0; i < len - 1; ++i) {
        int target = value - arr[i];
        int left = i + 1;
        int right = len - 1;
        int found = -1;
        
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (arr[mid] == target) {
                found = mid;
                break;
            } else if (arr[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        if (found != -1) {
            int first = found;
            int last = found;
            
            int l = i + 1;
            int r = found - 1;
            while (l <= r) {
                int m = l + (r - l) / 2;
                if (arr[m] == target) {
                    first = m;
                    r = m - 1;
                } else if (arr[m] < target) {
                    l = m + 1;
                } else {
                    r = m - 1;
                }
            }
            
            l = found + 1;
            r = len - 1;
            while (l <= r) {
                int m = l + (r - l) / 2;
                if (arr[m] == target) {
                    last = m;
                    l = m + 1;
                } else if (arr[m] < target) {
                    l = m + 1;
                } else {
                    r = m - 1;
                }
            }
            
            count += (last - first + 1);
            i = last;
        }
    }
    return count;
}

vector<int> generateSortedArray(int size) {
    vector<int> arr(size);
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % (size * 2);
    }
    sort(arr.begin(), arr.end());
    return arr;
}

double measureTime(int (*func)(int*, int, int), int* arr, int len,
                   int value) {
    auto start = high_resolution_clock::now();
    func(arr, len, value);
    auto end = high_resolution_clock::now();
    duration<double> elapsed = end - start;
    return elapsed.count();
}

int main() {
    srand(42);
    
    vector<int> sizes;
    for (int i = 100; i <= 2000; i += 100) {
        sizes.push_back(i);
    }
    
    ofstream dataFile("timing_data.txt");
    dataFile << "size\ttime1\ttime2\ttime3\n";
    
    cout << "Running benchmarks...\n";
    
    for (int size : sizes) {
        vector<int> arr = generateSortedArray(size);
        int value = arr[size/2] + arr[size/3];
        
        double t1 = measureTime(countPairs1, arr.data(), size, value);
        double t2 = measureTime(countPairs2, arr.data(), size, value);
        double t3 = measureTime(countPairs3, arr.data(), size, value);
        
        dataFile << size << "\t" << t1 << "\t" << t2 << "\t" << t3 << "\n";
        
        cout << "Size: " << size << " - t1: " << t1 << "s, t2: " << t2
             << "s, t3: " << t3 << "s\n";
    }
    
    dataFile.close();
    
    ofstream gnuplotScript("plot.gnu");
    gnuplotScript << "set terminal png size 1024,768\n";
    gnuplotScript << "set output 'Plots/lin.png'\n";
    gnuplotScript << "set title 'Time Complexity Comparison ";
    gnuplotScript << "(Linear Scale)'\n";
    gnuplotScript << "set xlabel 'Array Size'\n";
    gnuplotScript << "set ylabel 'Time (seconds)'\n";
    gnuplotScript << "set grid\n";
    gnuplotScript << "plot 'timing_data.txt' using 1:2 with linespoints ";
    gnuplotScript << "title 'countPairs1 (O(n^2))', \\\n";
    gnuplotScript << "     'timing_data.txt' using 1:3 with linespoints ";
    gnuplotScript << "title 'countPairs2 (O(n))', \\\n";
    gnuplotScript << "     'timing_data.txt' using 1:4 with linespoints ";
    gnuplotScript << "title 'countPairs3 (O(n log n))'\n";
    
    gnuplotScript << "set output 'Plots/log.png'\n";
    gnuplotScript << "set title 'Time Complexity Comparison ";
    gnuplotScript << "(Log-Log Scale)'\n";
    gnuplotScript << "set logscale xy\n";
    gnuplotScript << "replot\n";
    gnuplotScript.close();
    
    if (system("gnuplot plot.gnu") == -1) {
        cout << "Warning: Could not run gnuplot\n";
    }
    
    cout << "\nPlots saved to Plots/lin.png and Plots/log.png\n";
    
    return 0;
}
