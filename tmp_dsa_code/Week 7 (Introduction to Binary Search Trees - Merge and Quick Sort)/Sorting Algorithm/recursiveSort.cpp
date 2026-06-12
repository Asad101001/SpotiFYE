#include <iostream>
using namespace std;

void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for(int i = 0; i < n1; i++) L[i] = arr[l + i];
    for(int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;

    while(i < n1 && j < n2)
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];

    while(i < n1) arr[k++] = L[i++];
    while(j < n2) arr[k++] = R[j++];
}

void mergeSort(int arr[], int l, int r) {
    if(l < r) {
        int m = l + (r - l)/2;
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        merge(arr, l, m, r);
    }
}


int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for(int j = low; j < high; j++)
        if(arr[j] < pivot)
            swap(arr[++i], arr[j]);

    swap(arr[i+1], arr[high]);
    return i + 1;
}

void quickSort(int arr[], int low, int high) {
    if(low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi-1);
        quickSort(arr, pi+1, high);
    }
}

int main() {
    int arr1[] = {12, 11, 13, 5, 6, 7};
    int n1 = sizeof(arr1)/sizeof(arr1[0]);

    mergeSort(arr1, 0, n1-1);

    for(int i = 0; i < n1; i++)
        cout << arr1[i] << " ";

    cout << endl;
    //

    int arr2[] = {10, 7, 8, 9, 1, 5};
    int n2 = sizeof(arr2)/sizeof(arr2[0]);

    quickSort(arr2, 0, n2-1);

    for(int i = 0; i < n2; i++)
        cout << arr2[i] << " ";
}