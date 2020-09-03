#ifndef DKRV_EFFECT_H
#define DKRV_EFFECT_H
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Player.h"
#include <iostream>


class Effect {
private:
    sf::CircleShape shape, textureShape;
    sf::Vector2f position;
    std::vector<Player>& players;
    bool selfTargeted, collected = false, finished = false, isInClient;
    int collectedPlayerId;
    float timer = 0;
    void addEffects();
    void removeEffects();
    virtual void addEffect(Player* player);
    virtual void removeEffect(Player* player);
public:
    bool sent = false;
    Effect(float x, float y, bool self, std::vector<Player> &players);
    virtual ~Effect() = default;
    void setTexture(const sf::Texture* texture);
    void draw(sf::RenderWindow* window);
    void evaluate(float dt);
    void collect(int playerId);
    void finish();
    void setAsClient();
    bool isSelfTargeted() const;
    bool isCollected() const;
    int getCollectedPlayerId() const;
};


#endif //DKRV_EFFECT_H
