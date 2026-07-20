#pragma once

#include "entityManager.h"
#include <SFML/Graphics.hpp>
#include <memory>

// data structures / structures to read data from configuration file
struct PlayerConfig
{
    int SR; // shape radious
    int CR; // collision radious
    float S; // speed
    int FR; // fill color : red
    int FG; // fill color : green
    int FB; // fill color : blue
    int OR; // fill outline color : red
    int OG; // fill ouline color : green 
    int OB; // fill outline color : blue
    int OT; // outline thickness
    int V; // number of sides
};

struct EnemyConfig
{
    int SR; // shape radious
    int CR; // collision radious
    int sMin; // min speed
    int sMax; // max speed
    int OR; // fill outline color : red
    int OG; // fill ouline color : green 
    int OB; // fill outline color : blue
    int OT; // outline thickness
    int Vmin; // min number of sides
    int Vmax; // min number of sides
    int L; // clones lifespan
    int SP; // spawn interval - in this interval, enemies are spwned regularly
};


struct  BulletConfig
{
    int SR; // shape radious
    int CR; // collision radious
    float S; // speed
    int FR; // fill color : red
    int FG; // fill color : green
    int FB; // fill color : blue
    int OR; // fill outline color : red
    int OG; // fill ouline color : green 
    int OB; // fill outline color : blue
    int OT; // outline thickness
    int V; // number of sides
    int L; // bullet lifespan
};




class Game
{
    sf::RenderWindow m_window;
    EntityManager m_entities;
    sf::Font m_font;
    // sf::Text m_text;
    PlayerConfig m_playerConfig;
    EnemyConfig m_enemyConfig;
    BulletConfig m_bulletConfig;
    int m_score = 0;
    int m_currentFrame = 0;
    int m_lastEnemySpawnTime = 0;
    bool m_paused = 0;
    bool m_running = 1;

    std::shared_ptr<Entity> m_player;
    
    void init (const std::string &config);
    void pauseGame ();

    void sMovement ();
    void sUserInput ();
    void sBulletSpawner (std::shared_ptr<Entity> &e, Vec2 &target);
    void sLifespan (const std::string &tag);
    void sRender ();
    void sEnemySpawner ();
    void sSmallEnemySpawner (std::shared_ptr<Entity> &enemy);
    void sCollisions ();

public:

// Game (); 
     Game (const std::string &config); 

    void run ();
    void spawnPlayer ();
    void spawnBullet ();
    void spawnEnemy ();
    void spawnSmallEnemy ();
    

};