//
//  root_graph.cpp
//  hough
//
//  Created by Артём Lazemir on 15.11.2019.
//  Copyright © 2019 MIPT DGAP. All rights reserved.
//

//#include "hough.hpp"
//#include "root_graph.hpp"
//#include <iostream>

void draw_graph_root(int points[2][N]) {
    TGraph *gr1 = new TGraph(N, points[0], points[1]);
    TCanvas *c1 = new TCanvas ("c1","Graph Draw Options",200,10,600,400);
    gr1->SetMarkerStyle(21);
    gr1->Draw("AP");
}
