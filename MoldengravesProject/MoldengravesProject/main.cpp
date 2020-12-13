#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <string>

// Here is a small helper for you! Have a look.
#include "ResourcePath.hpp"

// Size of the world map
const int mapWidth = 24;
const int mapHeight = 24;
const float fpsRefreshTime = 0.1f;

int worldMap[mapWidth][mapHeight] =
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

int screenWidth = 1440;
int screenHeight = 900;

int main(int, char const**)
{
    sf::RenderWindow window(sf::VideoMode(1440, 900), "Moldengraves");
    window.setFramerateLimit(60);
    
    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    sf::Font font;
    if (!font.loadFromFile(resourcePath() + "sansation.ttf")) {
        std::cout << "Can't load font." << std::endl;
        return EXIT_FAILURE;
    }
    
    sf::Text fpsText("FPS: ", font, 24);
    fpsText.setPosition(5, 0);
    fpsText.setFillColor(sf::Color::White);
    
    sf::VertexArray lines(sf::Lines, 18 * screenWidth);
    
    // Player
    sf::Vector2f position(22, 12);
    sf::Vector2f direction(-1, 0);
    
    // Camera
    sf::Vector2f plane(0, 0.66);
    
    sf::Clock clock;
    float dtCounter = 0.0f;
    int frameCounter = 0;
    int64_t frameTimeMicro = 0;

    // Start the game loop
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        if (dtCounter >= fpsRefreshTime) {
            float fps = (float) frameCounter / dtCounter;
            frameTimeMicro /= frameCounter;
            // Text output can be placed here
            fpsText.setString("FPS: " + std::to_string(fps));
            //std::cout << "FPS: " << fps << std::endl;
            dtCounter = 0.0f;
            frameCounter = 0;
            frameTimeMicro = 0;
        }
        dtCounter += dt;
        ++frameCounter;
        
        //speed modifiers
        double moveSpeed = 5.0 * dt; //the constant value is in squares/second
        double rotSpeed = 3.0 * dt; //the constant value is in radians/second
        
        int prevMousePosX = sf::Mouse::getPosition(window).x;
        
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
            
            // Mouse movement
//            if (event.type == sf::Event::MouseMoved) {
//                std::cout << "Mouse (" << event.mouseMove.x << ", " << event.mouseMove.y << ")" << std::endl;
//                if (event.mouseMove.x < prevMousePosX) {
//                    float oldDirX = direction.x;
//                    direction.x = direction.x * cos(rotSpeed) - direction.y * sin(rotSpeed);
//                    direction.y = oldDirX * sin(rotSpeed) + direction.y * cos(rotSpeed);
//
//                    float oldPlaneX = plane.x;
//                    plane.x = plane.x * cos(rotSpeed) - plane.y * sin(rotSpeed);
//                    plane.y = oldPlaneX * sin(rotSpeed) + plane.y * cos(rotSpeed);
//                }
//
//                if (event.mouseMove.y > prevMousePosX) {
//                    float oldDirX = direction.x;
//                    direction.x = direction.x * cos(-rotSpeed) - direction.y * sin(-rotSpeed);
//                    direction.y = oldDirX * sin(-rotSpeed) + direction.y * cos(-rotSpeed);
//
//                    float oldPlaneX = plane.x;
//                    plane.x = plane.x * cos(-rotSpeed) - plane.y * sin(-rotSpeed);
//                    plane.y = oldPlaneX * sin(-rotSpeed) + plane.y * cos(-rotSpeed);
//                }
//
//            }
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                if (worldMap[int(position.x + direction.x * moveSpeed)][int(position.y)] == false) position.x += direction.x * moveSpeed;
                if (worldMap[int(position.x)][int(position.y + direction.y * moveSpeed)] == false) position.y += direction.y * moveSpeed;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                if (worldMap[int(position.x - direction.x * moveSpeed)][int(position.y)] == false) position.x -= direction.x * moveSpeed;
                if (worldMap[int(position.x)][int(position.y - direction.y * moveSpeed)] == false) position.y -= direction.y * moveSpeed;
            }
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                float oldDirX = direction.x;
                direction.x = direction.x * cos(rotSpeed) - direction.y * sin(rotSpeed);
                direction.y = oldDirX * sin(rotSpeed) + direction.y * cos(rotSpeed);
                
                float oldPlaneX = plane.x;
                plane.x = plane.x * cos(rotSpeed) - plane.y * sin(rotSpeed);
                plane.y = oldPlaneX * sin(rotSpeed) + plane.y * cos(rotSpeed);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                float oldDirX = direction.x;
                direction.x = direction.x * cos(-rotSpeed) - direction.y * sin(-rotSpeed);
                direction.y = oldDirX * sin(-rotSpeed) + direction.y * cos(-rotSpeed);
                
                float oldPlaneX = plane.x;
                plane.x = plane.x * cos(-rotSpeed) - plane.y * sin(-rotSpeed);
                plane.y = oldPlaneX * sin(-rotSpeed) + plane.y * cos(-rotSpeed);
            }
        }
        
        lines.resize(0);
        
        for (int x = 0; x < screenWidth; x++) {
            
            float cameraX = 2 * x / (double) screenWidth - 1;
            sf::Vector2f rayPos = position;
            sf::Vector2f rayDir = direction + plane * cameraX;
            
            sf::Vector2i mapPos(rayPos);
            sf::Vector2f sideDist;
            sf::Vector2f deltaDist(
                       sqrt(1.0f + pow(rayDir.y, 2) / pow(rayDir.x, 2)),
                       sqrt(1.0f + pow(rayDir.x, 2) / pow(rayDir.y, 2))
            );
            
            float perpWallDist;
            sf::Vector2i step;
            
            int hit = 0;
            int side;
            
            if (rayDir.x < 0) {
                step.x = -1;
                sideDist.x = (position.x - mapPos.x) * deltaDist.x;
            } else {
                step.x = 1;
                sideDist.x = (mapPos.x + 1.0 - position.x) * deltaDist.x;
            }
            
            if (rayDir.y < 0) {
                step.y = -1;
                sideDist.y = (position.y - mapPos.y) * deltaDist.y;
            } else {
                step.y = 1;
                sideDist.y = (mapPos.y + 1.0 - position.y) * deltaDist.y;
            }
            
            // DDA
            while (hit == 0) {
                if (sideDist.x < sideDist.y) {
                    sideDist.x += deltaDist.x;
                    mapPos.x += step.x;
                    side = 0;
                } else {
                    sideDist.y += deltaDist.y;
                    mapPos.y += step.y;
                    side = 1;
                }
                if (worldMap[mapPos.x][mapPos.y] > 0) {
                    hit = 1;
                }
            }
            
            if (side == 0) {
                perpWallDist = (mapPos.x - position.x + (1- step.x) / 2) / rayDir.x;
            } else {
                perpWallDist = (mapPos.y - position.y + (1- step.y) / 2) / rayDir.y;
            }
            int h = 400;
            
            int lineHeight = (int) (screenHeight / perpWallDist);
            
            int drawStart = -lineHeight / 2 + screenHeight / 2;
            if (drawStart < 0) {
                drawStart = 0;
            }
            
            int drawEnd = lineHeight / 2 + screenHeight / 2;
            if (drawEnd >= screenHeight) {
                drawEnd = screenHeight - 1;
            }
            
            sf::Color lineColor;
            switch (worldMap[mapPos.x][mapPos.y]) {
                case 1: lineColor = sf::Color::Red; break;
                case 2: lineColor = sf::Color::Green; break;
                case 3: lineColor = sf::Color::Blue; break;
                case 4: lineColor = sf::Color::White; break;
                default: lineColor = sf::Color::Yellow; break;
            }
            
            if (side == 1) {
                lineColor.r /= 2;
                lineColor.g /= 2;
                lineColor.b /= 2;
            }
            
            lines.append(sf::Vertex(sf::Vector2f((float) x, (float) drawStart), lineColor));
            lines.append(sf::Vertex(sf::Vector2f((float) x, (float) drawEnd), lineColor));
        }
        
        window.clear();
        window.draw(lines);
        window.draw(fpsText);
        frameTimeMicro = clock.getElapsedTime().asMicroseconds();
        window.display();
    }

    return EXIT_SUCCESS;
}
