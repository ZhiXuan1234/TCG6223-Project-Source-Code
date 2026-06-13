#include "KingerAnimation.hpp"
#include <cmath>

KingerAnimation::KingerAnimation()
    : idleTimer(0.0f)
    , hoverOffset(0.0f)
    , clothRotation(0.0f)
    , armRotation(0.0f)
    , skillTimer(0.0f)
    , isCastingSkill(false)
    , skillArmRotation(0.0f)
    , skillBodyYOffset(0.0f)
    , skillBodyZOffset(0.0f)
    , isBulletActive(false)
    , bulletZOffset(0.0f)
    , bulletStartY(0.0f)
{
}

void KingerAnimation::castGunSkill()
{
    if (!isCastingSkill)
    {
        isCastingSkill  = true;
        skillTimer      = 0.0f;
        skillArmRotation  = 0.0f;
        skillBodyYOffset  = 0.0f;
        skillBodyZOffset  = 0.0f;
        // Ensure bullet is inactive at the start of every new cast
        isBulletActive  = false;
        bulletZOffset   = 0.0f;
        bulletStartY    = 0.0f;
    }
}

void KingerAnimation::updateSkillState(float deltaTime)
{
    if (!isCastingSkill)
    {
        skillArmRotation = 0.0f;
        skillBodyYOffset = 0.0f;
        skillBodyZOffset = 0.0f;
        return;
    }

    skillTimer += deltaTime;

    if (skillTimer <= 0.5f)
    {
        // Phase 1 (0.0s - 0.5s) Wind-up
        float t = skillTimer / 0.5f;
        skillArmRotation = t * -45.0f;
        skillBodyYOffset = t * -0.5f;
    }
    else if (skillTimer <= 0.7f)
    {
        // Phase 2 (0.5s - 0.7s) Execution
        float t = (skillTimer - 0.5f) / 0.2f;
        skillArmRotation = -45.0f + t * (90.0f - (-45.0f));
        skillBodyYOffset = -0.5f + t * 0.5f;

        // Spawn the bullet exactly once when Phase 2 begins
        if (!isBulletActive)
        {
            isBulletActive = true;
            bulletZOffset  = BULLET_SPAWN_Z;
            bulletStartY   = hoverOffset + skillBodyYOffset + BULLET_BARREL_Y_BIAS;
        }
    }
    else if (skillTimer <= 1.2f)
    {
        // Phase 3 (0.7s - 1.2s) Recoil & Recovery
        float t = (skillTimer - 0.7f) / 0.5f;
        skillArmRotation = 90.0f * (1.0f - t);

        if (t <= 0.5f)
        {
            float t2 = t / 0.5f;
            skillBodyZOffset = t2 * -2.0f;
        }
        else
        {
            float t2 = (t - 0.5f) / 0.5f;
            skillBodyZOffset = -2.0f * (1.0f - t2);
        }
    }
    else
    {
        // Phase 4: Reset all skill variables
        isCastingSkill   = false;
        skillTimer       = 0.0f;
        skillArmRotation = 0.0f;
        skillBodyYOffset = 0.0f;
        skillBodyZOffset = 0.0f;
    }

    // Bullet travel & despawn — runs independently every tick while active
    if (isBulletActive)
    {
        bulletZOffset += BULLET_TRAVEL_SPEED * deltaTime;

        if (bulletZOffset < BULLET_MAX_DISTANCE)
        {
            isBulletActive = false;
        }
    }
}

void KingerAnimation::updateIdleState(float deltaTime)
{
    idleTimer += deltaTime;

    // Smooth hover animation: Y-axis oscillation for the body
    hoverOffset = 1.2f * std::sin(idleTimer * 2.0f);

    // Fluttering cloth animation: Z-axis rotation oscillation
    clothRotation = 3.0f * std::cos(idleTimer * 2.5f);

    // Breathing arms animation: X-axis rotation oscillation
    armRotation = 4.0f * std::sin(idleTimer * 1.8f);
}
