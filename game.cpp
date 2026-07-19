#include "game.h"
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>


Game::Game (const std::string &config)
{
    init(config);
}


void Game::init (const std::string &config)
{
    // // read the file 
    // std::ifstream file(path);

    std::string name;
    int width,height,fps,fullScreen = 0;

    // // reading window details
    // file >> name >> width >> height >> fps >> fullScreen;

    // // reading player details
    // file >> 
    //     name >> 
    //     m_playerConfig.SR >> 
    //     m_playerConfig.CR >> 
    //     m_playerConfig.S >> 
    //     m_playerConfig.FR >>
    //     m_playerConfig.FG >> 
    //     m_playerConfig.FB >> 
    //     m_playerConfig.OR >>
    //     m_playerConfig.OG >>
    //     m_playerConfig.OB >> 
    //     m_playerConfig.OT >> 
    //     m_playerConfig.V;

    // // reading enemy details
    // file >> 
    //     name >>
    //     m_enemyConfig.SR >>
    //     m_enemyConfig.CR >> 
    //     m_enemyConfig.sMin >> 
    //     m_enemyConfig.sMax >> 
    //     m_enemyConfig.OR >>
    //     m_enemyConfig.OG >> 
    //     m_enemyConfig.OB >>
    //     m_enemyConfig.OT >> 
    //     m_enemyConfig.Vmin >> 
    //     m_enemyConfig.Vmax >> 
    //     m_enemyConfig.L >> 
    //     m_enemyConfig.SP;

    // // reading bullets details
    // file >> 
    //     name >>
    //     m_bulletConfig.SR >>
    //     m_bulletConfig.CR >> 
    //     m_bulletConfig.S >> 
    //     m_bulletConfig.FR >> 
    //     m_bulletConfig.FG >> 
    //     m_bulletConfig.FB >> 
    //     m_bulletConfig.OR >>
    //     m_bulletConfig.OG >> 
    //     m_bulletConfig.OB >>
    //     m_bulletConfig.OT >> 
    //     m_bulletConfig.V >> 
    //     m_bulletConfig.L;

        

    if(fullScreen) m_window.create(
        sf::VideoMode({1000 , 800}),
        "DSU_Strike",
        sf::State::Fullscreen);

    else m_window.create(
        sf::VideoMode({1000 , 800}),
        "DSU_Strike");

    m_window.setFramerateLimit(60);

}



void Game::run()
{
    spawnPlayer();
    while(m_running)
    {
        m_entities.update();

   
        sEnemySpawner();
        
        sUserInput();
        sMovement();
        sEnemySpawner();

        sRender();
        m_currentFrame++;
    }
}




void Game::spawnPlayer()
{
    auto e = m_entities.addEntity("player");
    float cx = m_window.getSize().x / 2.0f;
    float cy = m_window.getSize().y / 2.0f;

    e -> cTransform = std::make_shared<CTransform> (Vec2(cx, cy), Vec2(0, 0), 0.0f );
    e -> cShape = std::make_shared<CShape> (40, sf::Color(0,0,0), 8, sf::Color(255,255,255), 5);
    e -> cInput = std::make_shared<CInput> ();
    m_player = e;
   
}



void Game::sEnemySpawner ()
{
    if(m_currentFrame - m_lastEnemeySpawnTime >= 240) spawnEnemy();
}

void Game::spawnEnemy()
{
    auto e = m_entities.addEntity("enemy");
    int cx = m_window.getSize().x;
    int cy = m_window.getSize().y;


    float px = rand() % (cx - 100) + 50;
    float py = rand() % (cy - 100) + 50;

    e -> cTransform = std::make_shared<CTransform> (Vec2(px, py), Vec2(5,5), 0.0f );
    e -> cShape = std::make_shared<CShape> (50, sf::Color(255,255,255), 3, sf::Color(220,220,220), 12);

    // no input component needed for enemy

    m_lastEnemeySpawnTime = m_currentFrame;

}


void Game::spawnBullet()
{
    

}


void Game::sMovement ()
{
    float w = m_window.getSize().x;
    float h = m_window.getSize().y;

    // we dont want a uniform speed or sunifrom shift
    // we want a jerk
    // thus at every frame off the velocity, if previously provides
    m_player -> cTransform -> velocity = { 0.0f , 0.0f };


    // handle input instructions
    if(m_player -> cInput -> up)
    {
        m_player -> cTransform -> velocity.y = -5;
    }
    if(m_player -> cInput -> down)
    {
        m_player -> cTransform -> velocity.y = 5;
    }
    if(m_player -> cInput -> left)
    {
        m_player -> cTransform -> velocity.x = -5;
    }
    if(m_player -> cInput -> right)
    {
        m_player -> cTransform -> velocity.x = 5;
    }

    // if goes out of bound handle it
    float nx = m_player -> cTransform -> position.x + m_player -> cTransform -> velocity.x;
    float ny = m_player -> cTransform -> position.y + m_player -> cTransform -> velocity.y;
    float err = m_player -> cShape -> shape.getRadius() + 
                m_player -> cShape -> shape.getOutlineThickness();
    if(nx - err <= 0) nx = err;
    if(nx + err >= w) nx = w - err;
    if(ny + err >= h) ny = h - err;
    if(ny - err <= 0) ny = err;


    m_player -> cTransform -> position = { nx , ny };
    
   


    for(auto &e : m_entities.getEntities())
    {
        float newX = e -> cTransform -> position.x + e -> cTransform -> velocity.x;
        float newY = e -> cTransform -> position.y + e -> cTransform -> velocity.y;

        if(e -> tag() == "enemy")
        {   // logic to prevent escaping the boundaries
            float r = e -> cShape -> shape.getRadius();
            float t = e -> cShape -> shape.getOutlineThickness();
            if( ( newX + r + t >= w) || ( newX - r - t <= 0 ) )
            {
                e -> cTransform -> velocity.x = -1.0f * e -> cTransform -> velocity.x;
            } 

            if( ( newY + r + t >= h) || ( newY - r - t <= 0 ) )
            {
                e -> cTransform -> velocity.y = -1.0f * e -> cTransform -> velocity.y;
            } 

        }


        // setting the new positions every time 
        e -> cTransform -> position = { newX , newY };
        e -> cShape -> shape.setPosition( { newX , newY } );
        
        
    }

}




void Game::sRender()
{
    m_window.clear();

    // render player
    m_player->cShape->shape.setPosition( { m_player->cTransform->position.x, m_player->cTransform->position.y } );
    m_player->cTransform->angle += 1.0f;
    m_player->cShape->shape.setRotation(sf::degrees(m_player->cTransform->angle));
    m_window.draw(m_player->cShape->shape);

    for(auto &e : m_entities.getEntities())
    {
        e->cTransform->angle += 1.0f;
        e->cShape->shape.setRotation(sf::degrees(e->cTransform->angle)); 
        m_window.draw(e->cShape->shape);
    }

    m_window.display();

}



void Game::sUserInput ()
{
    // handling events here
    while(const std::optional event = m_window.pollEvent())
    {
        if(event -> is <sf::Event::Closed>())
        {
            m_window.close();
            m_running = 0;
        }

        const auto* key = event -> getIf <sf::Event::KeyPressed>();
        if(key && key -> code == sf::Keyboard::Key::E)
        {
            m_window.close();
            m_running = 0;
        }
        if(key && key -> code == sf::Keyboard::Key::Up)
        {
            m_player -> cInput -> up = 1;
        }
        else if(key && key -> code == sf::Keyboard::Key::Down)
        {
            m_player -> cInput -> down = 1;
        }
        else if(key && key -> code == sf::Keyboard::Key::Left)
        {
            m_player -> cInput -> left = 1;
        }
        else if(key && key -> code == sf::Keyboard::Key::Right)
        {
            m_player -> cInput -> right = 1;
        }





        const auto* keyy = event -> getIf <sf::Event::KeyReleased>();
        if(keyy && keyy -> code == sf::Keyboard::Key::Up)
        {
            m_player -> cInput -> up = 0;
        }
        else if(keyy && keyy -> code == sf::Keyboard::Key::Down)
        {
            m_player -> cInput -> down = 0;
        }
        else if(keyy && keyy -> code == sf::Keyboard::Key::Left)
        {
            m_player -> cInput -> left = 0;
        }
        else if(keyy && keyy -> code == sf::Keyboard::Key::Right)
        {
            m_player -> cInput -> right = 0;
        }


    }






    

}