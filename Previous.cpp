//
//  Previous.cpp
//  zion
//
//  Created by Nakul Khambhati on 6/26/22.
//

#include "Previous.h"
#include <iostream>


Previous::Previous(int nRows, int nCols){ // when called by arena, arena's private members should be passed into this
    m_rows = nRows;
    m_cols = nCols;
    int r, c;

        // Fill the grid with zeros
    for (r = 0; r < m_rows; r++)
        for (c = 0; c < m_cols; c++)
            m_grid[r][c] = 0;
}

bool Previous::dropACrumb(int r, int c){
    if (r < 1 || r > m_rows || c < 1 || c > m_cols)
    {
        return false;
    }
    
    if(m_grid[r-1][c-1] < 26) m_grid[r-1][c-1]++;
    
    return true;
}

void Previous::showPreviousMoves() const{
    clearScreen();
    int r,c;
    for (r = 0; r < m_rows; r++)
    {
        for (c = 0; c < m_cols; c++)
            if (m_grid[r][c] == 0) std::cout << '.';
            else std::cout << char(m_grid[r][c] - 1 + 'A');
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
