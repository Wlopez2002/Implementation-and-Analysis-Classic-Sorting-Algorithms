/*
William Lopez
12/6/2023

CS-361L
University Of New Mexico

CS361 Programming Project
*/

#include <iostream>
#include <chrono>
#include <string>

using namespace std;

static int *veryLargeArray;
static int *tempArray;
static double *veryLargeDoubleArray;
static double *tempDoubleArray;


template <typename T>
int checkSorted(T arr[], int s){
    for(int i = 1; i < s; i++){
        if (arr[i-1] > arr[i]){
            return 0;
        }
    }
    return 1;
}

template <typename T>
void copyArray(T *a, T *b, int s){
    for(int i = 0; i < s; i++){
        b[i] = a[i];
    }
}

template <typename T>
void swapValuesAt(T a[], int indexOne, int indexTwo){
    T temp = a[indexOne];
    a[indexOne] = a[indexTwo];
    a[indexTwo] = temp;
}

template <typename T>
void printArray(T a[], int s){
    if (s == 0){
        cout << "Array = {}" << endl;
        return;
    }
    cout << "Array = {";
    for (int i = 0; i < s-1; i++){
        cout << a[i] << ", ";
    }
    cout << a[s-1] << "}" << endl;
    return;
}

//Iterative
template <typename T>
void quadMaxHeapify(T a[], int s, unsigned int currentIndex){
    while(true){
        unsigned int largest = currentIndex;
        unsigned int first = 4*currentIndex + 1;
        unsigned int second = 4*currentIndex + 2;
        unsigned int third = 4*currentIndex + 3;
        unsigned int fourth = 4*currentIndex + 4;

        // Stack overflow here for large values
        if (first < s && a[first] > a[largest]){
            largest = first;
        }
        if (second < s && a[second] > a[largest]){
            largest = second;
        };
        if (third < s && a[third] > a[largest]){
            largest = third;
        }
        if (fourth < s && a[fourth] > a[largest]){
            largest = fourth;
        };
        if (largest != currentIndex){
            swapValuesAt(a,largest,currentIndex);
            currentIndex = largest;
        } else {
            break;
        }
    }
}

template <typename T>
void quadHeapSort(T a[], int s){
    // Start by building the initial max-heap
    for(int i = s; i >= 0; i--){
        quadMaxHeapify(a,s,i);
    }
    // A quad heap can be indexed with (4i + (1 to 4))
    while (s != 0){
        // Put largest value at the end
        swapValuesAt(a,0,s-1);
        // iterative call
        s = s - 1;
        quadMaxHeapify(a,s,0);
    }
    return;
}

// Checks if a array should be picked from.
template <typename T>
bool mergeHelper(T a[], T b[], int sa, int sb){
    // there is nothing in a.
    if (sa == 0 && sb != 0){
        return false;
    }
    // there is nothing in b.
    if (sa != 0 && sb == 0){
        return true;
    }
    // nothing in either
        if (sa == 0 && sb == 0){
        return false;
    }

    // true is a[0] is smaller than b[0]
    if (a[0] > b[0]){
        return false;
    }
    return true;
}

template <typename T>
void mergeStep(T storage[], T a[], T b[], T c[], int sa, int sb, int sc){
    // if the sum of sizes is equal to zero we have run out of values to take.
    for (int i = 0; sa + sb + sc > 0; i++){
        // Next value in a is smallest
        if (mergeHelper(a, b, sa, sb) && mergeHelper(a, c, sa, sc)){
            storage[i] = a[0];
            a++;
            sa--;
            continue;
        }
        // Next value in b is smallest
        if (mergeHelper(b, a, sb, sa) && mergeHelper(b, c, sb, sc)){
            storage[i] = b[0];
            b++;
            sb--;
            continue;
        }
        // Take from c if there are no other options
        if (sc > 0){

            storage[i] = c[0];
            c++;
            sc--;
            continue;
        }
    }
    return;
}

//Inductive Version
template <typename T>
void threeWayMergeSort(T a[], int s){
    //The sort breaks unless the merge is in the heap.
    T *mergeTemp = new T[s];
    for (int currentSize = 1; currentSize < s; currentSize = 3*currentSize){
        for (int start = 0; start < s; start += 3*currentSize){
            int copySize = currentSize * 3;
            int startOne = start;
            int startTwo = start + currentSize;
            int startThree = start + currentSize + currentSize;

            int sizeOne = currentSize;
            int sizeTwo = currentSize;
            int sizeThree = currentSize;

            if (copySize + start > s){
                copySize = s - start;
            }

            if (copySize == 1){
                sizeTwo = 0;
                sizeThree = 0;
                startTwo = 0;
                startThree = 0;
            }
            if (copySize == 2){
                sizeThree = 0;
                startThree = 0;
            }

            if (startOne + sizeOne > s){
                sizeOne = max(s - startOne,0);
            }
            if (startTwo + sizeTwo > s){
                sizeTwo = max(s - startTwo,0);
            }
            if (startThree + sizeThree > s){
                sizeThree = max(s - startThree,0);
            }

            mergeStep(mergeTemp, a+startOne, a+startTwo, a+startThree, sizeOne, sizeTwo, sizeThree);
            copyArray(mergeTemp, a+start, copySize);
        }
    }
    delete [] mergeTemp;
}

template <typename T>
void randomQuickSort(T a[], int startIndex, int endIndex){
    int pivotIndex;
    int j = startIndex; int i = startIndex-1;

    // The base case.
    if (startIndex >= endIndex){
        return;
    }

    pivotIndex = rand() % (endIndex - startIndex + 1) + startIndex;

    swapValuesAt(a, pivotIndex, endIndex); // places the pivot at the end;

    // The code below moves all values less than the pivot
    // to the left if it. Greater values are moved to the
    // right of it.
    while (j < endIndex){
        if (a[j] < a[endIndex]){
            i++;
            swapValuesAt(a, i, j);
        }
        j++;
    }
    i++;
    swapValuesAt(a, i, endIndex);

    //Now we repeat the process for elements to the
    //right and left of the pivot.
    randomQuickSort(a, startIndex, i-1);
    randomQuickSort(a, i+1, endIndex);
    return;
}

// This function tests every algorithm for correctness and runtime.
void testAll(int arr[], int s, int verbose){
    tempArray = new int[s];
    auto start = chrono::high_resolution_clock::now();
    auto finish = chrono::high_resolution_clock::now();
    double result;

    //QuickSort
    copyArray(arr, tempArray, s);
    cout << "running random quicksort..." << endl;

    start = chrono::high_resolution_clock::now();
    randomQuickSort(tempArray, 0, s-1);
    finish = chrono::high_resolution_clock::now();
    result = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
    result *= 1e-6;

    if (checkSorted(tempArray,s)){
        cout << "sort passed." << endl;
    } else {
        cout << "sort failed." << endl;
    }
    cout << fixed << result;
    cout << " milliseconds." << endl;
    if (verbose == 1){
        cout << "Printing sorted array." << endl;
        printArray(tempArray,s);
    }
    cout << endl;

    //Mergesort
    copyArray(arr, tempArray, s);
    cout << "running 3-way mergesort..." << endl;

    start = chrono::high_resolution_clock::now();
    threeWayMergeSort(tempArray, s);
    finish = chrono::high_resolution_clock::now();
    result = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
    result *= 1e-6;

    if (checkSorted(tempArray,s)){
        cout << "sort passed." << endl;
    } else {
        cout << "sort failed." << endl;
    }
    cout << fixed << result;
    cout << " milliseconds." << endl;
    if (verbose == 1){
        cout << "Printing sorted array." << endl;
        printArray(tempArray,s);
    }
    cout << endl;

    //Heapsort
    copyArray(arr, tempArray, s);
    cout << "running quad heapsort..." << endl;

    start = chrono::high_resolution_clock::now();
    quadHeapSort(tempArray, s);
    finish = chrono::high_resolution_clock::now();
    result = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
    result *= 1e-6;

    if (checkSorted(tempArray,s)){
        cout << "sort passed." << endl;
    } else {
        cout << "sort failed." << endl;
    }
    cout << fixed << result;
    cout << " milliseconds." << endl;
    if (verbose == 1){
        cout << "Printing sorted array." << endl;
        printArray(tempArray,s);
    }
    cout << endl;
}
void testAll(double arr[], int s, int verbose){
    tempDoubleArray = new double[s];
    auto start = chrono::high_resolution_clock::now();
    auto finish = chrono::high_resolution_clock::now();
    double result;

    //QuickSort
    copyArray(arr, tempDoubleArray, s);
    cout << "running random quicksort..." << endl;

    start = chrono::high_resolution_clock::now();
    randomQuickSort(tempDoubleArray, 0, s-1);
    finish = chrono::high_resolution_clock::now();
    result = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
    result *= 1e-6;

    if (checkSorted(tempDoubleArray,s)){
        cout << "sort passed." << endl;
    } else {
        cout << "sort failed." << endl;
    }
    cout << fixed << result;
    cout << " milliseconds." << endl;
    if (verbose == 1){
        cout << "Printing sorted array." << endl;
        printArray(tempDoubleArray,s);
    }
    cout << endl;

    //Mergesort
    copyArray(arr, tempDoubleArray, s);
    cout << "running 3-way mergesort..." << endl;

    start = chrono::high_resolution_clock::now();
    threeWayMergeSort(tempDoubleArray, s);
    finish = chrono::high_resolution_clock::now();
    result = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
    result *= 1e-6;

    if (checkSorted(tempDoubleArray,s)){
        cout << "sort passed." << endl;
    } else {
        cout << "sort failed." << endl;
    }
    cout << fixed << result;
    cout << " milliseconds." << endl;
    if (verbose == 1){
        cout << "Printing sorted array." << endl;
        printArray(tempDoubleArray,s);
    }
    cout << endl;

    //Heapsort
    copyArray(arr, tempDoubleArray, s);
    cout << "running quad heapsort..." << endl;

    start = chrono::high_resolution_clock::now();
    quadHeapSort(tempDoubleArray, s);
    finish = chrono::high_resolution_clock::now();
    result = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
    result *= 1e-6;

    if (checkSorted(tempDoubleArray,s)){
        cout << "sort passed." << endl;
    } else {
        cout << "sort failed." << endl;
    }
    cout << fixed << result;
    cout << " milliseconds." << endl;
    if (verbose == 1){
        cout << "Printing sorted array." << endl;
        printArray(tempDoubleArray,s);
    }
    cout << endl;
}

void fillWithRandomDoubles(double arr[], int s){
    for(int i = 0; i < s; i++){
        double decimal = (double)rand() /10000;
        arr[i] = ((double)rand() + decimal);
    }
}

void fillWithDuplicates(int arr[], int s){
    for(int i = 0; i < s; i++){
        arr[i] = (rand() % 100);
    }
}

void fillWithRandomIntegers(int arr[], int s){
    for(int i = 0; i < s; i++){
        arr[i] = rand();
    }
}

void fillWithReverseSorted(int arr[], int s){
    for(int i = 0; i < s; i++){
        arr[i] = s-i;
    }
}

int main()
{
    int arraySize = 0;
    char toTest = 'i';
    int verbose = 0;
    srand(time(NULL));

    cout << "Input a character for array type:" << endl;
    cout << "p = array of random double precision floating point numbers." << endl;
    cout << "d = random integer array with more possible duplicates." << endl;
    cout << "i = array of random integer values." << endl;
    cout << "r = reverse sorted array." << endl;
    cout << "c = custom array from user input." << endl;
    cin >> toTest;
    cout << "Enter a number for the array's size." << endl;
    cin >> arraySize;
    if (toTest != 'c'){
        cout << "Do you want to print the sorted array. (Yes = 1 | No = 0)" << endl;
        cin >> verbose;
        cout << endl;
        cout << "Starting... this make take a few seconds depending on input size." << endl;
    }
    switch (toTest){
        default:
            veryLargeArray = new int[arraySize];
            fillWithRandomIntegers(veryLargeArray,arraySize);
            testAll(veryLargeArray,arraySize,verbose);
        case 'p':
            veryLargeDoubleArray = new double[arraySize];
            fillWithRandomDoubles(veryLargeDoubleArray,arraySize);
            testAll(veryLargeDoubleArray,arraySize,verbose);
            break;
        case 'd':
            veryLargeArray = new int[arraySize];
            fillWithDuplicates(veryLargeArray,arraySize);
            testAll(veryLargeArray,arraySize,verbose);
            break;
        case 'i':
            veryLargeArray = new int[arraySize];
            fillWithRandomIntegers(veryLargeArray,arraySize);
            testAll(veryLargeArray,arraySize,verbose);
            break;
        case 'r':
            veryLargeArray = new int[arraySize];
            fillWithReverseSorted(veryLargeArray,arraySize);
            testAll(veryLargeArray,arraySize,verbose);
            break;
        case 'c':
            cout << "Will the values be doubles or integers?. (double = 1 | integer = 0)" << endl;
            int typeChoice = 0;
            cin >> typeChoice;
            cout << "Enter " << arraySize << " numbers to sort. (one per line)" << endl;
            if(typeChoice == 1){
                veryLargeDoubleArray = new double[arraySize];
                for (int i = 0; i < arraySize; i++){
                    double nextNum;
                    cin >> nextNum;
                    veryLargeDoubleArray[i] = nextNum;
                }

                cout << "Do you want to print the sorted array. (Yes = 1 | No = 0)" << endl;
                cin >> verbose;
                cout << endl;
                cout << "Starting... this make take a few seconds depending on input size." << endl;
                testAll(veryLargeDoubleArray,arraySize,verbose);
                break;
            } else {
                veryLargeArray = new int[arraySize];
                for (int i = 0; i < arraySize; i++){
                    int nextNum;
                    cin >> nextNum;
                    veryLargeArray[i] = nextNum;
                }

                cout << "Do you want to print the sorted array. (Yes = 1 | No = 0)" << endl;
                cin >> verbose;
                cout << endl;
                cout << "Starting... this make take a few seconds depending on input size." << endl;
                testAll(veryLargeArray,arraySize,verbose);
                break;
            }
            break;
    }
    cout << "Done. Enter any character to stop the program." << endl;
    cin >> toTest; // Pauses the program so user can read info.
    return 0;
}
