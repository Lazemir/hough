//
//  main.cpp
//  hough
//
//  Created by Артём Lazemir on 09.11.2019.
//  Copyright © 2019 MIPT DGAP. All rights reserved.
//

#include "hough.hpp"
#include "root_graph.hpp"
#include <iostream>

int main(){
    int points[2][N];
    filling_array(points);
    finding_lines(points);
    draw_graph_root(points);
}
