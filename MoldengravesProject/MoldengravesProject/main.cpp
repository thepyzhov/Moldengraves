#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

// Here is a small helper for you! Have a look.
#include "ResourcePath.hpp"

const int mapWidth = 24;
const int mapHeight = 24;

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

int SCREEN_WIDTH = 1440;
int SCREEN_HEIGHT = 900;

int main(int, char const**)
{
    sf::RenderWindow window(sf::VideoMode(1440, 900), "Moldengraves");
//    sf::View playerView;
//    playerView.reset(sf::FloatRect(200.f, 200.f, 0, 0));
//
//    window.setView(playerView);

    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    int posX = 22;
    int posY = 12;
    
    int dirX = -1;
    int dirY = 0;
    
    double planeX = 0;
    double planeY = 0.66;
    
    sf::Clock clock;
    double time = 0; //time of current frame
    double oldTime = 0; //time of previous frame
    
    oldTime = time;
    time = clock.getElapsedTime().asMicroseconds();
    double frameTime = (time - oldTime) / 1000.0; //frameTime is the time this frame has taken, in seconds

    //speed modifiers
    double moveSpeed = frameTime * 5.0; //the constant value is in squares/second
    double rotSpeed = frameTime * 3.0; //the constant value is in radians/second

    // Start the game loop
    while (window.isOpen())
    {
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
            
//            // Mouse x, y
//            if (event.type == sf::Event::MouseMoved) {
//                std::cout << "Mouse (" << event.mouseMove.x << ", " << event.mouseMove.y << ")" << std::endl;
//            }
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                if (worldMap[int(posX + dirX * moveSpeed)][int(posY)] == false) posX += dirX * moveSpeed;
                if (worldMap[int(posX)][int(posY + dirY * moveSpeed)] == false) posY += dirY * moveSpeed;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                if (worldMap[int(posX - dirX * moveSpeed)][int(posY)] == false) posX -= dirX * moveSpeed;
                if (worldMap[int(posX)][int(posY - dirY * moveSpeed)] == false) posY -= dirY * moveSpeed;
            }
        }

        // Clear screen
        window.clear();
        
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            double cameraX = 2 * x / (double) SCREEN_WIDTH - 1;
            double rayDirX = dirX + planeX * cameraX;
            double rayDirY = dirY + planeY * cameraX;
            
            int mapX = (int) posX;
            int mapY = (int) posY;
            
            double sideDistX;
            double sideDistY;
            
//            double deltaDistX = std::abs(1 / rayDirX);
//            double deltaDistY = std::abs(1 / rayDirY);
            double deltaDistX = (rayDirY == 0) ? 0 : ((rayDirX == 0) ? 1 : std::abs(1 / rayDirX));
            double deltaDistY = (rayDirX == 0) ? 0 : ((rayDirY == 0) ? 1 : std::abs(1 / rayDirY));
            double perpWallDist;
            
            int stepX;
            int stepY;
            
            int hit = 0;
            int side;
            
            if (rayDirX < 0) {
                stepX = -1;
                sideDistX = (posX - mapX) * deltaDistX;
            } else {
                stepX = -1;
                sideDistX = (mapX + 1.0 - posX) * deltaDistX;
            }
            
            if (rayDirY < 0) {
                stepY = -1;
                sideDistY = (posY - mapY) * deltaDistY;
            } else {
                stepY = -1;
                sideDistY = (mapY + 1.0 - posY) * deltaDistY;
            }
            
            // DDA
            while (hit == 0) {
                if (sideDistX < sideDistY) {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                } else {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                }
                if (worldMap[mapX][mapY] > 0) {
                    hit = 1;
                }
            }
            
            if (side == 0) {
                perpWallDist = (mapX - posX + (1- stepX) / 2) / rayDirX;
            } else {
                perpWallDist = (mapY - posY + (1- stepY) / 2) / rayDirY;
            }
            int h = 400;
            
            int lineHeight = (int) (SCREEN_HEIGHT / perpWallDist);
            
            int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
            if (drawStart < 0) {
                drawStart = 0;
            }
            
            int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
            if (drawEnd >= SCREEN_HEIGHT) {
                drawEnd = SCREEN_HEIGHT - 1;
            }
            
            sf::Color lineColor;
            switch (worldMap[mapX][mapY]) {
                case 1:
                    lineColor = sf::Color::Red;
                    break;
                case 2:
                    lineColor = sf::Color::Green;
                    break;
                case 3:
                    lineColor = sf::Color::Blue;
                    break;
                case 4:
                    lineColor = sf::Color::White;
                    break;
                default:
                    lineColor = sf::Color::Yellow;
                    break;
            }
            
            if (side == 1) {
                lineColor.r /= 2;
                lineColor.g /= 2;
                lineColor.b /= 2;
            }
            
            sf::RectangleShape line(sf::Vector2f(1.f, lineHeight));
            line.setPosition(x, drawStart);
            line.setFillColor(lineColor);
            window.draw(line);
        }
    
        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}
