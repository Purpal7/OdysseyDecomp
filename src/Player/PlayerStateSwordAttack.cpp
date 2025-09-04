#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Player/PlayerStateSwordAttack.h"

namespace {
NERVE_IMPL(PlayerStateSwordAttack, Attack);

NERVES_MAKE_NOSTRUCT(PlayerStateSwordAttack, Attack);
}

PlayerStateSwordAttack::PlayerStateSwordAttack(al::LiveActor* actor, al::LiveActor* a2) 
    : al::ActorStateBase::ActorStateBase("剣攻撃", actor), _20(a2), mIsPowerGrove(false) {
        if (a2) {
            mIsPowerGrove = al::isEqualString(al::getModelName(a2), "PowerGrove");
        }
        initNerve(&Attack, 0);
}

void PlayerStateSwordAttack::appear() {
    mIsDead = false;
    al::setNerve(this, &Attack);
}

void PlayerStateSwordAttack::kill() {
    mIsDead = true;
    if (_20) {
        al::invalidateHitSensors(_20);
    }
}

void PlayerStateSwordAttack::exeAttack() {
    if (al::isFirstStep(this)) {
        if (mIsPowerGrove) {
            al::startAction(mActor, "Fire");
        }
        else {
            al::startAction(mActor, "Punch");
        }
    }
    if (al::isStep(this, 0) && _20) {
        al::invalidateHitSensors(_20);
    }
    bool isGrounded = al::isOnGround(mActor, 2);
    if (isGrounded) {
        const sead::Vector3f* groundNorm = &al::getCollidedGroundNormal(mActor);
        //sead::Vector3f intoGround{-groundNorm->x, -groundNorm->y, -groundNorm->z};
        al::addVelocityToDirection(mActor, -*groundNorm, 1.0f);
        al::scaleVelocityExceptDirection(mActor, -*groundNorm, 0.9f);
        al::reboundVelocityFromCollision(mActor, 0.0f, 0.0f, 1.0f);
        bool doKill = al::isActionEnd(mActor) && isGrounded;
        if (doKill) {
            kill();
        }
        return;
    }
    else {
        al::addVelocityToGravity(mActor, 1.0f);
        al::scaleVelocityDirection(mActor, al::getGravity(mActor), 0.99f);
        al::isActionEnd(mActor);
    }
}