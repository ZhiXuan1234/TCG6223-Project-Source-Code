#include <GL/glut.h>
#include <iostream>
#include "Kinger.hpp"

using namespace ProjectKinger;

GLuint headTextureID;

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
}

void Kinger::update(float deltaTime)
{
    animation.updateIdleState(deltaTime);
    animation.updateSkillState(deltaTime);
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
    glTranslatef(0.0f, animation.hoverOffset + animation.skillBodyYOffset, animation.skillBodyZOffset); //for animation y axis
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
    glRotatef(animation.armRotation, 1.0f, 0.0f, 0.0f);
    glRotatef(animation.skillArmRotation, 1.0f, 0.0f, 0.0f);
    glRotatef(180,1,0,0);
    glRotatef(180,0,0,1);
    //glTranslatef(0.0f, -18.0f, -5.0f);
    glTranslatef(0.0f, -18.0f, 0.0f);
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

    glPushMatrix();

    // Position bullet at the locked barrel height (bulletStartY) and
    // the current travel depth (bulletZOffset), centred on X.
    glTranslatef(-15.0f, animation.bulletStartY, animation.bulletZOffset+15);

    // Disable texturing so the emissive color is not tinted.
    glDisable(GL_TEXTURE_2D);

    // Bright emissive yellow-orange to read clearly against the scene.
    glColor3ub(255, 200, 0);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    glutSolidSphere(0.8, 10, 10);
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
}

void Kinger::draw() const
{

    drawHead();
    drawHeadPiece();
    drawLeftEye();
    drawRightEyeN();
    drawBody();
    drawCloth();
    drawLeftHand();
    drawRightHandwGun();
    drawBucket();
    drawBucketHandle();
    drawBullet();   // Rendered last; independent from all body-part transforms
}
