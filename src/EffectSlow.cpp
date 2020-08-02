#include "../include/EffectSlow.h"

EffectSlow::EffectSlow(float x, float y, bool self, std::vector<Player> &players) : Effect(x, y, self, players) {
    texture.loadFromFile("img/slow.png");
    textureShape.setTexture(&texture);
}

void EffectSlow::addEffect(Player *player) {
    player->velocityModifier /= 2;
    std::cout << player->velocityModifier << std::endl;
}

void EffectSlow::removeEffect(Player *player) {
    player->velocityModifier *= 2;
    std::cout << player->velocityModifier << std::endl;
}