#include "../include/EffectSlow.h"

EffectSlow::EffectSlow(float x, float y, bool self, std::vector<Player> &players) : Effect(
        x, y, self, players) {

}

void EffectSlow::addEffect(Player *player) {
    player->velocityModifier /= 2;
}

void EffectSlow::removeEffect(Player *player) {
    player->velocityModifier *= 2;
}