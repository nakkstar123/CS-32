#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <cstdlib>
#include <vector>
#include <time.h>
#include <cmath>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
        m_protestor_time = 0;
        m_nProtestors = 0;
        
	}
    
    ~StudentWorld();
    
    //HELPERS
    
    double dist(int x1, int x2, int y1, int y2); // used to distribute game objects 6.0 units away
    
    
    //METHODS
    
    std::string formatter(int level, int lives, int health, int squirts, int gold, int barrelsLeft, int sonar, int score); // formats the status bar according to the spec
    void setDisplayText(); // uses the previous functin as a helper to update the status
    
    Earth* getEarth(int x, int y){return m_earthArr[x][y];}
    void fillEarth(); // distributes earth on the oilfield and stores it in the earth array
    bool hasEarth(int x, int y); // boolean returns if a point has earth
    bool hasAnyEarth(int x, int y); // boolean returns if 4X4 surrounding region has any earth
    bool hasBoulder(int x, int y); // boolean returns if 4X4 surrounding region has any earth
    bool nearBoulder(int x, int y); // boolena returns if something is a radius of 3.0 units away from the boulder
    void dig(int x, int y); // helper in Tunnelman's doSomething that clears out the surrounding earth
    void addObject(Actor* actr_ptr); // helper used in move() to spawn in new game objects
    
    void distributeBoulders(); // used in init() to distribute initial boulders
    void distributeOil(); // used in init() to distribute initial oil barrels
    void distributeGold(); // used in init() to distribute gold nuggets
    
    
    void trySpawningGoodies(); // used in init() to spawn new goodies randomly as and when required
    void trySpawningProtestors(); // used in init() to spawn new protestors when its possible
    
    bool canPlaceObject(int x, int y); // boolean helper used to determine whether a new goodie can be placed there (whether there's anything else 6.0 units within it
    bool coveredInEarth(int x, int y); // similiar to hasAnyEarth but checks that the point is surrounded by earth instead
    
    
    bool bribeNearbyProtestor(int x, int y); // checks if there's a protestor near a gold nugget to be bribed
    void annoyNearbyProtestors(int x, int y, bool &annoyed_any, int amt, int points); // checks if there is a protestor near a squirt or boulder to be annoyed
    void annoyNearbyTunnelMan(int x, int y, int amt); // checks if there is a tunnelman near a squirt or boulder to be annoyed
    

    
    int getTX() const;
    int getTY() const;
    
    void hurtTM(int amt); // same as annoytunnelman but without the distance requirements
    
 
    void collectGold(){m_tunnelman->incGold();}
    void collectSquirt(){m_tunnelman->incSquirts();}
    void collectOil(){m_tunnelman->incOil();}
    void collectSonar(){m_tunnelman->incSonar();}
    
    void revealViaSonar(int x, int y); // reveals all hidden objects in a 12.0 unit radius
    void findRandomEarthlessSpot(int &x, int &y); // used for spawning water, returns a random earthless spot away from other game objects
    
    
    virtual int init();

    virtual int move();

    virtual void cleanUp();
    


private:
    
    Earth* m_earthArr[VIEW_HEIGHT][VIEW_WIDTH];
    TunnelMan* m_tunnelman;
    std::vector<Actor*> m_gameObjects;
        
    int m_protestor_time;
    int m_nProtestors;
};

#endif // STUDENTWORLD_H_
