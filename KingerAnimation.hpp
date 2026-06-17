#ifndef KINGER_ANIMATION_HPP
#define KINGER_ANIMATION_HPP

// Constants — avoids magic numbers in both animation and rendering code
static const float BULLET_TRAVEL_SPEED   = 250.0f;
static const float BULLET_SPAWN_Z        = -25.0f; // Pushed out to the exact tip of the gun barrel
static const float BULLET_MAX_DISTANCE   = -1000.0f;
static const float BULLET_BARREL_Y_BIAS  = -2.0f;
static const float HURT_DURATION         = 0.5f;
static const float DEATH_DURATION        = 1.0f;
static const float RESPAWN_DELAY          = 1.0f;

class KingerAnimation
{
public:
    // Idle animation variables
    float idleTimer;
    float hoverOffset;
    float clothRotation;
    float armRotation;
    float armSwayWeight;

    // Gun Shot
    float skillTimer;
    bool  isCastingSkill;
    float skillArmRotation;
    float skillBodyYOffset;
    float skillBodyZOffset;
    float armRecoilOffset;

    // Projectile state variables
    bool  isBulletActive;
    float shootYaw;
    float shootPitch;
    float bulletPosX;
    float bulletPosY;
    float bulletPosZ;
    
    // Trajectory Data
    float bulletStartX;
    float bulletStartY;
    float bulletStartZ;
    float bulletDirX;
    float bulletDirY;
    float bulletDirZ;
    
    float bulletDistance;

    // Roll State Variables
    bool  isRolling;
    float rollTimer;
    int   rollPhase;
    float rollSquashY;
    bool  showBallModel;

    // Ammo & Reload State Variables
    int currentAmmo;
    static const int MAX_AMMO = 30;
    
    bool isReloading;
    float reloadTimer;
    float leftArmReloadPitch;
    float leftArmReloadYaw;
    float leftArmReloadYOffset;
    float rightArmReloadPitch;
    float rightArmReloadYaw;
    
    // Butterfly Heal Variables
    int butterflyCharges;
    bool isHealing;
    float healTimer;
    float leftArmHealPitch;

    // Hurt/Damage State Variables
    bool isHurt;
    float hurtTimer;

    // Death State Variables
    bool isDead;
    float deathTimer;

    KingerAnimation();

    void updateIdleState(float deltaTime);
    void castGunSkill();
    void castRollSkill();
    void castReload();
    void castHealSkill(int& currentHealth, int maxHealth);
    void updateRollState(float deltaTime);
    void updateReloadState(float deltaTime);
    void updateHealState(float deltaTime);
    void updateSkillState(float deltaTime, float currentYaw, float currentPitch, float kingerX, float kingerY, float kingerZ);
    void updateHurtState(float deltaTime);
    void triggerHurt();
    void updateDeathState(float deltaTime);
    void triggerDeath();
};

#endif // KINGER_ANIMATION_HPP

