//
//  main.cpp
//  hough
//
//  Created by Артём Lazemir on 09.11.2019.
//  Copyright © 2019 MIPT DGAP. All rights reserved.
//

#include <iostream>
#include <cmath>

template <typename T>
T min(T x, T y) {
    if (x < y) {
        return x;
    } else {
        return y;
    }
}

template <typename T>
T max(T x, T y) {
    if (x > y) {
        return x;
    } else {
        return y;
    }
}

const int
    X_min = -20,
    X_max = 20,
    Y_min = -20,
    Y_max = 20,
    N_on_line = 20,
    N_noise = 10,
    quantity_of_lines = 2,
    N = quantity_of_lines * N_on_line + N_noise,
    Fi_nums = 30,
    R_nums = 30;

const float
    Rx_max = max(abs(X_min), abs(X_max)),
    Ry_max = max(abs(Y_min), abs(Y_max)),
    R_max = pow(pow(Rx_max, 2) + pow(Ry_max, 2), 0.5);
    

int rand_int(int start, int end) {
    return start + rand() % (end - start + 1); //Возвращает случайное целое число от start до end, включая концы
};

float rand_double(double start, double end){
    return start + (end - start) * ((float) rand()) / (float) RAND_MAX;
};

double func_from_hough(double x, double r, double fi) {
    return - 1 / tan(fi) * x + r / sin(fi);
};


int fill_array_points_of_line(int (&points)[N][2], int index_of_start, double r, double fi, int x_min, int x_max, int y_min, int y_max) {
    
    int x, y;
    
    if (fi == 0) {
        x = r;
        for (int i = index_of_start; i < index_of_start + N_on_line; i++) {
            y = rand_int(y_min, y_max);
        }
    } else {
        if (y_min > func_from_hough(x_min, r, fi)) {
            y_min = func_from_hough(x_min, r, fi);
        };
        
        if (y_max < func_from_hough(x_max, r, fi)) {
            y_max = func_from_hough(x_max, r, fi);
        };
        
        for (int i = index_of_start; i < index_of_start + N_on_line; i++) {
            x = rand_int(x_min, x_max);
            y = round(func_from_hough(x, r, fi));
            points[i][0] = x;
            points[i][1] = y;
        }
    }
    return index_of_start + N_on_line;
}

int fill_array_noise(int (&points)[N][2], int index_of_start, int x_min, int x_max, int y_min, int y_max) {
    for (int i = index_of_start; i < index_of_start + N_noise; i++) {
        points[i][0] = rand_int(x_min, x_max);
        points[i][1] = rand_int(y_min, y_max);
    }
    return index_of_start + N_on_line;
}

void init_hough(int (&hough)[Fi_nums + 1][R_nums + 1]) {
    for (int i = 0; i < Fi_nums; i++) {
        for(int j = 0; j < R_nums; j++) {
            hough[i][j] = 0;
        }
    }
}

void fill_hough(int hough[Fi_nums + 1][R_nums + 1], int points[N][2]) {
    for (int i = 0; i < N; i++) {
        int x = points[i][0];
        int y = points[i][1];
        for (int fi_bin= 0; fi_bin <= Fi_nums; fi_bin++) {
            float fi = fi_bin * M_PI / Fi_nums;
            float r = x * cos(fi) + y * sin(fi);
            int r_bin = round((R_max + r) * R_nums / (2 * R_max));
            hough[fi_bin][r_bin] += 1;
        }
    }
}

void init_of_array(int (&points)[N][2]) {
    srand((unsigned) time(NULL));
    double fi, r;
    int index_of_start = 0;
    
    for (int i = 0; i < quantity_of_lines; i++) {
        fi = rand_double(0, M_PI);
        r = rand_double(max(X_min, Y_min), min(X_max, Y_max));
        index_of_start = fill_array_points_of_line(points, index_of_start, r, fi, X_min, X_max, Y_min, Y_max);
    }
    
    index_of_start = fill_array_noise(points, index_of_start, X_min, X_max, Y_min, Y_max);
}

void finding_lines(int points[N][2]) {
    int hough[Fi_nums + 1][R_nums + 1];
    init_hough(hough);
    fill_hough(hough, points);
    
   
    int schet = 0;
    for (int i = 0; i < Fi_nums; i++) {
        std::cout << '\n';
        for(int j = 0; j < R_nums; j++) {
            std::cout << hough[i][j] << ' ';
            
            if (hough[i][j]) {
                schet++;
            }
        }
    }
    
    std::cout << '\n' << schet << '\n';
    
}

int main(){
    int points[N][2];
    init_of_array(points);
    finding_lines(points);
    
    
    
    return 0;
}
