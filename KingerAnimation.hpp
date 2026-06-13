#ifndef KINGER_ANIMATION_HPP
#define KINGER_ANIMATION_HPP

// Constants — avoids magic numbers in both animation and rendering code
static const float BULLET_TRAVEL_SPEED   = 80.0f;
static const float BULLET_SPAWN_Z        = -10.0f;
static const float BULLET_MAX_DISTANCE   = -150.0f;
static const float BULLET_BARREL_Y_BIAS  = -2.0f;

class KingerAnimation
{
public:
    // Idle animation variables
    float idleTimer;
    float hoverOffset;
    float clothRotation;
    float armRotation;

    // Combat skill state variables
    float skillTimer;
    bool  isCastingSkill;
    float skillArmRotation;
    float skillBodyYOffset;
    float skillBodyZOffset;

    // Projectile state variables
    bool  isBulletActive;
    float bulletZOffset;
    float bulletStartY;

    KingerAnimation();

    void updateIdleState(float deltaTime);
    void castGunSkill();
    void updateSkillState(float deltaTime);
};

#endif // KINGER_ANIMATION_HPP

