const int
    POL = 2,
    X_min = 0,
    X_max = 40,
    Y_min = 0,
    Y_max = 40,
    N_on_line = 10,
    N_noise = 0,
    quantity_of_lines = 2,
    N = quantity_of_lines * N_on_line + N_noise,
    Edge = 8;

const float
    Rx_max = max(abs(X_min), abs(X_max)),
    Ry_max = max(abs(Y_min), abs(Y_max)),
    R_max = pow(pow(Rx_max, 2) + pow(Ry_max, 2), 0.5);

const int
    R_nums = 100,
    Fi_nums = 5657; //int(R_max * R_nums) + 1; для данной конфигурации

void filling_array(int(&)[2][N]);
void finding_lines(int[2][N]);
