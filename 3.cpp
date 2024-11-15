
#include <bits/stdc++.h>

#define MAX 4
#define NUM_THREADS 4

using namespace std;

int A[MAX][MAX];
int B[MAX][MAX];
int C[MAX][MAX];

struct ThreadData {
    int row;
    int col;
};

char operation;

void* add(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    C[data->row][data->col] = A[data->row][data->col] + B[data->row][data->col];
    pthread_exit(nullptr);
}

void* subtract(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    C[data->row][data->col] = A[data->row][data->col] - B[data->row][data->col];
    pthread_exit(nullptr);
}

void* multiply(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int sum = 0;
    for (int i = 0; i < MAX; i++) {
        sum += A[data->row][i] * B[i][data->col];
    }
    C[data->row][data->col] = sum;
    pthread_exit(nullptr);
}

void* divide(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    if (B[data->row][data->col] != 0) {
        C[data->row][data->col] = A[data->row][data->col] / B[data->row][data->col];
    } else {
        C[data->row][data->col] = 0; // Handle division by zero
    }
    pthread_exit(nullptr);
}

int main() {
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    int thread_count = 0;

    cout << "Enter elements of matrix A (4x4):" << endl;
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            cin >> A[i][j];
        }
    }

    cout << "Enter elements of matrix B (4x4):" << endl;
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            cin >> B[i][j];
        }
    }

    cout << "Enter the operation to perform (+, -, *, /): ";
    cin >> operation;

    void* (*operation_function)(void*);

    switch (operation) {
        case '+':
            operation_function = add;
            break;
        case '-':
            operation_function = subtract;
            break;
        case '*':
            operation_function = multiply;
            break;
        case '/':
            operation_function = divide;
            break;
        default:
            cout << "Invalid operation." << endl;
            return 1;
    }

    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            thread_data[thread_count].row = i;
            thread_data[thread_count].col = j;

            pthread_create(&threads[thread_count], nullptr, operation_function, (void*)&thread_data[thread_count]);

            thread_count++;

            if (thread_count == NUM_THREADS) {
                for (int t = 0; t < NUM_THREADS; t++) {
                    pthread_join(threads[t], nullptr);
                }
                thread_count = 0;
            }
        }
    }

    for (int t = 0; t < thread_count; t++) {
        pthread_join(threads[t], nullptr);
    }

    cout << "Result Matrix C:" << endl;
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            cout << C[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
