#include "../include/Effect.h"

Effect::Effect(float x, float y, bool self, std::vector<Player> &players) : players(players) {
    position = sf::Vector2f(x, y);
    shape.setRadius(EFFECT_RADIUS);
    shape.setOrigin(EFFECT_RADIUS, EFFECT_RADIUS);
    shape.setPosition(position);
    textureShape.setRadius(EFFECT_RADIUS);
    textureShape.setOrigin(EFFECT_RADIUS, EFFECT_RADIUS);
    textureShape.setPosition(position);
    isInClient = false;
    selfTargeted = self;
    if (selfTargeted)
        shape.setFillColor(sf::Color::Green);
    else
        shape.setFillColor(sf::Color::Red);
}

void Effect::setTexture(const sf::Texture *texture) {
    textureShape.setTexture(texture);
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
            if (timer > EFFECT_TIMER)
                removeEffects();
        } else {
            if (!isInClient) {
                for (int playerId = 0; playerId < players.size(); playerId++) {
                    sf::Vector2f playerPosition = players[playerId].getPosition();
                    if (std::sqrt(
                            (position.x - playerPosition.x) * (position.x - playerPosition.x) +
                            (position.y - playerPosition.y) * (position.y - playerPosition.y)) <
                        PLAYER_RADIUS + EFFECT_RADIUS) {
                        collect(playerId);
                        break;
                    }
                }
            }
        }
    }
}

void Effect::collect(int playerId) {
    if (!collected) {
        collectedPlayerId = playerId;
        collected = true;
        addEffects();
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

void Effect::setAsClient() {
    isInClient = true;
}

bool Effect::isSelfTargeted() const {
    return selfTargeted;
}

bool Effect::isCollected() const {
    return collected;
}

int Effect::getCollectedPlayerId() const {
    return collectedPlayerId;
}

void Effect::addEffect(Player *player) {

}

void Effect::removeEffect(Player *player) {

}