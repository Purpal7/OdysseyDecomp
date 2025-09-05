#pragma once

#include <Library/Nerve/NerveStateBase.h>

class IUsePlayerCollision;
class PlayerActionAirMoveControl;
class PlayerAnimator;
class PlayerConst;
class PlayerInput;

class PlayerStateNormalFall : public al::ActorStateBase {
public:
    PlayerStateNormalFall(al::LiveActor* player, const PlayerConst* pConst, const IUsePlayerCollision* collider, 
                          const PlayerInput* input, PlayerAnimator* animator);
                           
    void appear() override;
    
    void exeFall();
    
private:
    const PlayerConst* mConst;
    const IUsePlayerCollision* mCollider;
    PlayerAnimator* mAnimator;
    PlayerActionAirMoveControl* mMoveCtrl;
};