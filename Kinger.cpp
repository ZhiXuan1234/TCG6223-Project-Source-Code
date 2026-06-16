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

    // World-space position: start at origin, at ground height
    posX      = 0.0f;
    posY      = -18.7f; // ground level — used as camera look-at target Y
    posZ      = 0.0f;
    facingYaw = 0.0f;   // radians, 0 = facing -Z (into the scene)

    // Task 1: Initialize lean variables
    targetLeanPitch  = 0.0f;
    currentLeanPitch = 0.0f;
    targetLeanRoll   = 0.0f;
    currentLeanRoll  = 0.0f;

    // Task 1: Initialize Jump Physics
    velocityY = 0.0f;
    isGrounded = true;
    jumpScaleY = 1.0f;
}

void Kinger::jump()
{
    // Task 1: Movement Lock
    if (animation.isHealing) return;

    if (isGrounded)
    {
        velocityY = 30.0f; // Initial jump impulse
        isGrounded = false;
        jumpScaleY = 1.2f; // Visual stretch upwards when leaving ground
    }
}

// ---------------------------------------------------------------------------
// update — called every frame from CNAWorld::tickTime().
//
// Movement is evaluated here rather than inside the keyboard callback so that
// multiple keys held simultaneously are all detected in the same frame.
//
// Diagonal normalisation:
//   When W+D are both held the raw direction vector is (forwardX+rightX,
//   forwardZ+rightZ).  Its magnitude is sqrt(2) ≈ 1.414, which would make
//   Kinger move 41% faster diagonally.  We normalise the vector to length 1
//   before multiplying by speed*deltaTime so all directions are equal.
// ---------------------------------------------------------------------------
void Kinger::update(float deltaTime, float cameraYaw, float cameraPitch, const bool* keyStates)
{
    // Always lock Kinger's rotation directly to the camera view.
    // This ensures smooth turning with the mouse rather than instant snap-turns on WASD.
    facingYaw = cameraYaw;
    aimPitch = -cameraPitch;

    // Auto-fire logic: if the user holds Left Click (mapped to index 1) and the previous shot finished, fire again!
    if (keyStates[1])
    {
        if (!animation.isCastingSkill)
        {
            animation.castGunSkill();
        }
    }

    // Bug Fix: Restore the Idle State Animation
    // The idle math (hovering, breathing) was not being ticked! We must run it continuously
    // so the sine/cosine waves accumulate time even when we are running around.
    animation.updateIdleState(deltaTime);

    // Tick the animation state machines (idle hover, gun skill, etc.)
    // Pass Kinger's exact world position (including jump height) and aiming angles to the skill state
    // so the bullet spawns and travels accurately in 3D space.
    animation.updateSkillState(deltaTime, cameraYaw, cameraPitch, posX, posY, posZ);
    animation.updateRollState(deltaTime);
    animation.updateReloadState(deltaTime);
    animation.updateHealState(deltaTime);

    // Task 1: Movement Lock
    if (!animation.isHealing)
    {
        // --- Accumulate raw input axes ---
    // Each held key contributes ±1 to the forward (fwd) and right (rgt) axes.
    float fwd = 0.0f;   // +1 = W (forward),  -1 = S (back)
    float rgt = 0.0f;   // +1 = D (right),    -1 = A (left)

    if (keyStates['w'] || keyStates['W']) fwd += 1.0f;
    if (keyStates['s'] || keyStates['S']) fwd -= 1.0f;
    if (keyStates['d'] || keyStates['D']) rgt += 1.0f;
    if (keyStates['a'] || keyStates['A']) rgt -= 1.0f;

    // --- Normalise to prevent diagonal speed boost ---
    float magnitude = std::sqrt(fwd * fwd + rgt * rgt);
    if (magnitude > 0.0f)
    {
        // Bring the vector to unit length so speed is identical in every
        // direction, then scale by KINGER_MOVE_SPEED * deltaTime.
        // We cannot reference the CNAmain constant here, so the caller
        // should bake it into deltaTime, OR we accept a speed parameter.
        // For simplicity we use a fixed internal speed constant.
        float KINGER_INTERNAL_SPEED = 45.0f; // world units per second

        // Task 4: Apply Speed Boost during Roll
        if (animation.isRolling)
        {
            const float ROLL_SPEED_MULTIPLIER = 2.5f;
            KINGER_INTERNAL_SPEED *= ROLL_SPEED_MULTIPLIER;
        }

        float step = KINGER_INTERNAL_SPEED * deltaTime;

        float normFwd = fwd / magnitude;
        float normRgt = rgt / magnitude;

        // Forward vector:  (-sin(yaw), 0, -cos(yaw))
        // Right   vector:  (+cos(yaw), 0, -sin(yaw))
        posX += step * (normFwd * -std::sin(cameraYaw) + normRgt * std::cos(cameraYaw));
        posZ += step * (normFwd * -std::cos(cameraYaw) + normRgt * -std::sin(cameraYaw));
    }

    // --- Task 2: Calculate Smooth Lean Logic ---
    // Pitch (Forward/Backward lean)
    if (keyStates['w'] || keyStates['W']) {
        targetLeanPitch = MAX_LEAN_ANGLE;
    } else if (keyStates['s'] || keyStates['S']) {
        targetLeanPitch = -MAX_LEAN_ANGLE;
    } else {
        targetLeanPitch = 0.0f;
    }

    // Roll (Left/Right strafe lean)
    if (keyStates['a'] || keyStates['A']) {
        targetLeanRoll = -MAX_LEAN_ANGLE;
    } else if (keyStates['d'] || keyStates['D']) {
        targetLeanRoll = MAX_LEAN_ANGLE;
    } else {
        targetLeanRoll = 0.0f;
    }

    // Smoothly interpolate (Lerp) the current lean towards the target
    const float LEAN_SPEED = 3.0f;
    currentLeanPitch += (targetLeanPitch - currentLeanPitch) * LEAN_SPEED * deltaTime;
    currentLeanRoll  += (targetLeanRoll  - currentLeanRoll)  * LEAN_SPEED * deltaTime;
    } // End Movement Lock

    // --- Task 2: Implement Gravity & Physics Update ---
    // Apply gravity constantly
    velocityY += -30.0f * deltaTime;

    // Update vertical position
    posY += velocityY * deltaTime;

    // Floor Collision
    if (posY <= -18.7f) // -18.7f is ground level
    {
        posY = -18.7f;
        velocityY = 0.0f;
        if (!isGrounded)
        {
            // Just landed!
            isGrounded = true;
            jumpScaleY = 0.8f; // Squash effect on landing
        }
    }

    // Smoothly recover jumpScaleY back to 1.0f (normal size)
    jumpScaleY += (1.0f - jumpScaleY) * 10.0f * deltaTime;
}

// ---------------------------------------------------------------------------
// moveForward — legacy single-key helper (kept for external callers).
// ---------------------------------------------------------------------------
void Kinger::moveForward(float speed, float cameraYaw)
{
    posX -= speed * std::sin(cameraYaw);
    posZ -= speed * std::cos(cameraYaw);
    // Removed facingYaw assignment to prevent snap-rotation
}

// ---------------------------------------------------------------------------
// moveRight — legacy single-key helper (kept for external callers).
//
// Right vector = cross( forward, up ) where up = (0,1,0):
//   rightX =  cos(cameraYaw)
//   rightZ = -sin(cameraYaw)
// ---------------------------------------------------------------------------
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

    glColor3f(1.0f, 1.0f, 1.0f);      // White allows texture to show fully
    //glColor3ub(254, 226, 205);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    headModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);          // Disable texturing for other parts
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

    // Task 4: Apply Left Arm Transformation
    // The left arm physically rotates across the chest during the reload state.
    glRotatef(animation.leftArmReloadYaw, 0.0f, 1.0f, 0.0f);
    glRotatef(animation.leftArmReloadPitch, 1.0f, 0.0f, 0.0f);

    // Task 2: Left Arm Heal Pitch
    glRotatef(animation.leftArmHealPitch, 1.0f, 0.0f, 0.0f);

    // Dynamically track the gun's vertical aim so the hands meet perfectly no matter where the camera looks!
    float visualPitch = aimPitch;
    if (visualPitch < -0.7f) visualPitch = -0.7f;
    if (visualPitch >  1.2f) visualPitch =  1.2f;
    float gunAimPitch = (-visualPitch * 57.2957795f);

    // We only want the left hand to track the gun's aim WHEN it is actively reloading.
    // The deeper into the reload (leftArmReloadYaw goes from 0 to -135), the more it tracks.
    float aimBlend = (animation.leftArmReloadYaw / -135.0f);
    if (aimBlend < 0.0f) aimBlend = 0.0f;
    if (aimBlend > 1.0f) aimBlend = 1.0f;

    glRotatef(gunAimPitch * aimBlend, 1.0f, 0.0f, 0.0f);

    glRotatef(-animation.armRotation, 1.0f, 0.0f, 0.0f); //for animation x axis
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

    // Task 4: Apply Right Arm Transformation (Reloading)
    // The right arm brings the gun inward toward the chest so the left hand can reach it.
    glRotatef(animation.rightArmReloadYaw, 0.0f, 1.0f, 0.0f);
    glRotatef(animation.rightArmReloadPitch, 1.0f, 0.0f, 0.0f);

    // Task 3: Apply the Horizontal Translation
    // Slide the arm straight back into Kinger's shoulder along the Z axis
    glTranslatef(0.0f, 0.0f, animation.armRecoilOffset);

    // Note: The old glRotatef(animation.skillArmRotation...) swing logic has been removed
    // to support the rapid linear recoil instead.

    // Visually aim the gun up or down to align with the camera pitch.
    float visualPitch = aimPitch;
    if (visualPitch < -0.7f) visualPitch = -0.7f;
    if (visualPitch >  1.2f) visualPitch =  1.2f;

    // Bug Fix: Combine the idle breathing sway WITH the aiming pitch.
    // Task 1 & 2: Isolating the Right Arm during combat.
    // Because animation.armRotation is mathematically scaled to 0.0f inside updateIdleState
    // when shooting, adding it here will effectively freeze the gun steady while still
    // allowing the rest of the body (hover, cloth) to flutter naturally!
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

    // --- Task 2: Create the Trajectory Render Function ---
    // Draw the glowing tracer line from the gun barrel to the bullet
    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(3.0f);

    // Disable texturing to draw a pure colored line
    glDisable(GL_TEXTURE_2D);

    // Bright yellow/orange with 0.6f alpha
    glColor4f(1.0f, 0.8f, 0.0f, 0.6f);

    glBegin(GL_LINES);
        // Start exactly where the bullet spawned (gun barrel tip)
        glVertex3f(animation.bulletStartX, animation.bulletStartY, animation.bulletStartZ);
        // End at the current bullet position
        glVertex3f(animation.bulletPosX, animation.bulletPosY, animation.bulletPosZ);
    glEnd();

    glPopAttrib();

    glPushMatrix();

    // Position bullet at the locked 3D world-space coordinates
    // (calculated continuously based on shootYaw and shootPitch)
    glTranslatef(animation.bulletPosX, animation.bulletPosY, animation.bulletPosZ);

    // Disable texturing so the emissive color is not tinted.
    glDisable(GL_TEXTURE_2D);

    // Bright emissive yellow-orange to read clearly against the scene.
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
    // Push a world-position + facing transform so that every part-draw
    // is expressed relative to Kinger's current arena position.
    glPushMatrix();

    // The individual parts encode their own Y offsets relative to the ground.
    // We add (posY + 18.7f) so that when Kinger jumps, the entire model moves upward!
    glTranslatef(posX, posY + 18.7f, posZ);

    // Convert facingYaw (radians) to degrees for glRotatef.
    // Rotating around Y makes Kinger turn left / right.
    const float RAD_TO_DEG = 57.2957795f; // 180 / PI

    // The Kinger mesh was built facing +Z (toward the camera at yaw = 0).
    // Adding MODEL_FACING_OFFSET (π rad = 180°) flips it so the model's
    // BACK faces the camera, which is the correct TPS default orientation.
    const float MODEL_FACING_OFFSET = 3.14159265f; // π radians = 180 degrees
    glRotatef((facingYaw + MODEL_FACING_OFFSET) * RAD_TO_DEG, 0.0f, 1.0f, 0.0f);

    // Task 3: Apply the Model Swap
    if (animation.showBallModel)
    {
        glPushMatrix();
        // Rotate rapidly along the X-axis to simulate rolling forward
        float rollSpin = animation.rollTimer * 1500.0f;
        glRotatef(rollSpin, 1.0f, 0.0f, 0.0f);

        // Draw the pre-loaded ball model from the global world instance
        ::myvirtualworld.kingerRoll.draw();
        glPopMatrix();
    }
    else
    {
        // =====================================================================
        // 1. LEANING BODY GROUP
        // =====================================================================
        // The body leans forward/backward based on movement momentum.
        glPushMatrix();

        // Apply the squash and stretch from jumping AND the dodge roll squash
        glScalef(1.0f, jumpScaleY * animation.rollSquashY, 1.0f);

        glRotatef(currentLeanPitch, 1.0f, 0.0f, 0.0f); // Lean forward/backward
        // Removed currentLeanRoll as requested to simplify the animation

        drawHead();
        drawHeadPiece();
        drawLeftEye();
        drawRightEyeN();
        drawBody();
        drawCloth();
        drawBucket();
        drawBucketHandle();

    // Removed the static butterfly rendering from inside the leaning body group

        glPopMatrix();

        // =====================================================================
        // 2. STEADY ARM GROUP
        // =====================================================================
        // The arms bypass the lean so the gun barrel stays locked to the crosshair.
        glPushMatrix();

        drawLeftHand();

        glRotatef(25, 1.0f, 0.0f, 0.0f);
        drawRightHandwGun();
        glRotatef(-25, 1.0f, 0.0f, 0.0f);

        glPopMatrix();
    }

    glPopMatrix();

    // --- Task 3: Dynamic Butterfly Trajectory ---
    if (animation.isHealing)
    {
        glPushMatrix();

        // 1. Hand Position (Phase 1 start)
        const float HAND_OFFSET_X = -12.0f; // Approximate left hand X
        const float HAND_OFFSET_Y = 15.0f;  // Arm raised height (above ground)
        const float HAND_OFFSET_Z = -15.0f; // Arm extended forward

        // Convert local to world based on Kinger's rotation
        float hx = (HAND_OFFSET_X * std::cos(facingYaw)) + (HAND_OFFSET_Z * -std::sin(facingYaw));
        float hz = (HAND_OFFSET_X * -std::sin(facingYaw)) + (HAND_OFFSET_Z * -std::cos(facingYaw));

        float handWorldX = posX + hx;
        float handWorldY = posY + 18.7f + HAND_OFFSET_Y;
        float handWorldZ = posZ + hz;

        // 2. Bucket Position (Target)
        const float BUCKET_HEIGHT_OFFSET = 25.0f;
        float bucketWorldX = posX;
        float bucketWorldY = posY + 18.7f + BUCKET_HEIGHT_OFFSET;
        float bucketWorldZ = posZ;

        // 3. Interpolate position based on phase
        float flyX = handWorldX, flyY = handWorldY, flyZ = handWorldZ;
        float scale = 1.0f;
        float flapAngle = 0.0f;

        if (animation.healTimer <= 0.5f)
        {
            // Phase 1: Locked to Hand
            flapAngle = std::sin(animation.healTimer * 20.0f) * 30.0f;
        }
        else if (animation.healTimer <= 1.5f)
        {
            // Phase 2: Lerp to Bucket
            // Smoothly transition the butterfly's XYZ coordinates from the Left Hand to the Bucket
            float t = (animation.healTimer - 0.5f) / 1.0f;
            flyX = handWorldX + (bucketWorldX - handWorldX) * t;
            flyY = handWorldY + (bucketWorldY - handWorldY) * t;
            flyZ = handWorldZ + (bucketWorldZ - handWorldZ) * t;
            flapAngle = std::sin(animation.healTimer * 20.0f) * 30.0f;
        }
        else
        {
            // Phase 3: Resting on Bucket, fade out scale
            flyX = bucketWorldX;
            flyY = bucketWorldY;
            flyZ = bucketWorldZ;
            flapAngle = 0.0f; // Stop flapping
            scale = 1.0f - ((animation.healTimer - 1.5f) / 0.5f);
            if (scale < 0.0f) scale = 0.0f;
        }

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);

        // Draw the actual Butterfly Model
        glPushMatrix();
        glTranslatef(flyX, flyY, flyZ);
        glScalef(scale, scale, scale);

        glPushMatrix();
        glScalef(0.3f, 0.3f, 0.3f); // Base particle scale factor
        // Let the actual Butterfly model handle its own drawing and flapping!
        ::myvirtualworld.butterfly.draw(flapAngle);
        glPopMatrix();
        glPopMatrix(); // End Butterfly Transform

        // Draw High-Quality '+' Symbols ONLY when butterfly stops at the top (Phase 3)
        if (animation.healTimer >= 1.5f)
        {
            glPushMatrix();
            // Anchor the particle burst to the Bucket
            glTranslatef(bucketWorldX, bucketWorldY, bucketWorldZ);

            glColor3f(0.2f, 1.0f, 0.2f); // Vibrant light green

            // Timer strictly for the crosses (goes from 0.0 to 0.5s)
            float crossTimer = animation.healTimer - 1.5f;

            // Draw multiple floating 3D crosses to create a premium particle emitter effect
            for (int i = 0; i < 3; i++)
            {
                glPushMatrix();

                // Offset each cross in height and give it a slight orbit around the center
                float offsetTimer = crossTimer + (i * 0.1f);
                float height = offsetTimer * 15.0f; // Fast rise
                float orbitX = std::sin(offsetTimer * 10.0f + i) * 3.0f;
                float orbitZ = std::cos(offsetTimer * 10.0f + i) * 3.0f;

                glTranslatef(orbitX, height, orbitZ);

                // Slowly spin the crosses
                glRotatef(offsetTimer * 300.0f, 0.0f, 1.0f, 0.0f);

                // Fade out individually as they rise
                float crossScale = 1.0f - (height / 10.0f);
                if (crossScale < 0.0f) crossScale = 0.0f;
                // Base scale modifier
                crossScale *= 0.7f;
                glScalef(crossScale, crossScale, crossScale);

                // Build a thick, premium 3D plus sign using intersecting cubes
                // Vertical block
                glPushMatrix();
                glScalef(0.6f, 2.0f, 0.6f);
                drawSolidCubeFallback(1.0f);
                glPopMatrix();

                // Horizontal block
                glPushMatrix();
                glScalef(2.0f, 0.6f, 0.6f);
                drawSolidCubeFallback(1.0f);
                glPopMatrix();

                glPopMatrix();
            }

            glPopMatrix(); // End '+' Burst Transform
        }

        glEnable(GL_LIGHTING);
        glPopMatrix();
    }

    glTranslatef(0.0f, 3.0f, 0.0f);
    drawBullet();
}
