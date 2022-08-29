//
//  Arena.hpp
//  zion
//
//  Created by Nakul Khambhati on 6/24/22.
//

#ifndef ARENA_H
#define ARENA_H

#include<string>

#include "globals.h"
#include "Previous.h"

class Robot;
class Player;
class Previous;

class Arena
{
  public:
        // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();

        // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     robotCount() const;
    int     nRobotsAt(int, int) const;
    void    display(std::string) const;
    Previous& thePrevious();

        // Mutators
    bool   addRobot(int, int);
    bool   addPlayer(int, int);
    void   damageRobotAt(int, int);
    bool   moveRobots();

  private:
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Robot*  m_robots[MAXROBOTS];
    int     m_nRobots;
    Previous m_previous; // confused why this gives an error?
};

#endif /* Arena_hpp */
