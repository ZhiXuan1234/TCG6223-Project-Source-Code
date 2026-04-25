#include <GL/glut.h>
#include <iostream>
#include "Kinger.hpp"

using namespace ProjectKinger;

Kinger::Kinger()
{
    headLoaded = false;
    headPieceLoaded = false;
    leftEyeLoaded = false;
    rightEyeLoaded = false;
    bodyLoaded = false;
    clothLoaded = false;
    leftHandLoaded = false;
    rightHandwGunLoaded = false;
    bucketLoaded = false;
    bucketHandleLoaded = false;
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

bool Kinger::loadRightEye(const std::string& filePath)
{
    rightEyeLoaded = rightEyeModel.loadFromObjText(filePath);
    return rightEyeLoaded;
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

    glPushMatrix();
    glTranslatef(0.0f, 15.0f, 0.0f);
    glScalef(1.6f, 2.6f, 0.9f);
    glColor3ub(254, 226, 205);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    headModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
}

void Kinger::drawHeadPiece() const
{
    if (!headPieceLoaded)
        return;

    glPushMatrix();
    glTranslatef(0.0f, 14.5f, 0.0f);
    glScalef(1.2f, 2.0f, 0.7f);
    glColor3ub(254, 226, 205);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    headPieceModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
}

void Kinger::drawLeftEye() const
{
    if (!leftEyeLoaded)
        return;

    glPushMatrix();
    glTranslatef(2.0f, 4.0f, 4.0f);
    glScalef(2.0f, 2.0f, 2.0f);
    glRotatef(270,0,0,1);
    glRotatef(90,1,0,0);
    glColor3ub(255, 255, 255);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    leftEyeModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
}

void Kinger::drawRightEye() const
{
    if (!rightEyeLoaded)
        return;

    glPushMatrix();
    glTranslatef(-2.0f, 7.0f, 5.0f);
    glScalef(2.5f, 2.5f, 2.5f);
    glRotatef(270,0,0,1);
    glRotatef(90,1,0,0);
    glColor3ub(255, 255, 255);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    rightEyeModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
}

void Kinger::drawBody() const
{
    if (!bodyLoaded)
        return;

    glPushMatrix();
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

    glPushMatrix();
    glTranslatef(0.0f, -6.5f, 0.0f);
    glScalef(6.5f, 9.0f, 7.0f);
    glColor3ub(128, 0, 128);
    glRotatef(180,0,1,0);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    clothModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
}

void Kinger::drawLeftHand() const
{
    if (!leftHandLoaded)
        return;

    glPushMatrix();
    glRotatef(180,0,0,1);
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
    if (!bodyLoaded)
        return;

    glPushMatrix();
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
}

void Kinger::drawBucket() const
{
    if (!bucketLoaded)
        return;

    glPushMatrix();
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
    //glTranslatef(0.0f, 7.90f, 0.0f);
    glScalef(3.0f, 3.0f, 3.0f);
    glColor3ub(254, 226, 205);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    bucketHandleModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
}

void Kinger::draw() const
{
    drawHead();
    drawHeadPiece();
    drawLeftEye();
    drawRightEye();
    drawBody();
    drawCloth();

    //#Bugged (Fixing)
    drawLeftHand();
    drawRightHandwGun();
    drawBucket();
    //drawBucketHandle();
}
