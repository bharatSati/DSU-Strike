#include "game.h"
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>



Game::Game (const std::string &config)
{
    init(config);
}


void Game::init (const std::string &config)
{
    // read the file 
    std::ifstream file(config);

    std::string type;
    unsigned int width,height;
    float fps,fullScreen;

    // reading window details
    file >> type;
    if(type == "Window")
    {
        file >> width >> height >> fps >> fullScreen;
        std::cout << width << std::endl;
    }
    
    file >> type;
    if(type == "Player")
    {
        // reading player details
        file >>
        m_playerConfig.SR >> 
        m_playerConfig.CR >> 
        m_playerConfig.S >> 
        m_playerConfig.FR >>
        m_playerConfig.FG >> 
        m_playerConfig.FB >> 
        m_playerConfig.OR >>
        m_playerConfig.OG >>
        m_playerConfig.OB >> 
        m_playerConfig.OT >> 
        m_playerConfig.V;
    }

    
    file >> type;
    if(type == "Enemy")
    {
        // reading enemy details
        file >> 
        m_enemyConfig.SR >>
        m_enemyConfig.CR >> 
        m_enemyConfig.sMin >> 
        m_enemyConfig.sMax >> 
        m_enemyConfig.OR >>
        m_enemyConfig.OG >> 
        m_enemyConfig.OB >>
        m_enemyConfig.OT >> 
        m_enemyConfig.Vmin >> 
        m_enemyConfig.Vmax >> 
        m_enemyConfig.L >> 
        m_enemyConfig.SP;
    }

    
    file >> type;
    if(type == "Bullet")
    {
        // reading bullets details
        file >>
        m_bulletConfig.SR >>
        m_bulletConfig.CR >> 
        m_bulletConfig.S >> 
        m_bulletConfig.FR >> 
        m_bulletConfig.FG >> 
        m_bulletConfig.FB >> 
        m_bulletConfig.OR >>
        m_bulletConfig.OG >> 
        m_bulletConfig.OB >>
        m_bulletConfig.OT >> 
        m_bulletConfig.V >> 
        m_bulletConfig.SP;
    }

        

    if(fullScreen) m_window.create(
        sf::VideoMode::getDesktopMode(),
        "DSU_Strike",
        sf::State::Fullscreen);

    else m_window.create(
        sf::VideoMode({width , height}),
        "DSU_Strike");

    m_window.setFramerateLimit(fps);


    

    file >> type;
    if(type == "Text")
    {
        float px,py;
        int size,r,g,b;
        file >> px >> py >> size >> r >> g >> b;
        
        if(!m_font.openFromFile("assets/fonts/font1.ttf"))
        {
            std::cout << "Font cant be loaded" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        // make object out of it
        m_text.emplace(m_font);

        // then use it properties
        m_text -> setStyle(sf::Text::Bold);
        m_text -> setPosition({ px , py });
        m_text -> setCharacterSize(size);
        m_text -> setFillColor(sf::Color(r,g,b));
        m_text -> setString("SCORE : 0");
    }

}



void Game::run()
{
    spawnPlayer();
    while(m_running)
    {
        m_entities.update();

        if(!m_paused)
        {
            sEnemySpawner();
            sMovement();
            sEnemySpawner();
            sCollisions();
            sLifespan("bullet");
            sLifespan("smallenemy");

        }
   
        
        sUserInput();
        
        
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
    e -> cShape = std::make_shared<CShape> (40 , 
            sf::Color(m_playerConfig.FR ,m_playerConfig.FG ,m_playerConfig.FB , 0) ,
            m_playerConfig.V,
            sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), 
            m_playerConfig.OT
        );
    e -> cInput = std::make_shared<CInput> ();
    m_player = e;
   
}



void Game::sEnemySpawner ()
{
    if(m_currentFrame - m_lastEnemySpawnTime >= m_enemyConfig.SP) spawnEnemy();
}

void Game::spawnEnemy()
{
    auto e = m_entities.addEntity("enemy");
    int cx = m_window.getSize().x;
    int cy = m_window.getSize().y;

    int r = m_playerConfig.SR, t = m_playerConfig.CR;
    float px = rand() % (cx - 2*r) + r;  // px = [r,cx - r]
    float py = rand() % (cy - 2*r) + r;  // py = [r, cy - r]

    // randome speed b/w [smin , smax]
    int speed = rand () % (m_enemyConfig.sMax - m_enemyConfig.sMin + 1) + m_enemyConfig.sMin; // [sMin , ]
    // std::cout << speed << std::endl;


    // random number of sides b/w [Vin , Vmax]
    int sides = rand () % (m_enemyConfig.Vmax - m_enemyConfig.Vmin + 1) + m_enemyConfig.Vmin;

    // random fill color 
    int FR = rand() % 136 + 80;   // inside red = [80 - 215]
    int FG = rand() % 136 + 80;   // inside green = [80 - 215]
    int FB = rand() % 136 + 80;   // inside blue [80 - 215]

    // give it a random angle of projection
    float angle = rand() % (360) + 1;

    float pi = 3.140;
    angle = (angle * pi)/180;
    float vx = speed * std::cos(angle);
    float vy = speed * std::sin(angle);

    e -> cTransform = std::make_shared<CTransform> (Vec2(px, py), Vec2(vx,vy), 0.0f );
    e -> cShape = std::make_shared<CShape> (
            m_enemyConfig.SR , // r
            sf::Color(FR , FG , FB) , // fill color
            sides , // number of sides
            sf::Color(m_enemyConfig.OR , m_enemyConfig.OG , m_enemyConfig.OB), // outline color
            m_enemyConfig.OT  // outline thickness
        );

    // no input component needed for enemy

    m_lastEnemySpawnTime = m_currentFrame;

}


void Game::sBulletSpawner(std::shared_ptr<Entity> &source, Vec2 &target)
{
    // source is the our player which generated the bullet
    // target is the mouse pointer

    // find unit vector in direction of final - initial
    // then multiply the magnitude of speed to it
    auto e = m_entities.addEntity("bullet");
    Vec2 vel = target - source -> cTransform -> position;
    vel.normalize().scale(m_bulletConfig.S);

    e -> cTransform = std::make_shared<CTransform> (source -> cTransform -> position , vel , 0.0);
    e -> cShape = std::make_shared<CShape> (
            m_bulletConfig.SR , // r
            sf::Color(m_bulletConfig.FR ,m_bulletConfig.FG , m_bulletConfig.FB) , // f color
            m_bulletConfig.V , // number of sides
            sf::Color(m_bulletConfig.OR ,m_bulletConfig.OG , m_bulletConfig.OB), // o color
            m_bulletConfig.OT // o thickness
        );
    // for bullet we need to it to fade out and thus remove after some number of frame
    e -> cLifespan = std::make_shared<CLifespan> (m_bulletConfig.SP);

}


void Game::sSmallEnemySpawner (std::shared_ptr<Entity> &enemy)
{
    // whenever a bullet hits enemy, some clones are geberated
    // their appearance and disappearance is handled by lifespan system
    // here we are just generating them

    // each clone is generated along each side
    // we have angle as well as magnitude

    // details of parent
    int sides = enemy -> cShape -> shape.getPointCount();
    float speed = enemy -> cTransform -> velocity.distance(Vec2 (0,0));
    sf::Color fillColor = enemy -> cShape -> shape.getFillColor();
    sf::Color outlineColor = enemy -> cShape -> shape.getOutlineColor();
    float r = enemy -> cShape -> shape.getRadius();
    float t = enemy -> cShape -> shape.getOutlineThickness();

    r = r/2,t = t/2;


    float pi = 3.14159265f;
    float baseAngle = 360 / sides;
    float temp = 0;
    for(int i = 1; i <= sides; i++)
    {
        float vx = speed * std::cos( (pi * temp)/180 );
        float vy = speed * std::sin( (pi * temp)/180 );
        auto e = m_entities.addEntity("smallenemy");
        e -> cTransform = std::make_shared<CTransform> (
            enemy -> cTransform -> position,
            Vec2(vx , vy),
            0.0f
        );

        e -> cShape = std::make_shared<CShape> (
            r ,
            fillColor ,
            sides ,
            outlineColor ,
            t
        );

        e -> cLifespan = std::make_shared<CLifespan> (m_enemyConfig.L);

        temp += baseAngle;
    }

    enemy -> destroy();
}




void Game::sLifespan (const std::string &tag)
{
    // here we handle elements related with lifespan
    // fade them acc to their life span
    // also remove them whenever these lifespan ends

    for(auto &it : m_entities.getEntityMap()[tag])
    {
        it -> cLifespan -> remaining -= 1;
        float num = it -> cLifespan -> remaining;
        float deno = it -> cLifespan -> total;
        if(num <= 0)
        {
            it -> destroy();
            continue;
        }
        float alpha =  num / deno * 255.0f;
        sf::Color fillcolor = it -> cShape -> shape.getFillColor(); 
        sf::Color outlinecolor = it -> cShape -> shape.getOutlineColor();
        fillcolor.a = alpha;
        outlinecolor.a = alpha;
        // std::cout << alpha << std::endl;
        it -> cShape -> shape.setFillColor(fillcolor);
        it -> cShape -> shape.setOutlineColor(outlinecolor);
        
    }
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
        m_player -> cTransform -> velocity.y = -1*m_playerConfig.S;
    }
    if(m_player -> cInput -> down)
    {
        m_player -> cTransform -> velocity.y = m_playerConfig.S;
    }
    if(m_player -> cInput -> left)
    {
        m_player -> cTransform -> velocity.x = -1*m_playerConfig.S;
    }
    if(m_player -> cInput -> right)
    {
        m_player -> cTransform -> velocity.x = m_playerConfig.S;
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
                e -> cTransform -> velocity.x *= -1.0f;;
            } 

            if( ( newY + r + t >= h) || ( newY - r - t <= 0 ) )
            {
                e -> cTransform -> velocity.y *= -1.0f;
            } 

        }

        // setting the new positions every time 
        e -> cTransform -> position = { newX , newY };
        e -> cShape -> shape.setPosition( { newX , newY } );
        
        
    }

}


void Game::sCollisions ()
{

    float x = m_player -> cTransform -> position.x;
    float y = m_player -> cTransform -> position.y;
    float r = m_player -> cShape -> shape.getRadius(); 
    // if some enemy hits me, i should be spawned again and old player should be removed
    for(auto &enemy : m_entities.getEntityMap()["enemy"])
    {
        float x1 = enemy -> cTransform -> position.x;
        float y1 = enemy -> cTransform -> position.y;
        float r1 = enemy -> cShape -> shape.getRadius();

        if( (x - x1)*(x - x1) + (y - y1)*(y - y1) <= (r + r1)*(r + r1))
        {
            // collision detected between player and enemy
            // remove current player and sspawn it again
            m_score = 0;
            m_text -> setString("SCORE : " + std::to_string(m_score));
            m_player -> destroy();
            spawnPlayer();
            break;
        }
    }





    // handle the situation whenever a bullet hits any enemy
    // just mark the m_alive of enemy = 0, whenever bullet hits it

    for(auto &enemy : m_entities.getEntityMap()["enemy"])
    {
        float x1 = enemy -> cTransform -> position.x;
        float y1 = enemy -> cTransform -> position.y;
        float r1 = enemy -> cShape -> shape.getRadius();
        for(auto &bullet : m_entities.getEntityMap()["bullet"])
        {
            float x2 = bullet -> cTransform -> position.x;
            float y2 = bullet -> cTransform -> position.y;
            float r2 = bullet -> cShape -> shape.getRadius();
            float dist = (x1 - x2) * (x1 - x2) + (y1 - y2)*(y1 - y2);
            if(dist <= (r1 + r2)*(r1 + r2))
            {
                // collision detected
                // remove this enemy and break out of this enemy loop
                m_score += 1;
                m_text -> setString("SCORE : " + std::to_string(m_score));
                sSmallEnemySpawner(enemy);
                break;
            }
        }
    }

}




void Game::sRender()
{
    m_window.clear();

    // render player
    m_player -> cShape -> shape.setPosition( { 
                m_player -> cTransform -> position.x, 
                m_player -> cTransform -> position.y } );
    m_player -> cTransform -> angle += 1.0f;
    m_player -> cShape -> shape.setRotation(sf::degrees(m_player -> cTransform -> angle));
    m_window.draw(m_player -> cShape -> shape);

    for(auto &e : m_entities.getEntities())
    {
        e -> cTransform -> angle += 1.0f;
        e -> cShape -> shape.setRotation(sf::degrees(e -> cTransform -> angle)); 
        m_window.draw(e -> cShape -> shape);
    }

    m_window.draw(*m_text);

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
        if(key && key -> code == sf::Keyboard::Key::P)
        {
            m_paused = 1 - m_paused;
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

        const auto mouse = event -> getIf <sf::Event::MouseButtonPressed>();
        if(mouse && mouse -> button == sf::Mouse::Button::Left)
        {
            Vec2 source = { (float) mouse -> position.x , (float)mouse -> position.y };
            sBulletSpawner(m_player, source);
        }


    }






    

}