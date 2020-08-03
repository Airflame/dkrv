#include "../include/Effect.h"

Effect::Effect(float x, float y, bool self, std::vector<Player> &players) : players(players) {
    position = sf::Vector2f(x, y);
    shape.setRadius(EFFECT_RADIUS);
    shape.setOrigin(EFFECT_RADIUS, EFFECT_RADIUS);
    shape.setPosition(position);
    textureShape.setRadius(EFFECT_RADIUS);
    textureShape.setOrigin(EFFECT_RADIUS, EFFECT_RADIUS);
    textureShape.setPosition(position);
    selfTargeted = self;
    if (selfTargeted)
        shape.setFillColor(sf::Color::Green);
    else
        shape.setFillColor(sf::Color::Red);
}

void Effect::draw(sf::RenderWindow* window) {
    if (!collected) {
        window->draw(shape);
        window->draw(textureShape);
    }
}

void Effect::evaluate(float dt) {
    if (!finished) {
        if (collected) {
            timer += dt;
            if (timer > EFFECT_INTERVAL)
                removeEffects();
        } else {
            for (int playerId = 0; playerId < players.size(); playerId++) {
                sf::Vector2f playerPosition = players[playerId].getPosition();
                if (std::sqrt(
                        (position.x - playerPosition.x) * (position.x - playerPosition.x) +
                        (position.y - playerPosition.y) * (position.y - playerPosition.y)) <
                    PLAYER_RADIUS + EFFECT_RADIUS) {
                    collectedPlayerId = playerId;
                    collected = true;
                    addEffects();
                    break;
                }
            }
        }
    }
}

void Effect::addEffects() {
    if (selfTargeted) {
        addEffect(&players[collectedPlayerId]);
    } else {
        for (int playerId = 0; playerId < players.size(); playerId++) {
            if (playerId == collectedPlayerId)
                continue;
            addEffect(&players[playerId]);
        }
    }
}

void Effect::removeEffects() {
    if (selfTargeted) {
        removeEffect(&players[collectedPlayerId]);
    } else {
        for (int playerId = 0; playerId < players.size(); playerId++) {
            if (playerId == collectedPlayerId)
                continue;
            removeEffect(&players[playerId]);
        }
    }
    finish();
}

void Effect::finish() {
    finished = collected = true;
}

bool Effect::isCollected() const {
    return collected;
}

void Effect::setTexture(const sf::Texture *texture) {
    textureShape.setTexture(texture);
}

void Effect::addEffect(Player *player) {

}

void Effect::removeEffect(Player *player) {

}
