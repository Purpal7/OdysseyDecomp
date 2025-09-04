#pragma once

#include "Library/Nerve/NerveStateBase.h"

class PlayerStateSwordAttack : public al::ActorStateBase {
public:
    PlayerStateSwordAttack(al::LiveActor* actor, al::LiveActor* a2);

    void appear() override;
    void kill() override;
    void exeAttack();

private:
    al::LiveActor* _20;
    bool mIsPowerGrove;
};

