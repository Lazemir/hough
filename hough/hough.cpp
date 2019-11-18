//#include <iostream>
//#include <cmath>
//#include "hough.hpp"

//template <typename T>
//T min(T x, T y) {
//    if (x < y) {
//        return x;
//    } else {
//        return y;
//    }
//}
//
//template <typename T>
//T max(T x, T y) {
//    if (x > y) {
//        return x;
//    } else {
//        return y;
//    }
//}

#include <vector>
#include <limits>

int rand_int(int start, int end) {
    return start + rand() % (end - start + 1); //Возвращает случайное целое число от start до end, включая концы
};

float rand_double(double start, double end){
    return start + (end - start) * ((float) rand()) / (float) RAND_MAX;
};

double func_from_hough(double x, double r, double fi) {
    return - 1 / tan(fi) * x + r / sin(fi);
};


int fill_array_points_of_line(int (&points)[2][N], int index_of_start, double r, double fi, int x_min, int x_max, int y_min, int y_max) {

    int x, y;

    if (fi == 0) {
        x = r;
        for (int i = index_of_start; i < index_of_start + N_on_line; i++) {
            y = rand_int(y_min, y_max);
        }
    } else {
        double k = - 1 / tan(fi);
        int b = round(r / sin(fi));
        int x, y;
        
        for (int i = index_of_start; i < index_of_start + N_on_line; i++) {
            do {
                x = rand_int(X_min, X_max);
                y = round(k * x + b);
//                std::cout << x << ' ' << y << std::endl;
            } while (y < 0);
            
            points[0][i] = x;
            points[1][i] = y;
        }
    }
    return index_of_start + N_on_line;
}

int fill_array_noise(int (&points)[2][N], int index_of_start, int x_min, int x_max, int y_min, int y_max) {
    for (int i = index_of_start; i < index_of_start + N_noise; i++) {
        points[0][i] = rand_int(x_min, x_max);
        points[1][i] = rand_int(y_min, y_max);
    }
    return index_of_start + N_on_line;
}

void init_hough(int (&hough)[Fi_nums][R_nums + 1]) {
    for (int i = 0; i < Fi_nums; i++) {
        for(int j = 0; j < R_nums; j++) {
            hough[i][j] = 0;
        }
    }
}

void filling_array(int (&points)[2][N]) {
    srand((unsigned) time(NULL));
    double fi, r;
    int index_of_start = 0;
    
    for (int i = 0; i < quantity_of_lines; i++) {
        fi = rand_double(0, M_PI);
        r = rand_double(0, 20);
        std::cout << fi << ' ' << r <<std::endl;
        index_of_start = fill_array_points_of_line(points, index_of_start, r, fi, X_min, X_max, Y_min, Y_max);
    }
    index_of_start = fill_array_noise(points, index_of_start, X_min, X_max, Y_min, Y_max);
    
    std::cout << std::endl;
}

void fill_hough(int hough[Fi_nums][R_nums + 1], int points[2][N]) {
    for (int i = 0; i < N; i++) {
        int x = points[0][i];
        int y = points[1][i];
        for (int fi_bin= 0; fi_bin < Fi_nums; fi_bin++) {
            float fi = fi_bin * 2*M_PI / Fi_nums;
            float r = x * cos(fi) + y * sin(fi);
            if (r >= 0) {
                int r_bin = round(r * R_nums / R_max);
                hough[fi_bin][r_bin] += 1;
            }
        }
    }
}

int fiding_figures_in_hough(int (&hough)[Fi_nums][R_nums + 1]) {
    int num_of_figures = 0;
    int fig = 0;
    for (int fi_bin = 0; fi_bin < Fi_nums; fi_bin++) {
        for(int r_bin = 0; r_bin < R_nums; r_bin++) {
            if (hough[fi_bin][r_bin] >= Edge) {
                if (hough[fi_bin - 1][r_bin - 1]) {
                    fig = hough[fi_bin - 1][r_bin - 1];
                } else if (hough[fi_bin - 1][r_bin]) {
                    fig = hough[fi_bin - 1][r_bin];
                } else if (hough[fi_bin - 1][r_bin + 1]) {
                    fig = hough[fi_bin - 1][r_bin + 1];
                } else if (hough[fi_bin][r_bin - 1]) {
                    fig = hough[fi_bin][r_bin - 1];
                } else { //если три слева и один сверху нули
                    num_of_figures++; //увеличиваем счетчик фигур
                    fig = num_of_figures; //новая фигура
                }
            } else {
                fig = 0;
            }
            hough[fi_bin][r_bin] = fig;
        }
    }
    return num_of_figures;
}

struct Point {
    int fi, r, n;
};

std::vector <Point> fiding_center_of_figures(int (&hough)[Fi_nums][R_nums + 1], int num_of_figures) {
    std::vector <Point> figures(num_of_figures + 1);
    
    for (int fi_bin = 0; fi_bin < Fi_nums; fi_bin++) {
        for(int r_bin = 0; r_bin < R_nums; r_bin++) {
            if (hough[fi_bin][r_bin]) {
                figures[hough[fi_bin][r_bin]].fi += fi_bin;
                figures[hough[fi_bin][r_bin]].r += r_bin;
                figures[hough[fi_bin][r_bin]].n++;
            }
        }
    }
    
    for (int i = 1; i <= num_of_figures; i++) {
        figures[i].fi /= figures[i].n;
        figures[i].r /= figures[i].n;
    }
    
    return figures;
}

void finding_lines(int points[2][N]) {
    int hough[Fi_nums][R_nums + 1];
    init_hough(hough);
    fill_hough(hough, points);
    int num_of_figures = fiding_figures_in_hough(hough);
    
    std::vector <Point> figures(num_of_figures + 1);
    figures = fiding_center_of_figures(hough, num_of_figures);
    
    for (int i = 1; i <= num_of_figures; i++) {
        std::cout << figures[i].fi * 2*M_PI / Fi_nums << ' ' << figures[i].r * R_max / R_nums << std::endl;
    }
}

bool check_point_on_line(int x, int y, float k, int b) {
    return y == k * x + b;
}
