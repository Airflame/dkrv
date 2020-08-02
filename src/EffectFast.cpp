#include "../include/EffectFast.h"

EffectFast::EffectFast(float x, float y, bool self, std::vector<Player> &players) : Effect(x, y, self, players) {
    texture.loadFromFile("img/fast.png");
    textureShape.setTexture(&texture);
}

void EffectFast::addEffect(Player *player) {
    player->velocityModifier *= 2;
}

void EffectFast::removeEffect(Player *player) {
    player->velocityModifier /= 2;
}
