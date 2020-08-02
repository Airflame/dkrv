#include "../include/EffectFast.h"

EffectFast::EffectFast(float x, float y, bool self, std::vector<Player> &players) : Effect(x, y, self, players) {
    texture.loadFromFile("img/fast.png");
    textureShape.setTexture(&texture);
}

void EffectFast::addEffect(Player *player) {
    player->velocityModifier *= 2;
    std::cout << player->velocityModifier << std::endl;
}

void EffectFast::removeEffect(Player *player) {
    player->velocityModifier /= 2;
    std::cout << player->velocityModifier << std::endl;
}
