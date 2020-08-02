#ifndef DKRV_EFFECTSLOW_H
#define DKRV_EFFECTSLOW_H
#include "Effect.h"


class EffectSlow : public Effect {
private:
    void addEffect(Player *player) override;
    void removeEffect(Player *player) override;
public:
    EffectSlow(float x, float y, bool self, std::vector<Player>& players);
};


#endif //DKRV_EFFECTSLOW_H
