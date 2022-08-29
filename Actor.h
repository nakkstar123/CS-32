#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject{
    
private:  StudentWorld* m_world;
    
public:
    Actor(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0, StudentWorld* world_ptr = nullptr) : GraphObject(imageID, startX, startY, dir, size, depth) {
        
        m_world = world_ptr;
        setVisible(true);
        
    }
    virtual ~Actor(){}
    
    virtual bool isAlive() = 0;
    virtual void doSomething() = 0;
    virtual void getAnnoyed(int amt, int points) = 0;
    virtual void setDead() = 0;
    
    virtual StudentWorld* getWorld(){return m_world;}
};

class TunnelMan : public Actor{
private:
    
    int m_hp;
    int m_nSquirts;
    int m_nSonar;
    int m_nGold;
    int m_nOil;
    
    
public:
    
    TunnelMan(StudentWorld* world_ptr): Actor(TID_PLAYER, 30, 60, right, 1.0, 0, world_ptr)
    {
        m_hp = 10;
        m_nSquirts = 5;
        m_nSonar = 1;
        m_nGold = 0;
        m_nOil = 0;
        setVisible(true);
    };
    
    ~TunnelMan(){}
    
    virtual void setDead(){m_hp = 0;}
    
    virtual bool isAlive(){
        return (m_hp > 0);
    }
    virtual void doSomething();
    virtual void getAnnoyed(int amt, int points){m_hp -= amt;}
    
    void tryMove(Direction dir);
    
    void incSonar(){m_nSonar++;}
    void incOil(){m_nOil++;}
    void incGold(){m_nGold++;}
    void incSquirts(){m_nSquirts++;}
    
    void decHealth(){m_hp--;}
    
    
    int getOil() const {return m_nOil;} // this seems illegal, need to probably find an alt
    int getHealth() const {return m_hp;}
    int getSonar() const {return m_nSonar;}
    int getGold() const {return m_nGold;}
    int getSquirts() const {return m_nSquirts;}
    
    
};

class Earth : public Actor{
private:
public:
    
    Earth(int x, int y) : Actor(TID_EARTH, x, y, right, 0.25, 3){
        setVisible(true);
    }
    ~Earth(){}
    virtual bool isAlive(){return true;}
    virtual void doSomething(){return;}
    virtual void getAnnoyed(int amt, int points){return;}
    
    virtual void setDead(){return;} // can prolly do something smarter
    
};

class StudentWorld;

class Boulder : public Actor{
private:
    //                 0       1        2       3
    int m_state; // stable, waiting, falling, dead
    int m_wait;
    
public:
    
    Boulder(int x, int y, StudentWorld* world_ptr) : Actor(TID_BOULDER, x, y, down, 1.0, 1, world_ptr){
        setVisible(true);
        m_state = 0; // stable
        
        m_wait = 30;
    }
    
    ~Boulder(){}
    
    virtual void setDead(){m_state = 3;}
    
    virtual bool isAlive();
    virtual void doSomething();
    virtual void getAnnoyed(int amt, int points){return;}
    
};

class Squirt : public Actor{
private:
    
    int m_dist;
    
public:
    Squirt(int x, int y, Direction dir, StudentWorld* world_ptr) : Actor(TID_WATER_SPURT, x, y, dir, 1.0, 1, world_ptr) {
        m_dist = 4;
        setVisible(true);
    }
    
    virtual bool isAlive();
    virtual void doSomething();
    virtual void getAnnoyed(int amt, int points){return;}
    virtual void setDead(){m_dist = 0;}
    
    
    bool canMove(); // helper function that determines whether the squirt can continue moving in the direction it currently faces
};

class Barrel : public Actor{
private:
    bool m_alive;
public:
    Barrel(int x, int y, StudentWorld* world_ptr) : Actor(TID_BARREL, x, y, right, 1.0, 2, world_ptr){
        setVisible(false);
        m_alive = true;
    }
    
    bool nearTunnelMan(double dist);
    
    virtual bool isAlive(){return m_alive;}
    virtual void doSomething();
    virtual void getAnnoyed(int amt, int points){return;}
    virtual void setDead(){m_alive = false;}
    
};

class GoldNugget : public Actor{
private:
    bool m_alive;
    bool m_perm;
    bool m_pick;
    
    int m_wait;
public:
    GoldNugget(int x, int y, bool visibility, bool perm, bool pick, StudentWorld* world_ptr) : Actor(TID_GOLD, x, y, right, 1.0, 2, world_ptr){
        
        setVisible(visibility); // Nuggets buried within the Earth of the oil field always start out invisible, whereas Nuggets dropped by the TunnelMan start out visible.
        
        m_alive = true;
        m_perm = perm;
        m_pick = pick;
        
        m_wait = 100;// change this??
        
    }
    
    bool nearTunnelMan(double dist); // reuse goodies stuff?? 
    
    virtual bool isAlive(){return m_alive;}
    virtual void doSomething();
    virtual void getAnnoyed(int amt, int points){return;}
    virtual void setDead(){m_alive = false;}
    
};

class Sonar : public Actor{
private:
    bool m_alive;
    int m_wait;
public:
    Sonar(int x, int y, StudentWorld* world_ptr);
    
    bool nearTunnelMan(double dist); // reuse goodies stuff??
    
    virtual bool isAlive(){return m_alive;}
    virtual void doSomething();
    virtual void getAnnoyed(int amt, int points){return;}
    virtual void setDead(){m_alive = false;}
    
};

class Water : public Actor{
private:
    bool m_alive;
    int m_wait;
public:
    Water(int x, int y, StudentWorld* world_ptr);
    
    bool nearTunnelMan(double dist); // reuse goodies stuff??
    
    virtual bool isAlive(){return m_alive;}
    virtual void doSomething();
    virtual void getAnnoyed(int amt, int points){return;}
    virtual void setDead(){m_alive = false;}
    
};

class Protestor : public Actor{
private:
    bool m_alive;
    int m_numSquaresToMove;
    int m_hp;
    bool leave;
    int m_wait;
    int max_wait; 
    
    int last_shout;
    bool just_shouted;
    
    int m_perp;
    
    bool isHardcore;
    
    bool made_exit_stack;
    std::stack<Direction> m_exit_stack;
    
public:
    Protestor(int numSquaresToMove, StudentWorld* world_ptr, int TID);
    
    bool nearTunnelMan(double dist);
    bool isFacingTunnelMan(); // checks if the direction corresponds to where the tunnelman is wrt protestor
    bool canReachTunnelMan(); // needed in the doSomething to check for line of sight
    void moveInCurrentDir();
    void moveInDir(Actor::Direction dir);
    
    void setWait(int wait){m_wait = wait;}
    
    bool canMoveInDir(Direction dir); // checks if one step in a given direction has earth, boulders or borders
    bool canMoveToLoc(int x, int y); // same as above but specifies loc instead of dir
    
    Direction pickRandomDir();
    
    virtual void setDead();
    
    bool currDirIsBlocked();
    
    bool isAtIntersection(Direction& dir); // checks for availability of perpendicular directions and if they are, randomly selects one and assigns it to dir, also returns true
    
    virtual bool isAlive(){return m_alive;}
    virtual void doSomething();
    
    virtual void getAnnoyed(int amt, int points);
    
    std::string pointToPath(int x, int y); // takes in a point (usually exit) and returns path from current position to that
    
    std::stack<Direction> createExitStack(std::string path); // converts a string of characters U, D, L, R into a stack of Directions
    
};

class HardcoreProtestor : public Protestor{
    
public:
    HardcoreProtestor(int numSquaresToMove, StudentWorld* world_ptr) : Protestor(numSquaresToMove, world_ptr, TID_HARD_CORE_PROTESTER) {}
    
    virtual void setDead();
    
};

#endif // ACTOR_H_
