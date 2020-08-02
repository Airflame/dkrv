#ifndef DKRV_EFFECTFAST_H
#define DKRV_EFFECTFAST_H
#include "Effect.h"


class EffectFast : public Effect {
private:
    void addEffect(Player *player) override;
    void removeEffect(Player *player) override;
public:
    EffectFast(float x, float y, bool self, std::vector<Player>& players);
};


#endif //DKRV_EFFECTFAST_H
