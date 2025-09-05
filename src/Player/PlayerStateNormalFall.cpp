#include "Player/PlayerStateNormalFall.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Player/PlayerActionAirMoveControl.h"
#include "Player/PlayerAnimator.h"
#include "Player/PlayerConst.h"
#include "Util/PlayerCollisionUtil.h"

namespace {
NERVE_IMPL(PlayerStateNormalFall, Fall);

NERVES_MAKE_NOSTRUCT(PlayerStateNormalFall, Fall);
}

PlayerStateNormalFall::PlayerStateNormalFall(al::LiveActor* player, const PlayerConst* pConst,
                                             const IUsePlayerCollision* collider, const PlayerInput* input,
                                             PlayerAnimator* animator) 
    : al::ActorStateBase("落下", player), mConst(pConst), mCollider(collider), mAnimator(animator) {
        mMoveCtrl = nullptr;
        if (input) {
            mMoveCtrl = new PlayerActionAirMoveControl(player, pConst, input, collider, false);
        }
        initNerve(&Fall, 0);
}

void PlayerStateNormalFall::appear() {
    al::ActorStateBase::appear();
    al::setNerve(this, &Fall);
}

void PlayerStateNormalFall::exeFall() {
    al::LiveActor* actor = mActor;
    if (al::isFirstStep(this)) {
        if (mAnimator) {
            mAnimator->startAnim("Fall");
        }
        else {
            al::startAction(actor, sead::SafeString("Fall").cstr());
        }
        if (mMoveCtrl) {
            mMoveCtrl->setup(100.0, mConst->getJumpMoveSpeedMin(), 0, 0.0,
                             mConst->getJumpGravity(), 0, mConst->getJumpInertiaRate());
        }
    }
    if (mMoveCtrl) {
        mMoveCtrl->update();
    }
    if (!mCollider) {
        if (!al::isOnGround(actor, 0)) {
            return;
        }
    }
    else if (!rs::isOnGround(actor, mCollider)) {
        return;
    }
    if (mCollider) {
        rs::reboundVelocityFromCollision(actor, mCollider, 0.0, 0.0, 1.0);
    }
    else {
        al::reboundVelocityFromCollision(actor, 0.0, 0.0, 1.0);
    }
    kill();
}

