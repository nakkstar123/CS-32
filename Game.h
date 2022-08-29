//
//  Game.hpp
//  zion
//
//  Created by Nakul Khambhati on 6/24/22.
//

#ifndef GAME_H
#define GAME_H

class Arena;

class Game
{
  public:
        // Constructor/destructor
    Game(int rows, int cols, int nRobots);
    ~Game();

        // Mutators
    void play();

  private:
    Arena* m_arena;
};

#endif /* Game_hpp */
