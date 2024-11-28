#include <string>

#pragma once
#ifndef PLAYER_H
#define PLAYER_H

class Player {
public:
    Player(const std::string& name);
    void setup();
    void update();
    void move();
    void attack();

    int getHealth() const;
    void setHealth(int h);

private:
    std::string name;
    int health;
    int x, y;
    int speed;
};

#endif