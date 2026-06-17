#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include "Kinger.hpp"
#include "CNAWorld.hpp"

// Global scope reference to the main world instance
extern ProjectWorld::MyVirtualWorld myvirtualworld;

using namespace ProjectKinger;

GLuint headTextureID;
static void drawSolidCubeFallback(float size)
{
    float h = size / 2.0f;

    glBegin(GL_QUADS);

    // Front
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-h, -h, h);
    glVertex3f( h, -h, h);
    glVertex3f( h,  h, h);
    glVertex3f(-h,  h, h);

    // Back
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f( h, -h, -h);
    glVertex3f(-h, -h, -h);
    glVertex3f(-h,  h, -h);
    glVertex3f( h,  h, -h);

    // Left
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-h, -h, -h);
    glVertex3f(-h, -h,  h);
    glVertex3f(-h,  h,  h);
    glVertex3f(-h,  h, -h);

    // Right
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(h, -h,  h);
    glVertex3f(h, -h, -h);
    glVertex3f(h,  h, -h);
    glVertex3f(h,  h,  h);

    // Top
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-h, h,  h);
    glVertex3f( h, h,  h);
    glVertex3f( h, h, -h);
    glVertex3f(-h, h, -h);

    // Bottom
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-h, -h, -h);
    glVertex3f( h, -h, -h);
    glVertex3f( h, -h,  h);
    glVertex3f(-h, -h,  h);

    glEnd();
}

Kinger::Kinger()
{
    headLoaded = false;
    headPieceLoaded = false;
    leftEyeLoaded = false;
    rightEyeNLoaded = false;
    bodyLoaded = false;
    clothLoaded = false;
    leftHandLoaded = false;
    rightHandwGunLoaded = false;
    bucketLoaded = false;
    bucketHandleLoaded = false;

    posX      = 0.0f;
    posY      = -18.7f; 
    posZ      = 0.0f;
    facingYaw = 0.0f;

    targetLeanPitch  = 0.0f;
    currentLeanPitch = 0.0f;
    targetLeanRoll   = 0.0f;
    currentLeanRoll  = 0.0f;

    velocityY = 0.0f;
    isGrounded = true;
    jumpScaleY = 1.0f;
}

void Kinger::jump()
{

    if (animation.isHealing || animation.isDead) return;

    if (isGrounded)
    {
        velocityY = 50.0f; 
        isGrounded = false;
        jumpScaleY = 1.2f; 
    }
}

void Kinger::takeDamage(int amount)
{
    if (animation.isHurt || animation.isRolling || animation.isDead) return; // Prevent damage while hurt, rolling, or dead

    currentHealth -= amount;
    if (currentHealth <= 0)
    {
        currentHealth = 0;
        animation.triggerDeath();
    }
    else
    {
        animation.triggerHurt();
    }
}

void Kinger::rebirth()
{
    currentHealth = maxHealth;
    posX = 0.0f;
    posY = -18.7f;
    posZ = 0.0f;
    facingYaw = 0.0f;
    velocityY = 0.0f;
    isGrounded = true;

    // Reset animation death state
    animation.isDead = false;
    animation.deathTimer = 0.0f;
}


void Kinger::update(float deltaTime, float cameraYaw, float cameraPitch, const bool* keyStates)
{
    if (animation.isDead)
    {
        animation.updateDeathState(deltaTime);
        // Apply gravity during death falling
        velocityY += -50.0f * deltaTime;
        posY += velocityY * deltaTime;
        if (posY <= -18.7f)
        {
            posY = -18.7f;
            velocityY = 0.0f;
        }

        if (animation.deathTimer >= RESPAWN_DELAY)
        {
            rebirth();
        }
        return;
    }

    facingYaw = cameraYaw;
    aimPitch = -cameraPitch;

    if (keyStates[1])
    {
        if (!animation.isCastingSkill)
        {
            animation.castGunSkill();
        }
    }

    animation.updateIdleState(deltaTime);

    animation.updateSkillState(deltaTime, cameraYaw, cameraPitch, posX, posY, posZ);
    animation.updateRollState(deltaTime);
    animation.updateReloadState(deltaTime);
    animation.updateHealState(deltaTime);
    animation.updateHurtState(deltaTime);


    if (!animation.isHealing)
    {
    float fwd = 0.0f;
    float rgt = 0.0f;

    if (keyStates['w'] || keyStates['W']) fwd += 1.0f;
    if (keyStates['s'] || keyStates['S']) fwd -= 1.0f;
    if (keyStates['d'] || keyStates['D']) rgt += 1.0f;
    if (keyStates['a'] || keyStates['A']) rgt -= 1.0f;

    float magnitude = std::sqrt(fwd * fwd + rgt * rgt);
    if (magnitude > 0.0f)
    {
        float KINGER_INTERNAL_SPEED = 45.0f; 

        if (animation.isRolling)
        {
            const float ROLL_SPEED_MULTIPLIER = 2.5f;
            KINGER_INTERNAL_SPEED *= ROLL_SPEED_MULTIPLIER;
        }

        if (animation.isHurt)
        {
            const float HURT_SPEED_MULTIPLIER = 0.5f;
            KINGER_INTERNAL_SPEED *= HURT_SPEED_MULTIPLIER;
        }

        float step = KINGER_INTERNAL_SPEED * deltaTime;

        float normFwd = fwd / magnitude;
        float normRgt = rgt / magnitude;

        posX += step * (normFwd * -std::sin(cameraYaw) + normRgt * std::cos(cameraYaw));
        posZ += step * (normFwd * -std::cos(cameraYaw) + normRgt * -std::sin(cameraYaw));
    }

    if (keyStates['w'] || keyStates['W']) {
        targetLeanPitch = MAX_LEAN_ANGLE;
    } else if (keyStates['s'] || keyStates['S']) {
        targetLeanPitch = -MAX_LEAN_ANGLE;
    } else {
        targetLeanPitch = 0.0f;
    }

    if (keyStates['a'] || keyStates['A']) {
        targetLeanRoll = -MAX_LEAN_ANGLE;
    } else if (keyStates['d'] || keyStates['D']) {
        targetLeanRoll = MAX_LEAN_ANGLE;
    } else {
        targetLeanRoll = 0.0f;
    }

    const float LEAN_SPEED = 3.0f;
    currentLeanPitch += (targetLeanPitch - currentLeanPitch) * LEAN_SPEED * deltaTime;
    currentLeanRoll  += (targetLeanRoll  - currentLeanRoll)  * LEAN_SPEED * deltaTime;
    }

    // Apply gravity constantly
    velocityY += -60.0f * deltaTime;

    posY += velocityY * deltaTime;

    if (posY <= -18.7f)
    {
        posY = -18.7f;
        velocityY = 0.0f;
        if (!isGrounded)
        {
            isGrounded = true;
            jumpScaleY = 0.8f;
        }
    }

    jumpScaleY += (1.0f - jumpScaleY) * 10.0f * deltaTime;
}

void Kinger::moveForward(float speed, float cameraYaw)
{
    posX -= speed * std::sin(cameraYaw);
    posZ -= speed * std::cos(cameraYaw);
}

void Kinger::moveRight(float speed, float cameraYaw)
{
    posX += speed * std::cos(cameraYaw);
    posZ -= speed * std::sin(cameraYaw);
}

bool Kinger::loadHead(const std::string& filePath)
{
    headLoaded = headModel.loadFromObjText(filePath);
    return headLoaded;
}

bool Kinger::loadHeadPiece(const std::string& filePath)
{
    headPieceLoaded = headPieceModel.loadFromObjText(filePath);
    return headPieceLoaded;
}

bool Kinger::loadLeftEye(const std::string& filePath)
{
    leftEyeLoaded = leftEyeModel.loadFromObjText(filePath);
    return leftEyeLoaded;
}

bool Kinger::loadRightEyeN(const std::string& filePath)
{
    rightEyeNLoaded = rightEyeNModel.loadFromObjText(filePath);
    return rightEyeNLoaded;
}

bool Kinger::loadBody(const std::string& filePath)
{
    bodyLoaded = bodyModel.loadFromObjText(filePath);
    return bodyLoaded;
}

bool Kinger::loadCloth(const std::string& filePath)
{
    clothLoaded = clothModel.loadFromObjText(filePath);
    return clothLoaded;
}

bool Kinger::loadLeftHand(const std::string& filePath)
{
    leftHandLoaded = leftHandModel.loadFromObjText(filePath);
    return leftHandLoaded;
}

bool Kinger::loadRightHandwGun(const std::string& filePath)
{
    rightHandwGunLoaded = rightHandwGunModel.loadFromObjText(filePath);
    return rightHandwGunLoaded;
}

bool Kinger::loadBucket(const std::string& filePath)
{
    bucketLoaded = bucketModel.loadFromObjText(filePath);
    return bucketLoaded;
}

bool Kinger::loadBucketHandle(const std::string& filePath)
{
    bucketHandleLoaded = bucketHandleModel.loadFromObjText(filePath);
    return bucketHandleLoaded;
}

void Kinger::drawHead() const
{
    if (!headLoaded)
        return;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, headTextureID);

    glPushMatrix();
    glTranslatef(0.0f, animation.hoverOffset + animation.skillBodyYOffset, animation.skillBodyZOffset);
    glTranslatef(0.0f, 15.0f, 0.0f);
    glScalef(1.6f, 2.6f, 0.9f);

    glColor3f(1.0f, 1.0f, 1.0f);
    //glColor3ub(254, 226, 205);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    headModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Kinger::drawHeadPiece() const
{
    if (!headPieceLoaded)
        return;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, headPieceTextureID);

    glPushMatrix();
    glTranslatef(0.0f, animation.hoverOffset + animation.skillBodyYOffset, animation.skillBodyZOffset);
    glTranslatef(0.0f, 14.5f, 0.0f);
    glScalef(1.2f, 2.0f, 0.7f);
    //glColor3ub(254, 226, 205);
    glColor3f(1.0f, 1.0f, 1.0f);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    headPieceModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Kinger::drawLeftEye() const
{
    if (!leftEyeLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, leftEyeTextureID);

    glPushMatrix();
    glTranslatef(0.0f, animation.hoverOffset + animation.skillBodyYOffset, animation.skillBodyZOffset);
    glTranslatef(6.0f, 28.0f, 0.0f);
    glScalef(8.0f, 8.0f, 8.0f);
    glRotatef(180,1,0,0);
    glColor3ub(255, 255, 255);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    leftEyeModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Kinger::drawRightEyeN() const
{
    if (!rightEyeNLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, rightEyeNTextureID);

    glPushMatrix();
    glTranslatef(0.0f, animation.hoverOffset + animation.skillBodyYOffset, animation.skillBodyZOffset);
    glRotatef(180,1,0,0);
    glTranslatef(-7.0f, -32.0f, 2.0f);
    glScalef(7.5f, 7.5f, 7.5f);

    glColor3ub(255, 255, 255);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    rightEyeNModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Kinger::drawBody() const
{
    if (!bodyLoaded)
        return;

    glPushMatrix();
    glTranslatef(0.0f, animation.hoverOffset + animation.skillBodyYOffset, animation.skillBodyZOffset);
    //glTranslatef(0.0f, 7.90f, 0.0f);
    glScalef(3.0f, 3.0f, 3.0f);
    glColor3ub(254, 226, 205);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    bodyModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
}

void Kinger::drawCloth() const
{
    if (!clothLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, clothTextureID);

    glPushMatrix();
    glTranslatef(0.0f, animation.hoverOffset + animation.skillBodyYOffset, animation.skillBodyZOffset);
    glRotatef(animation.clothRotation, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, -18.0f, 0.0f);
    glScalef(7.5f, 8.0f, 7.0f);
    //glColor3ub(128, 0, 128);
    glColor3f(1.0f, 1.0f, 1.0f);
    glRotatef(180,0,1,0);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    clothModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Kinger::drawLeftHand() const
{
    if (!leftHandLoaded)
        return;

    glPushMatrix();
    glTranslatef(0.0f, animation.hoverOffset + animation.skillBodyYOffset + animation.leftArmReloadYOffset, animation.skillBodyZOffset); //for animation y axis

    glRotatef(animation.leftArmReloadYaw, 0.0f, 1.0f, 0.0f);
    glRotatef(animation.leftArmReloadPitch, 1.0f, 0.0f, 0.0f);

    glRotatef(animation.leftArmHealPitch, 1.0f, 0.0f, 0.0f);

    float visualPitch = aimPitch;
    if (visualPitch < -0.7f) visualPitch = -0.7f;
    if (visualPitch >  1.2f) visualPitch =  1.2f;
    float gunAimPitch = (-visualPitch * 57.2957795f);

    float aimBlend = (animation.leftArmReloadYaw / -135.0f);
    if (aimBlend < 0.0f) aimBlend = 0.0f;
    if (aimBlend > 1.0f) aimBlend = 1.0f;

    glRotatef(gunAimPitch * aimBlend, 1.0f, 0.0f, 0.0f);

    glRotatef(-animation.armRotation, 1.0f, 0.0f, 0.0f); 
    //glRotatef(180,0,0,1); //initial position rotation
    glRotatef(150,0,0,1); //adjust position rotation
    glRotatef(180,1,0,0);
    glTranslatef(0.0f, -18.0f, 0.0f);
    glScalef(7.0f, 7.5f, 7.0f);
    glColor3ub(255, 255, 255);


    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    leftHandModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
}

void Kinger::drawRightHandwGun() const
{
    if (!rightHandwGunLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, rightHandwGunTextureID);

    glPushMatrix();
    glTranslatef(0.0f, animation.hoverOffset + animation.skillBodyYOffset, animation.skillBodyZOffset);

    glRotatef(animation.rightArmReloadYaw, 0.0f, 1.0f, 0.0f);
    glRotatef(animation.rightArmReloadPitch, 1.0f, 0.0f, 0.0f);

    glTranslatef(0.0f, 0.0f, animation.armRecoilOffset);

    float visualPitch = aimPitch;
    if (visualPitch < -0.7f) visualPitch = -0.7f;
    if (visualPitch >  1.2f) visualPitch =  1.2f;

    float finalArmPitch = (-visualPitch * 57.2957795f) + animation.armRotation;
    glRotatef(finalArmPitch, 1.0f, 0.0f, 0.0f);

    glRotatef(180,1,0,0);
    glRotatef(180,0,0,1);
    glTranslatef(0.0f, -18.0f, -5.0f);
    glScalef(7.0f, 7.5f, 7.0f);
    glColor3ub(255, 255, 255);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    rightHandwGunModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Kinger::drawBucket() const
{
    if (!bucketLoaded)
        return;

    glPushMatrix();
    glTranslatef(0.0f, animation.hoverOffset + animation.skillBodyYOffset, animation.skillBodyZOffset);
    glRotatef(-25,1,0,0);
    glTranslatef(8.3f, 4.0f, 4.0f);

    glRotatef(180,0,0,1);

    glScalef(0.6f, 0.4f, 0.6f);
    glColor3ub(244, 244, 244);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    bucketModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
}

void Kinger::drawBucketHandle() const
{
    if (!bucketHandleLoaded)
        return;

    glPushMatrix();
    glTranslatef(0.0f, animation.hoverOffset + animation.skillBodyYOffset, animation.skillBodyZOffset);
    glTranslatef(0.0f, 5.0f, 2.0f);
    glRotatef(90, 0, 1, 0);
    glRotatef(90, 0, 0, 1);
    glRotatef(-45, 0, 0, 1);
    glScalef(8.5f, 8.5f, 8.5f);
    //glColor3ub(254, 226, 205);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    bucketHandleModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
}

void Kinger::drawBullet() const
{
    if (!animation.isBulletActive)
        return;

    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(3.0f);

    glDisable(GL_TEXTURE_2D);

    glColor4f(1.0f, 0.8f, 0.0f, 0.6f);

    glBegin(GL_LINES);
        glVertex3f(animation.bulletStartX, animation.bulletStartY, animation.bulletStartZ);
        glVertex3f(animation.bulletPosX, animation.bulletPosY, animation.bulletPosZ);
    glEnd();

    glPopAttrib();

    glPushMatrix();

    glTranslatef(animation.bulletPosX, animation.bulletPosY, animation.bulletPosZ);

    glDisable(GL_TEXTURE_2D);

    glColor3ub(255, 200, 0);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    static GLUquadricObj* bulletQuadric = NULL;

    if (bulletQuadric == NULL)
    {
        bulletQuadric = gluNewQuadric();
        gluQuadricNormals(bulletQuadric, GLU_SMOOTH);
    }

    gluSphere(bulletQuadric, 0.3, 10, 10);

    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
}

void Kinger::draw() const
{
    glPushMatrix();

    glTranslatef(posX, posY + 18.7f, posZ);

    const float RAD_TO_DEG = 57.2957795f;

    const float MODEL_FACING_OFFSET = 3.14159265f;
    glRotatef((facingYaw + MODEL_FACING_OFFSET) * RAD_TO_DEG, 0.0f, 1.0f, 0.0f);

    // Apply Minecraft-style death animation: fall stiffly to the right side
    if (animation.isDead)
    {
        float progress = animation.deathTimer / DEATH_DURATION;
        if (progress > 1.0f) progress = 1.0f;
        float fallAngle = progress * 90.0f; // 0 to 90 degrees
        
        glTranslatef(0.0f, -18.7f, 0.0f);
        glRotatef(fallAngle, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.0f, 18.7f, 0.0f);
    }

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

        if (animation.showBallModel)
        {
            glPushMatrix();
            float rollSpin = animation.rollTimer * 1500.0f;
            glRotatef(rollSpin, 1.0f, 0.0f, 0.0f);

            ::myvirtualworld.kingerRoll.draw();
            glPopMatrix();
        }
        else
        {
            glPushMatrix();

            glScalef(1.0f, jumpScaleY * animation.rollSquashY, 1.0f);

            glRotatef(currentLeanPitch, 1.0f, 0.0f, 0.0f);

            drawHead();
            drawHeadPiece();
            drawLeftEye();
            drawRightEyeN();
            drawBody();
            drawCloth();
            drawBucket();
            drawBucketHandle();

            glPopMatrix();

            glPushMatrix();

            drawLeftHand();

            glRotatef(25, 1.0f, 0.0f, 0.0f);
            drawRightHandwGun();
            glRotatef(-25, 1.0f, 0.0f, 0.0f);

            glPopMatrix();
        }

        if (animation.isHurt || animation.isDead)
        {
            glPopAttrib();
        }
    }

    glPopMatrix();

    if (animation.isHealing)
    {
        glPushMatrix();

        const float HAND_OFFSET_X = -12.0f;
        const float HAND_OFFSET_Y = 15.0f;
        const float HAND_OFFSET_Z = -15.0f; 

        float hx = (HAND_OFFSET_X * std::cos(facingYaw)) + (HAND_OFFSET_Z * -std::sin(facingYaw));
        float hz = (HAND_OFFSET_X * -std::sin(facingYaw)) + (HAND_OFFSET_Z * -std::cos(facingYaw));

        float handWorldX = posX + hx;
        float handWorldY = posY + 18.7f + HAND_OFFSET_Y;
        float handWorldZ = posZ + hz;

        const float BUCKET_HEIGHT_OFFSET = 25.0f;
        float bucketWorldX = posX;
        float bucketWorldY = posY + 18.7f + BUCKET_HEIGHT_OFFSET;
        float bucketWorldZ = posZ;

        float flyX = handWorldX, flyY = handWorldY, flyZ = handWorldZ;
        float scale = 1.0f;
        float flapAngle = 0.0f;

        if (animation.healTimer <= 0.5f)
        {
            flapAngle = std::sin(animation.healTimer * 20.0f) * 30.0f;
        }
        else if (animation.healTimer <= 1.5f)
        {
            float t = (animation.healTimer - 0.5f) / 1.0f;
            flyX = handWorldX + (bucketWorldX - handWorldX) * t;
            flyY = handWorldY + (bucketWorldY - handWorldY) * t;
            flyZ = handWorldZ + (bucketWorldZ - handWorldZ) * t;
            flapAngle = std::sin(animation.healTimer * 20.0f) * 30.0f;
        }
        else
        {
            flyX = bucketWorldX;
            flyY = bucketWorldY;
            flyZ = bucketWorldZ;
            flapAngle = 0.0f;
            scale = 1.0f - ((animation.healTimer - 1.5f) / 0.5f);
            if (scale < 0.0f) scale = 0.0f;
        }

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);

        glPushMatrix();
        glTranslatef(flyX, flyY, flyZ);
        glScalef(scale, scale, scale);

        glPushMatrix();
        glScalef(0.3f, 0.3f, 0.3f);
        ::myvirtualworld.butterfly.draw(flapAngle);
        glPopMatrix();
        glPopMatrix();

        if (animation.healTimer >= 1.5f)
        {
            glPushMatrix();
            glTranslatef(bucketWorldX, bucketWorldY, bucketWorldZ);

            glColor3f(0.2f, 1.0f, 0.2f);

            float crossTimer = animation.healTimer - 1.5f;

            for (int i = 0; i < 3; i++)
            {
                glPushMatrix();

                float offsetTimer = crossTimer + (i * 0.1f);
                float height = offsetTimer * 15.0f;
                float orbitX = std::sin(offsetTimer * 10.0f + i) * 3.0f;
                float orbitZ = std::cos(offsetTimer * 10.0f + i) * 3.0f;

                glTranslatef(orbitX, height, orbitZ);

                glRotatef(offsetTimer * 300.0f, 0.0f, 1.0f, 0.0f);

                float crossScale = 1.0f - (height / 10.0f);
                if (crossScale < 0.0f) crossScale = 0.0f;
                crossScale *= 0.7f;
                glScalef(crossScale, crossScale, crossScale);

                glPushMatrix();
                glScalef(0.6f, 2.0f, 0.6f);
                drawSolidCubeFallback(1.0f);
                glPopMatrix();

                glPushMatrix();
                glScalef(2.0f, 0.6f, 0.6f);
                drawSolidCubeFallback(1.0f);
                glPopMatrix();

                glPopMatrix();
            }

            glPopMatrix();
        }

        glEnable(GL_LIGHTING);
        glPopMatrix();
    }

    glTranslatef(0.0f, 3.0f, 0.0f);
    drawBullet();
}
