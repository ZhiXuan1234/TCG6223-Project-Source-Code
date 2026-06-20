#include <GL/glut.h>
#include <iostream>
#include "Caine.hpp"
#include <cmath>    
#include "CNAWorld.hpp"

extern ProjectWorld::MyVirtualWorld myvirtualworld;
extern bool showHitboxes;

using namespace ProjectCaine;

// Adjustable settings for Caine's constant float height
static const float PLAYER_GROUND_Y = -18.7f;
static const float PLAYER_MODEL_HEIGHT = 22.0f; // Visual height offset parameter for Kinger's model
static const float CAINE_MODEL_Y_OFFSET = 19.5f; // Caine's model parts are translated down by -19.5f internally in Caine's drawing code
static const float CAINE_HOVER_HEIGHT_Y = PLAYER_GROUND_Y + 2.0f * PLAYER_MODEL_HEIGHT + CAINE_MODEL_Y_OFFSET; // -18.7f + 44.0f + 19.5f = 44.8f

// Compensating position shift when Caine is in LayDown state (tilted/rolled)
static const float CAINE_LAYDOWN_SHIFT_X = 15.0f;
static const float CAINE_LAYDOWN_SHIFT_Y = 8.0f;

/**
 * Construct a new Caine object and initialize all loading flags, positions, scales, and spawn state.
 */
Caine::Caine()
{
    hatLoaded = false;
    leftHandLoaded = false;
    leftLegLoaded = false;
    leftPalmLoaded = false;
    lowerJawLoaded = false;
    rightHandLoaded = false;
    rightLegLoaded = false;
    rightPalmLoaded = false;
    staffLoaded = false;
    tongueLoaded = false;
    tursoLoaded = false;
    upperJawLoaded = false;
    leftEyeLoaded = false;
    rightEyeLoaded = false;
    uniformScale = 1.0f;
    posX = 0.0f;
    posY = 0.0f;
    posZ = 0.0f;
    spawnX = 0.0f;
    spawnY = 0.0f;
    spawnZ = 0.0f;
    spawnPositionSaved = false;

    // Initialize AI variables
    aiFlightTimer = 0.0f;
    aiFlightDuration = 3.0f;
    aiTargetX = 0.0f;
    aiTargetZ = -120.0f;
    aiTeleportTimer = 0.0f;
    aiTeleportInterval = 10.0f;
    isTeleporting = false;
    isAppearing = false;
    teleportTransitionTimer = 0.0f;
    visualScaleFactor = 1.0f;
    facingYaw = 0.0f;
    currentHealth = 5;
    maxHealth = 5;

    shootCooldownTimer = 0.0f;
    shootInterval = 1.0f;
    for (int i = 0; i < MAX_CAINE_PROJECTILES; i++)
    {
        projectiles[i].active = false;
        projectiles[i].posX = 0.0f;
        projectiles[i].posY = 0.0f;
        projectiles[i].posZ = 0.0f;
        projectiles[i].dirX = 0.0f;
        projectiles[i].dirY = 0.0f;
        projectiles[i].dirZ = 0.0f;
        projectiles[i].lifeTimer = 0.0f;
    }
    for (int i = 0; i < MAX_TELEPORT_PARTICLES; i++)
    {
        teleportParticles[i].active = false;
        teleportParticles[i].posX = 0.0f;
        teleportParticles[i].posY = 0.0f;
        teleportParticles[i].posZ = 0.0f;
        teleportParticles[i].velX = 0.0f;
        teleportParticles[i].velY = 0.0f;
        teleportParticles[i].velZ = 0.0f;
        teleportParticles[i].size = 0.0f;
        teleportParticles[i].r = 0.0f;
        teleportParticles[i].g = 0.0f;
        teleportParticles[i].b = 0.0f;
        teleportParticles[i].alpha = 0.0f;
        teleportParticles[i].lifeTime = 0.0f;
        teleportParticles[i].maxLife = 0.0f;
    }

    sweepActive = false;
    sweepDirection = 0;
    sweepCurrentPos = 0.0f;
    sweepSpeed = 150.0f;
    sweepTimer = 0.0f;
    sweepInterval = 2.0f;
    wasLayingDown = false;
}

void Caine::resetAI()
{
    aiFlightTimer = 0.0f;
    aiFlightDuration = 3.0f;
    aiTargetX = 0.0f;
    aiTargetZ = -120.0f;
    aiTeleportTimer = 0.0f;
    aiTeleportInterval = 8.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 4.0f));
    isTeleporting = false;
    isAppearing = false;
    teleportTransitionTimer = 0.0f;
    visualScaleFactor = 1.0f;
    facingYaw = 0.0f;

    // Reset Caine position to spawn
    if (spawnPositionSaved)
    {
        posX = spawnX;
        posY = spawnY;
        posZ = spawnZ;
    }
    else
    {
        posX = 0.0f;
        posY = 0.0f;
        posZ = -120.0f;
    }

    // Reset animations
    animation.isLaughing = false;
    animation.isLayingDown = false;
    animation.isLeaningForward = false;
    animation.isShootingState = false;

    currentHealth = maxHealth;

    shootCooldownTimer = 0.0f;
    for (int i = 0; i < MAX_CAINE_PROJECTILES; i++)
    {
        projectiles[i].active = false;
    }
    for (int i = 0; i < MAX_TELEPORT_PARTICLES; i++)
    {
        teleportParticles[i].active = false;
    }

    sweepActive = false;
    sweepTimer = 0.0f;
    sweepInterval = 2.0f;
    wasLayingDown = false;
}

/**
 * Update Caine's animation state variables and check death timeline/respawn.
 * deltaTime Elapsed frame time (in seconds).
 */
void Caine::update(float deltaTime)
{
    // Capture the initial spawn position on the first update pass
    if (!spawnPositionSaved)
    {
        spawnX = posX;
        spawnY = posY;
        spawnZ = posZ;
        spawnPositionSaved = true;
    }

    // Accumulate death timer if Caine is dead
    if (animation.isDead)
    {
        animation.deathTimer += deltaTime;
        // Automatically respawn/reanimate Caine after exactly 2 seconds
        if (animation.deathTimer >= 2.0f)
        {
            animation.isDead = false;
            animation.deathTimer = 0.0f;
            currentHealth = maxHealth;

            if (spawnPositionSaved)
            {
                posX = spawnX;
                posY = spawnY;
                posZ = spawnZ;
            }
        }
    }

    // Forward updates to sub-animations
    animation.updateIdleState(deltaTime);
    animation.updateShootingState(deltaTime);
    animation.updateLayDown(deltaTime);
    animation.updateLeanForward(deltaTime);
    animation.updateHurtState(deltaTime);

    if (!::myvirtualworld.isDebugMode && !animation.isDead)
    {
        // 1. Maintain a constant height level of 2x the player model's height above the ground plane
        float targetY = CAINE_HOVER_HEIGHT_Y;
        posY += (targetY - posY) * 3.0f * deltaTime;

        // 2. Face the player at all times
        float dx = ::myvirtualworld.kinger.posX - posX;
        float dz = ::myvirtualworld.kinger.posZ - posZ;
        facingYaw = std::atan2(dx, dz) * 57.2957795f; // convert to degrees

        // 3. Handle Teleportation Cycle
        if (!isTeleporting && !isAppearing)
        {
            // Update teleport cooldown
            aiTeleportTimer += deltaTime;
            if (aiTeleportTimer >= aiTeleportInterval)
            {
                isTeleporting = true;
                teleportTransitionTimer = 0.0f;
            }

            // Normal flight behavior
            aiFlightTimer += deltaTime;
            if (aiFlightTimer >= aiFlightDuration)
            {
                // Safe arena bounds: X [-120.0, 120.0], Z [-200.0, 100.0]
                aiTargetX = -120.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 240.0f));
                aiTargetZ = -200.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 300.0f));
                aiFlightTimer = 0.0f;
                aiFlightDuration = 2.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 3.0f));
            }

            // Move slowly towards target
            float fdx = aiTargetX - posX;
            float fdz = aiTargetZ - posZ;
            float dist = std::sqrt(fdx * fdx + fdz * fdz);
            if (dist > 1.0f)
            {
                float flySpeed = 15.0f; // Slow fly speed
                posX += (fdx / dist) * flySpeed * deltaTime;
                posZ += (fdz / dist) * flySpeed * deltaTime;
            }

            // 4. Handle shooting at the player (only in default state, not in LayDown or LeaningForward)
            if (!animation.isLayingDown && !animation.isLeaningForward)
            {
                shootCooldownTimer += deltaTime;
                if (shootCooldownTimer >= shootInterval)
                {
                    shootCooldownTimer = 0.0f;
                    animation.triggerShootingState();
                    spawnProjectile();
                }
            }
            else
            {
                shootCooldownTimer = 0.0f;
            }
        }
        else if (isTeleporting)
        {
            // Shrink phase: shrink from scale 1.0 to 0.0 in 0.4 seconds
            teleportTransitionTimer += deltaTime;
            float progress = teleportTransitionTimer / 0.4f;
            if (progress >= 1.0f)
            {
                progress = 1.0f;
                isTeleporting = false;
                
                // Spawn poof at old position before moving Caine
                spawnTeleportPoof(posX, posY, posZ);
                
                // Teleport to a random location in the arena
                posX = -120.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 240.0f));
                posZ = -200.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 300.0f));
                
                // Face player immediately
                float ndx = ::myvirtualworld.kinger.posX - posX;
                float ndz = ::myvirtualworld.kinger.posZ - posZ;
                facingYaw = std::atan2(ndx, ndz) * 57.2957795f;

                // Pick random pose: default or laughing state (not leaning state)
                animation.isLeaningForward = false;
                animation.isShootingState = false;
                
                if (rand() % 2 == 0)
                {
                    animation.isLaughing = true;
                    animation.isLayingDown = true;
                }
                else
                {
                    animation.isLaughing = false;
                    animation.isLayingDown = false;
                }

                // Start appear phase
                isAppearing = true;
                teleportTransitionTimer = 0.0f;
                
                // Spawn poof at new position after placing Caine
                spawnTeleportPoof(posX, posY, posZ);
                
                // Pick next random teleport cooldown interval (between 8 and 12 seconds)
                aiTeleportTimer = 0.0f;
                aiTeleportInterval = 8.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 4.0f));
            }
            visualScaleFactor = 1.0f - progress;
        }
        else if (isAppearing)
        {
            // Appear phase: grow from scale 0.0 to 1.0 in 0.4 seconds
            teleportTransitionTimer += deltaTime;
            float progress = teleportTransitionTimer / 0.4f;
            if (progress >= 1.0f)
            {
                progress = 1.0f;
                isAppearing = false;
            }
            visualScaleFactor = progress;
        }

        // 5. Handle sweep attack (only in LayDown state)
        // Detect transition into LayDown state to set initial 2-second delay
        if (animation.isLayingDown && !wasLayingDown)
        {
            sweepTimer = 0.0f;
            sweepInterval = 2.0f; // first attack spawns after 2 seconds
        }
        wasLayingDown = animation.isLayingDown;

        if (animation.isLayingDown)
        {
            if (!sweepActive)
            {
                sweepTimer += deltaTime;
                if (sweepTimer >= sweepInterval)
                {
                    sweepActive = true;
                    sweepTimer = 0.0f;
                    
                    // Choose random direction
                    sweepDirection = rand() % 4;
                    
                    // Choose start coordinate
                    if (sweepDirection == 0 || sweepDirection == 2)
                    {
                        sweepCurrentPos = -290.0f;
                    }
                    else
                    {
                        sweepCurrentPos = 290.0f;
                    }
                    
                    // Set next cooldown interval during LayDown (constant 2 seconds)
                    sweepInterval = 2.0f;
                }
            }
        }
        else
        {
            sweepTimer = 0.0f;
        }

        // Update active sweep movement and collision checking
        if (sweepActive)
        {
            if (sweepDirection == 0 || sweepDirection == 2)
            {
                sweepCurrentPos += sweepSpeed * deltaTime;
                if (sweepCurrentPos >= 290.0f)
                {
                    sweepActive = false;
                }
            }
            else
            {
                sweepCurrentPos -= sweepSpeed * deltaTime;
                if (sweepCurrentPos <= -290.0f)
                {
                    sweepActive = false;
                }
            }

            // Check collision against Kinger
            if (sweepActive && !::myvirtualworld.kinger.animation.isDead)
            {
                bool hit = false;
                float playerX = ::myvirtualworld.kinger.posX;
                float playerY = ::myvirtualworld.kinger.posY;
                float playerZ = ::myvirtualworld.kinger.posZ;
                
                float wallTopY = -18.7f + 12.0f; // wall sits on ground (-18.7) and has total height of 12.0 (panelHalfHeight is 6.0)
                float hitThickness = 8.0f; // horizontal collision width of the sweeping plane

                if (sweepDirection == 0 || sweepDirection == 1) // North-to-South or South-to-North
                {
                    if (std::abs(playerZ - sweepCurrentPos) <= hitThickness && playerY < wallTopY)
                    {
                        hit = true;
                    }
                }
                else // East-to-West or West-to-East
                {
                    if (std::abs(playerX - sweepCurrentPos) <= hitThickness && playerY < wallTopY)
                    {
                        hit = true;
                    }
                }

                if (hit)
                {
                    ::myvirtualworld.kinger.takeDamage(20); // Deal 20 damage (triggers hurt animation)
                }
            }
        }
    }
    else
    {
        visualScaleFactor = 1.0f;
    }

    // Update active projectiles (unconditional)
    float projectileSpeed = 150.0f;
    for (int i = 0; i < MAX_CAINE_PROJECTILES; i++)
    {
        if (projectiles[i].active)
        {
            projectiles[i].posX += projectiles[i].dirX * projectileSpeed * deltaTime;
            projectiles[i].posY += projectiles[i].dirY * projectileSpeed * deltaTime;
            projectiles[i].posZ += projectiles[i].dirZ * projectileSpeed * deltaTime;
            projectiles[i].lifeTimer += deltaTime;

            // Deactivate if lifetime exceeded
            if (projectiles[i].lifeTimer >= 3.0f)
            {
                projectiles[i].active = false;
                continue;
            }

            // Check collision against Kinger
            float kingerScale = ::myvirtualworld.kinger.uniformScale;
            float radius = 3.0f * kingerScale;
            float hitTolerance = 4.0f; // tolerance for thin line collision check

            float dx = projectiles[i].posX - ::myvirtualworld.kinger.posX;
            float dz = projectiles[i].posZ - ::myvirtualworld.kinger.posZ;
            float distXZ = std::sqrt(dx * dx + dz * dz);

            if (distXZ <= (radius + hitTolerance))
            {
                float kingerMinY = ::myvirtualworld.kinger.posY;
                float kingerMaxY = ::myvirtualworld.kinger.posY + 22.0f * kingerScale;

                if (projectiles[i].posY >= (kingerMinY - hitTolerance) && 
                    projectiles[i].posY <= (kingerMaxY + hitTolerance))
                {
                    // Hit! Apply damage to Kinger
                    ::myvirtualworld.kinger.takeDamage(10);
                    projectiles[i].active = false;
                }
            }
        }
    }

    // Update active teleport particles (unconditional)
    for (int i = 0; i < MAX_TELEPORT_PARTICLES; i++)
    {
        if (teleportParticles[i].active)
        {
            teleportParticles[i].posX += teleportParticles[i].velX * deltaTime;
            teleportParticles[i].posY += teleportParticles[i].velY * deltaTime;
            teleportParticles[i].posZ += teleportParticles[i].velZ * deltaTime;
            
            // Add a bit of air resistance/drag
            teleportParticles[i].velX *= 0.92f;
            teleportParticles[i].velY *= 0.92f;
            teleportParticles[i].velZ *= 0.92f;
            
            teleportParticles[i].lifeTime += deltaTime;
            if (teleportParticles[i].lifeTime >= teleportParticles[i].maxLife)
            {
                teleportParticles[i].active = false;
            }
            else
            {
                // Fade out alpha
                float progress = teleportParticles[i].lifeTime / teleportParticles[i].maxLife;
                teleportParticles[i].alpha = 1.0f - progress;
                
                // Grow size slightly as they expand, then shrink
                if (progress < 0.2f)
                {
                    teleportParticles[i].size += deltaTime * 8.0f;
                }
                else
                {
                    teleportParticles[i].size -= deltaTime * 4.0f;
                    if (teleportParticles[i].size < 0.1f) teleportParticles[i].size = 0.1f;
                }
            }
        }
    }
}

/**
 * Handle death initialization, resetting all active posture modifiers and snapping to spawn position.
 */
void Caine::triggerDeath()
{
    animation.isDead = true;
    animation.deathTimer = 0.0f;
    animation.isLaughing = false;
    animation.isLayingDown = false;
    animation.layDownFactor = 0.0f;
    animation.isLeaningForward = false;
    animation.leanForwardFactor = 0.0f;
    animation.isShootingState = false;
    animation.shootingTimer = 0.0f;
    animation.isHurt = false;
    animation.hurtTimer = 0.0f;
}

/**
 * Handle hurt initialization, resetting active shooting postures and starting the flashing timer.
 */
void Caine::triggerHurt()
{
    animation.triggerHurt();
}

/**
 * Set the uniform scale multiplier of the model.
 * scale Uniform scale factor.
 */
void Caine::setScale(float scale)
{
    uniformScale = scale;
}

void Caine::takeDamage(int amount)
{
    if (animation.isHurt || animation.isDead) return;

    currentHealth -= amount;
    if (currentHealth <= 0)
    {
        currentHealth = 0;
        triggerDeath();
    }
    else
    {
        triggerHurt();
    }
}

Vec3 Caine::getCaineWorldCenter() const
{
    float currentScale = uniformScale * visualScaleFactor;
    float layDownAngleRad = animation.layDownFactor * 70.0f * 0.0174532925f;
    
    // Rotation shift of the scaled center around the pivot (0, -19.5)
    float rotShiftX = -11.0f * currentScale * std::sin(layDownAngleRad);
    float rotShiftY = 11.0f * currentScale * (std::cos(layDownAngleRad) - 1.0f);
    
    // Translation shifts (applied before scaling in draw())
    float transShiftX = animation.layDownFactor * CAINE_LAYDOWN_SHIFT_X;
    float transShiftY = animation.layDownFactor * (20.0f + CAINE_LAYDOWN_SHIFT_Y);
    
    // Total local shifts
    float localShiftX = transShiftX + rotShiftX;
    float localShiftY = transShiftY + rotShiftY;
    
    // Center Y starts at posY + hoverOffset, plus local Y shift, plus the baseline offset (-19.5 + 11) scaled
    float centerY = posY + animation.hoverOffset + localShiftY - 19.5f + 11.0f * currentScale;
    
    // Rotate localShiftX by facingYaw (in radians) to project to world coordinates
    float yawRad = facingYaw * 0.0174532925f;
    float centerX = posX + localShiftX * std::cos(yawRad);
    float centerZ = posZ - localShiftX * std::sin(yawRad);

    Vec3 center;
    center.x = centerX;
    center.y = centerY;
    center.z = centerZ;
    return center;
}

void Caine::spawnProjectile()
{
    for (int i = 0; i < MAX_CAINE_PROJECTILES; i++)
    {
        if (!projectiles[i].active)
        {
            projectiles[i].active = true;
            projectiles[i].lifeTimer = 0.0f;

            // Spawn at Caine's center
            Vec3 spawnPos = getCaineWorldCenter();
            projectiles[i].posX = spawnPos.x;
            projectiles[i].posY = spawnPos.y;
            projectiles[i].posZ = spawnPos.z;

            // Direction vector pointing to the player's chest (approx. posY + 11.0f)
            float kingerScale = ::myvirtualworld.kinger.uniformScale;
            float targetX = ::myvirtualworld.kinger.posX;
            float targetY = ::myvirtualworld.kinger.posY + 11.0f * kingerScale;
            float targetZ = ::myvirtualworld.kinger.posZ;

            float dx = targetX - projectiles[i].posX;
            float dy = targetY - projectiles[i].posY;
            float dz = targetZ - projectiles[i].posZ;

            float length = std::sqrt(dx * dx + dy * dy + dz * dz);
            if (length > 0.1f)
            {
                projectiles[i].dirX = dx / length;
                projectiles[i].dirY = dy / length;
                projectiles[i].dirZ = dz / length;
            }
            else
            {
                projectiles[i].dirX = 0.0f;
                projectiles[i].dirY = -1.0f;
                projectiles[i].dirZ = 0.0f;
            }
            break;
        }
    }
}

void Caine::spawnTeleportPoof(float x, float y, float z)
{
    int spawned = 0;
    for (int i = 0; i < MAX_TELEPORT_PARTICLES && spawned < 20; i++)
    {
        if (!teleportParticles[i].active)
        {
            teleportParticles[i].active = true;
            teleportParticles[i].posX = x;
            // Center the poof vertically on Caine
            teleportParticles[i].posY = y + 11.0f; 
            teleportParticles[i].posZ = z;
            
            // Random direction in a sphere
            float theta = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f * 3.14159265f;
            float phi = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 3.14159265f;
            float speed = 20.0f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 30.0f; // 20 to 50 units/sec
            
            teleportParticles[i].velX = speed * std::sin(phi) * std::cos(theta);
            teleportParticles[i].velY = speed * std::cos(phi);
            teleportParticles[i].velZ = speed * std::sin(phi) * std::sin(theta);
            
            // Glitch colors (cyan, magenta, yellow, white, red)
            int colType = rand() % 5;
            if (colType == 0) { // Cyan
                teleportParticles[i].r = 0.0f; teleportParticles[i].g = 0.9f; teleportParticles[i].b = 1.0f;
            } else if (colType == 1) { // Magenta / Pink
                teleportParticles[i].r = 1.0f; teleportParticles[i].g = 0.1f; teleportParticles[i].b = 0.6f;
            } else if (colType == 2) { // Yellow
                teleportParticles[i].r = 1.0f; teleportParticles[i].g = 0.9f; teleportParticles[i].b = 0.0f;
            } else if (colType == 3) { // Red
                teleportParticles[i].r = 1.0f; teleportParticles[i].g = 0.1f; teleportParticles[i].b = 0.1f;
            } else { // White
                teleportParticles[i].r = 1.0f; teleportParticles[i].g = 1.0f; teleportParticles[i].b = 1.0f;
            }
            
            teleportParticles[i].size = 2.0f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 4.0f; // size 2 to 6
            teleportParticles[i].alpha = 1.0f;
            teleportParticles[i].lifeTime = 0.0f;
            teleportParticles[i].maxLife = 0.5f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 0.3f; // 0.5 to 0.8 seconds
            
            spawned++;
        }
    }
}


bool Caine::loadHat(const std::string& filePath)
{
    hatLoaded = hatModel.loadFromObjText(filePath);
    return hatLoaded;
}

bool Caine::loadLeftHand(const std::string& filePath)
{
    leftHandLoaded = leftHandModel.loadFromObjText(filePath);
    return leftHandLoaded;
}

bool Caine::loadLeftLeg(const std::string& filePath)
{
    leftLegLoaded = leftLegModel.loadFromObjText(filePath);
    return leftLegLoaded;
}

bool Caine::loadLeftPalm(const std::string& filePath)
{
    leftPalmLoaded = leftPalmModel.loadFromObjText(filePath);
    return leftPalmLoaded;
}

bool Caine::loadLowerJaw(const std::string& filePath)
{
    lowerJawLoaded = lowerJawModel.loadFromObjText(filePath);
    return lowerJawLoaded;
}

bool Caine::loadRightHand(const std::string& filePath)
{
    rightHandLoaded = rightHandModel.loadFromObjText(filePath);
    return rightHandLoaded;
}

bool Caine::loadRightLeg(const std::string& filePath)
{
    rightLegLoaded = rightLegModel.loadFromObjText(filePath);
    return rightLegLoaded;
}

bool Caine::loadRightPalm(const std::string& filePath)
{
    rightPalmLoaded = rightPalmModel.loadFromObjText(filePath);
    return rightPalmLoaded;
}

bool Caine::loadStaff(const std::string& filePath)
{
    staffLoaded = staffModel.loadFromObjText(filePath);
    return staffLoaded;
}

bool Caine::loadTongue(const std::string& filePath)
{
    tongueLoaded = tongueModel.loadFromObjText(filePath);
    return tongueLoaded;
}

bool Caine::loadTurso(const std::string& filePath)
{
    tursoLoaded = tursoModel.loadFromObjText(filePath);
    return tursoLoaded;
}

bool Caine::loadUpperJaw(const std::string& filePath)
{
    upperJawLoaded = upperJawModel.loadFromObjText(filePath);
    return upperJawLoaded;
}

bool Caine::loadRightEye(const std::string& filePath)
{
    rightEyeLoaded = rightEyeModel.loadFromObjText(filePath);
    return rightEyeLoaded;
}

bool Caine::loadLeftEye(const std::string& filePath)
{
    leftEyeLoaded = leftEyeModel.loadFromObjText(filePath);
    return leftEyeLoaded;
}

// ==========================================
// Rendering Methods for Individual Parts
// ==========================================

/**
 * Render Caine's hat.
 */
void Caine::drawHat() const
{
    if (!hatLoaded) return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, hatTextureID);

    glPushMatrix();
    glRotatef(180, 0, 1, 0);
    glTranslatef(0.0f, -19.5f, 20.0f);
    glScalef(3.0f, 3.0f, 3.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    hatModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/**
 * Render Caine's left hand, including laying down, lean forward, and shooting transitions.
 */
void Caine::drawLeftHand() const
{
    if (!leftHandLoaded) return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, leftHandTextureID);

    glPushMatrix();
    glRotatef(180, 0, 1, 0);
    glTranslatef(0.0f, -19.5f, 20.0f);

    Vec3 center = leftHandModel.getCenter();
    center.x -= 1.25f; // Offset to line up shoulder pivot
    float scale = 3.0f;

    // --- Pivot Matrix Sandwich for Left Hand ---
    // 1. Translate to shoulder pivot center in scaled space
    glTranslatef(center.x * scale, center.y * scale, center.z * scale);
    
    // 2. Apply shooting/pointing rotation sequence
    animation.applyShootingAnimation();

    // 3. Apply raising arm rotations when laying down
    glRotatef(animation.layDownFactor * 45.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(animation.layDownFactor * 90.0f, 1.0f, 0.0f, 0.0f);

    // 4. Apply forward arm pitch when leaning forward
    glRotatef(animation.leanForwardFactor * 45.0f, 0.0f, 1.0f, 0.0f);

    // 5. Translate back from the shoulder pivot center
    glTranslatef(-center.x * scale, -center.y * scale, -center.z * scale);

    glScalef(scale, scale, scale);

    glColor3f(1.0f, 1.0f, 1.0f);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    leftHandModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/**
 * Render Caine's left leg.
 */
void Caine::drawLeftLeg() const
{
    if (!leftLegLoaded) return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, leftLegTextureID);

    glPushMatrix();
    glRotatef(180, 0, 1, 0);
    glTranslatef(0.0f, -19.5f, 20.0f);
    glScalef(3.0f, 3.0f, 3.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    leftLegModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/**
 * Render Caine's left palm, tracking parent hand position.
 */
void Caine::drawLeftPalm() const
{
    if (!leftPalmLoaded) return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, leftPalmTextureID);

    glPushMatrix();
    glRotatef(180, 0, 1, 0);
    glTranslatef(0.0f, -19.5f, 20.0f);

    Vec3 center = leftHandModel.getCenter();
    center.x -= 1.25f; // Match parental shoulder offset
    float scale = 3.0f;

    // --- Pivot Matrix Sandwich for Left Palm (inherits hand transformations) ---
    glTranslatef(center.x * scale, center.y * scale, center.z * scale);
    animation.applyShootingAnimation();
    glRotatef(animation.layDownFactor * 45.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(animation.layDownFactor * 90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(animation.leanForwardFactor * 45.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(-center.x * scale, -center.y * scale, -center.z * scale);

    glScalef(scale, scale, scale);

    glColor3f(1.0f, 1.0f, 1.0f);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    leftPalmModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/**
 * Render Caine's lower jaw, handling breathing mouth motions and death jaw drop.
 */
void Caine::drawLowerJaw() const
{
    if (!lowerJawLoaded) return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, lowerJawTextureID);

    glPushMatrix();
    glRotatef(180, 0, 1, 0);
    glTranslatef(0.0f, -19.5f, 20.0f);

    // Idle breathing TMJ swing
    glRotatef(animation.mouthOpenFactor * -5.0f, 1.0f, 0.0f, 0.0f);

    // Lean forward jaw rotation
    glRotatef(animation.leanForwardFactor * 10.0f, 0.0f, 1.0f, 0.0f);

    // Death wide mouth drop
    if (animation.isDead)
    {
        float deathFactor = animation.deathTimer;
        if (deathFactor > 1.0f) deathFactor = 1.0f;
        glRotatef(deathFactor * 15.0f, 0.0f, 1.0f, 0.0f);
    }

    glScalef(3.0f, 3.0f, 3.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    lowerJawModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/**
 * Render Caine's right hand.
 */
void Caine::drawRightHand() const
{
    if (!rightHandLoaded) return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, rightHandTextureID);

    glPushMatrix();
    glRotatef(180, 0, 1, 0);
    glTranslatef(0.0f, -19.5f, 20.0f);

    Vec3 center = rightHandModel.getCenter();
    center.x += 1.25f; // Symmetrical shoulder alignment offset
    float scale = 3.0f;

    // --- Pivot Matrix Sandwich for Right Hand ---
    glTranslatef(center.x * scale, center.y * scale, center.z * scale);
    
    // Resting roll angle (fades out when laying down)
    float rightRoll = 30.0f * (1.0f - animation.layDownFactor);
    glRotatef(rightRoll, 0.0f, 0.0f, 1.0f);

    // Arm raising when laying down
    glRotatef(animation.layDownFactor * -45.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(animation.layDownFactor * 90.0f, 1.0f, 0.0f, 0.0f);

    // Arm swing forward when leaning forward
    glRotatef(animation.leanForwardFactor * -45.0f, 0.0f, 1.0f, 0.0f);

    glTranslatef(-center.x * scale, -center.y * scale, -center.z * scale);

    glScalef(scale, scale, scale);

    glColor3f(1.0f, 1.0f, 1.0f);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    rightHandModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/**
 * Render Caine's right leg.
 */
void Caine::drawRightLeg() const
{
    if (!rightLegLoaded) return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, rightLegTextureID);

    glPushMatrix();
    glRotatef(180, 0, 1, 0);
    glTranslatef(0.0f, -19.5f, 20.0f);
    glScalef(3.0f, 3.0f, 3.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    rightLegModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/**
 * Render Caine's right palm.
 */
void Caine::drawRightPalm() const
{
    if (!rightPalmLoaded) return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, rightPalmTextureID);

    glPushMatrix();
    glRotatef(180, 0, 1, 0);
    glTranslatef(0.0f, -19.5f, 20.0f);

    if (rightHandLoaded)
    {
        Vec3 center = rightHandModel.getCenter();
        center.x += 1.25f; // Match right shoulder offset
        float scale = 3.0f;

        // Pivot Matrix Sandwich for Right Palm (inherits hand transformations)
        glTranslatef(center.x * scale, center.y * scale, center.z * scale);
        
        float rightRoll = 30.0f * (1.0f - animation.layDownFactor);
        glRotatef(rightRoll, 0.0f, 0.0f, 1.0f);
        
        glRotatef(animation.layDownFactor * -45.0f, 0.0f, 0.0f, 1.0f);
        glRotatef(animation.layDownFactor * 90.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(animation.leanForwardFactor * -45.0f, 0.0f, 1.0f, 0.0f);
        
        glTranslatef(-center.x * scale, -center.y * scale, -center.z * scale);
    }

    glScalef(3.0f, 3.0f, 3.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    rightPalmModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/**
 * Render Caine's staff, shifting alignment to the left hand grip space.
 */
void Caine::drawStaff() const
{
    if (!staffLoaded) return;

    // Local alignment offsets for Left Hand grip
    float staffOffsetX = 30.0f;
    float staffOffsetY = 18.0f;
    float staffOffsetZ = 3.0f;

    float staffGripRotX = 0.0f;
    float staffGripRotY = 0.0f;
    float staffGripRotZ = 0.0f;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, staffTextureID);

    glPushMatrix();
    glRotatef(180, 0, 1, 0);
    glTranslatef(0.0f, -19.5f, 20.0f);

    Vec3 centerLeft = leftHandModel.getCenter();
    centerLeft.x -= 1.25f;
    float scale = 3.0f;

    // Apply left hand shooting/pointing offsets
    glTranslatef(centerLeft.x * scale, centerLeft.y * scale, centerLeft.z * scale);
    animation.applyShootingAnimation();
    glRotatef(animation.layDownFactor * 45.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(animation.layDownFactor * 90.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(-centerLeft.x * scale, -centerLeft.y * scale, -centerLeft.z * scale);

    // Shift coordinate reference frame from right hand to left hand
    if (rightHandLoaded)
    {
        Vec3 centerRight = rightHandModel.getCenter();
        Vec3 shift;
        shift.x = centerLeft.x - centerRight.x;
        shift.y = centerLeft.y - centerRight.y;
        shift.z = centerLeft.z - centerRight.z;
        glTranslatef(shift.x * scale, shift.y * scale, shift.z * scale);
    }

    // Apply custom offsets, grip orientations, body breathing tilt, and scales
    glTranslatef(staffOffsetX, staffOffsetY, staffOffsetZ);
    glRotatef(staffGripRotX, 1.0f, 0.0f, 0.0f);
    glRotatef(staffGripRotY, 0.0f, 1.0f, 0.0f);
    glRotatef(staffGripRotZ, 0.0f, 0.0f, 1.0f);
    glRotatef(animation.bodyTiltAngle, 0.0f, 0.0f, 1.0f);
    glScalef(scale, scale, scale);

    glColor3f(1.0f, 1.0f, 1.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    staffModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/**
 * Render Caine's tongue, with mouth opening movement on death.
 */
void Caine::drawTongue() const
{
    if (!tongueLoaded) return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tongueTextureID);

    glPushMatrix();
    glRotatef(180, 0, 1, 0);
    glTranslatef(0.0f, -19.5f, 20.0f);
    
    // Slight tongue drop flat on death mouth opening (rotated around pitch axis)
    if (animation.isDead)
    {
        float deathFactor = animation.deathTimer;
        if (deathFactor > 1.0f) deathFactor = 1.0f;
        glRotatef(deathFactor * 15.0f, 0.0f, 1.0f, 0.0f);
    }

    glScalef(3.0f, 3.0f, 3.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    tongueModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/**
 * Render Caine's torso (Turso).
 */
void Caine::drawTurso() const
{
    if (!tursoLoaded) return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tursoTextureID);

    glPushMatrix();
    glRotatef(180, 0, 1, 0);
    glTranslatef(0.0f, -19.5f, 20.0f);
    glScalef(3.0f, 3.0f, 3.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    tursoModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/**
 * Render Caine's upper jaw, handling breathing TMJ movement, lean mouth open, and death mouth open.
 */
void Caine::drawUpperJaw() const
{
    if (!upperJawLoaded) return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, upperJawTextureID);

    glPushMatrix();
    glRotatef(180, 0, 1, 0);
    glTranslatef(0.0f, -19.5f, 20.0f);

    // Breathing mouth flapping counter-rotation
    glRotatef(animation.mouthOpenFactor * -3.0f, 1.0f, 0.0f, 0.0f);

    // Lean forward mouth open rotation
    glRotatef(animation.leanForwardFactor * 5.0f, 1.0f, 0.0f, 0.0f);

    // Death mouth open rotation (pitch axis)
    if (animation.isDead)
    {
        float deathFactor = animation.deathTimer;
        if (deathFactor > 1.0f) deathFactor = 1.0f;
        glRotatef(deathFactor * 15.0f, 0.0f, 1.0f, 0.0f);
    }

    glScalef(3.0f, 3.0f, 3.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    upperJawModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/**
 * Render Caine's right eye.
 */
void Caine::drawRightEye() const
{
    if (!rightEyeLoaded) return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, rightEyeTextureID);

    glPushMatrix();
    glRotatef(180, 0, 1, 0);
    glTranslatef(-0.5f, -9.0f, 20.0f);
    glScalef(4.0f, 4.0f, 4.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    rightEyeModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/**
 * Render Caine's left eye.
 */
void Caine::drawLeftEye() const
{
    if (!leftEyeLoaded) return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, leftEyeTextureID);

    glPushMatrix();
    glRotatef(180, 0, 1, 0);
    glTranslatef(-0.5f, -9.0f, 20.0f);
    glScalef(4.0f, 4.0f, 4.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    leftEyeModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

static void drawWireCube(float size)
{
    float h = size / 2.0f;
    glBegin(GL_LINE_LOOP);
        glVertex3f(-h,  h, -h);
        glVertex3f( h,  h, -h);
        glVertex3f( h,  h,  h);
        glVertex3f(-h,  h,  h);
    glEnd();

    glBegin(GL_LINE_LOOP);
        glVertex3f(-h, -h, -h);
        glVertex3f( h, -h, -h);
        glVertex3f( h, -h,  h);
        glVertex3f(-h, -h,  h);
    glEnd();

    glBegin(GL_LINES);
        glVertex3f(-h,  h, -h);
        glVertex3f(-h, -h, -h);

        glVertex3f( h,  h, -h);
        glVertex3f( h, -h, -h);

        glVertex3f( h,  h,  h);
        glVertex3f( h, -h,  h);

        glVertex3f(-h,  h,  h);
        glVertex3f(-h, -h,  h);
    glEnd();
}

/**
 * Render the entire Caine character, nesting all components under world space and posture transforms.
 */
void Caine::draw() const
{
    // Disappear completely after 1 second of dying
    if (animation.isDead && animation.deathTimer >= 1.0f)
        return;

    bool shouldRender = true;
    if (animation.isHurt)
    {
        // Toggle visibility every 0.05 seconds (10 Hz blink rate)
        if (std::fmod(animation.hurtTimer, 0.1f) < 0.05f)
        {
            shouldRender = false;
        }
    }

    if (shouldRender)
    {
        glPushMatrix();
        
        // 1. Move to world space coordinates
        glTranslatef(posX, posY, posZ);

        // Apply facing yaw so Caine faces the player
        glRotatef(facingYaw, 0.0f, 1.0f, 0.0f);

        // 2. Apply breathing hover offset
        glTranslatef(0.0f, animation.hoverOffset, 0.0f);

        // 3. Shift Caine upwards when laying down to keep above ground, and shift right/up to stay centered in hitbox
        glTranslatef(animation.layDownFactor * CAINE_LAYDOWN_SHIFT_X, 
                     animation.layDownFactor * (20.0f + CAINE_LAYDOWN_SHIFT_Y), 
                     0.0f);

        // 4. Transform to Caine's posture pivot center, apply posture rotations, scale, then translate back
        glTranslatef(0.0f, -19.5f, -20.0f);
        glRotatef(animation.bodyTiltAngle, 0.0f, 0.0f, 1.0f);          // Idle breathing body tilt
        glRotatef(animation.layDownFactor * 70.0f, 0.0f, 0.0f, 1.0f);    // Lay down transition rotation
        glRotatef(animation.leanForwardFactor * 45.0f, 1.0f, 0.0f, 0.0f); // Lean forward transition rotation
        float currentScale = uniformScale * visualScaleFactor;
        glScalef(currentScale, currentScale, currentScale);
        glTranslatef(0.0f, 19.5f, 20.0f);

        if (animation.isHurt || animation.isDead)
        {
            glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT);
            glDisable(GL_TEXTURE_2D);

            GLfloat redColor[]   = { 1.0f, 0.0f, 0.0f, 1.0f };
            GLfloat blackColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, redColor);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redColor);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, redColor);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blackColor);

            glColor3f(1.0f, 0.0f, 0.0f);
        }

        // 5. Draw the lower body limbs and torso
        drawTurso();
        drawLeftHand();
        drawRightHand();
        drawLeftPalm();
        drawRightPalm();
        drawLeftLeg();
        drawRightLeg();
        drawStaff();

        // 6. Draw the head parts grouped under a death tilt transform
        glPushMatrix();
        if (animation.isDead)
        {
            float deathFactor = animation.deathTimer;
            if (deathFactor > 1.0f) deathFactor = 1.0f;
            // Symmetrically rotate the head around local neck pivot
            glTranslatef(0.0f, 3.0f, -20.0f);
            glRotatef(deathFactor * 20.0f, 0.0f, 0.0f, 1.0f); // Tilt head to left
            glTranslatef(0.0f, -3.0f, 20.0f);
        }
        drawHat();
        drawUpperJaw();
        drawLowerJaw();
        drawTongue();
        drawRightEye();
        drawLeftEye();
        glPopMatrix();

        if (animation.isHurt || animation.isDead)
        {
            glPopAttrib();
        }

        glPopMatrix();
    }

    // Draw active projectiles in world space
    for (int i = 0; i < MAX_CAINE_PROJECTILES; i++)
    {
        if (projectiles[i].active)
        {
            myvirtualworld.environment.drawThinGlitchLineEffect(
                projectiles[i].posX, 
                projectiles[i].posY, 
                projectiles[i].posZ, 
                35.0f, 
                1.0f,
                projectiles[i].dirX,
                projectiles[i].dirY,
                projectiles[i].dirZ
            );
        }
    }

    // Draw active teleport particles in world space
    glPushAttrib(GL_LIGHTING_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    for (int i = 0; i < MAX_TELEPORT_PARTICLES; i++)
    {
        if (teleportParticles[i].active)
        {
            glPushMatrix();
            glTranslatef(teleportParticles[i].posX, teleportParticles[i].posY, teleportParticles[i].posZ);
            
            // Spin particles for a dynamic look
            float spin = teleportParticles[i].lifeTime * 250.0f;
            glRotatef(spin, 1.0f, 1.0f, 0.0f);
            
            glColor4f(teleportParticles[i].r, teleportParticles[i].g, teleportParticles[i].b, teleportParticles[i].alpha);
            
            // Draw a solid cube
            float s = teleportParticles[i].size * 0.5f;
            glBegin(GL_QUADS);
                // Front
                glVertex3f(-s, -s,  s); glVertex3f( s, -s,  s); glVertex3f( s,  s,  s); glVertex3f(-s,  s,  s);
                // Back
                glVertex3f(-s, -s, -s); glVertex3f(-s,  s, -s); glVertex3f( s,  s, -s); glVertex3f( s, -s, -s);
                // Top
                glVertex3f(-s,  s, -s); glVertex3f(-s,  s,  s); glVertex3f( s,  s,  s); glVertex3f( s,  s, -s);
                // Bottom
                glVertex3f(-s, -s, -s); glVertex3f( s, -s, -s); glVertex3f( s, -s,  s); glVertex3f(-s, -s,  s);
                // Right
                glVertex3f( s, -s, -s); glVertex3f( s,  s, -s); glVertex3f( s,  s,  s); glVertex3f( s, -s,  s);
                // Left
                glVertex3f(-s, -s, -s); glVertex3f(-s, -s,  s); glVertex3f(-s,  s,  s); glVertex3f(-s,  s, -s);
            glEnd();
            
            glPopMatrix();
        }
    }
    glPopAttrib();

    // Draw active sweep wall
    if (sweepActive)
    {
        float wallLength = 600.0f;
        int numPanels = 15;
        float step = wallLength / numPanels;
        float panelHalfWidth = step * 0.5f;
        float panelHalfHeight = 6.0f;
        float yCenter = -18.7f + panelHalfHeight; // sits on ground
        
        float rotationAngle = (sweepDirection == 0 || sweepDirection == 1) ? 0.0f : 90.0f;
        
        for (int i = 0; i < numPanels; i++)
        {
            float offset = -wallLength * 0.5f + i * step + panelHalfWidth;
            float px = 0.0f, py = yCenter, pz = 0.0f;
            if (rotationAngle == 0.0f)
            {
                px = offset;
                pz = sweepCurrentPos;
            }
            else
            {
                px = sweepCurrentPos;
                pz = offset;
            }
            
            float flicker = 0.5f + 0.5f * fabs(sin(myvirtualworld.environment.getAnimationTime() * 8.0f + i));
            float uShift = sin(myvirtualworld.environment.getAnimationTime() * 4.0f + i) * 0.2f;
            
            myvirtualworld.environment.drawGlitchPanelEffect(
                px, py, pz, 
                panelHalfWidth, panelHalfHeight, 
                uShift, 
                flicker, 
                rotationAngle
            );
        }
    }

    // Draw wireframe hitbox if showHitboxes is enabled
    if (showHitboxes && !animation.isDead)
    {
        glPushAttrib(GL_LIGHTING_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glLineWidth(2.0f);

        Vec3 center = getCaineWorldCenter();
        float boxSize = 24.0f * uniformScale;

        glPushMatrix();
        glTranslatef(center.x, center.y, center.z);
        glColor3f(0.0f, 1.0f, 0.0f); // Bright green hitbox outline
        drawWireCube(boxSize);
        glPopMatrix();

        glPopAttrib();
    }
}
