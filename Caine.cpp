#include <GL/glut.h>
#include <iostream>
#include "Caine.hpp"

using namespace ProjectCaine;



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

void Caine::drawHat() const
{
    if (!hatLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, hatTextureID);

    glPushMatrix();
    glTranslatef(0.0f, 15.0f, 0.0f);


    glColor3f(1.0f, 1.0f, 1.0f);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    hatModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Caine::drawLeftHand() const
{
    if (!leftHandLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, leftHandTextureID);

    glPushMatrix();
    glTranslatef(0.0f, 15.0f, 0.0f);


    glColor3f(1.0f, 1.0f, 1.0f);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    leftHandModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Caine::drawLeftLeg() const
{
    if (!leftLegLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, leftLegTextureID);

    glPushMatrix();
    glTranslatef(0.0f, 15.0f, 0.0f);


    glColor3f(1.0f, 1.0f, 1.0f);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    leftLegModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Caine::drawLeftPalm() const
{
    if (!leftPalmLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, leftPalmTextureID);

    glPushMatrix();
    glTranslatef(0.0f, 15.0f, 0.0f);


    glColor3f(1.0f, 1.0f, 1.0f);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    leftPalmModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Caine::drawLowerJaw() const
{
    if (!lowerJawLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, lowerJawTextureID);

    glPushMatrix();
    glTranslatef(0.0f, 15.0f, 0.0f);


    glColor3f(1.0f, 1.0f, 1.0f);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    lowerJawModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Caine::drawRightHand() const
{
    if (!rightHandLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, rightHandTextureID);

    glPushMatrix();
    glTranslatef(0.0f, 15.0f, 0.0f);


    glColor3f(1.0f, 1.0f, 1.0f);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    rightHandModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Caine::drawRightLeg() const
{
    if (!rightLegLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, rightLegTextureID);

    glPushMatrix();
    glTranslatef(0.0f, 15.0f, 0.0f);


    glColor3f(1.0f, 1.0f, 1.0f);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    rightLegModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Caine::drawRightPalm() const
{
    if (!rightPalmLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, rightPalmTextureID);

    glPushMatrix();
    glTranslatef(0.0f, 15.0f, 0.0f);


    glColor3f(1.0f, 1.0f, 1.0f);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    rightPalmModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Caine::drawStaff() const
{
    if (!staffLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, staffTextureID);

    glPushMatrix();
    glTranslatef(0.0f, 15.0f, 0.0f);


    glColor3f(1.0f, 1.0f, 1.0f);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    staffModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Caine::drawTongue() const
{
    if (!tongueLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tongueTextureID);

    glPushMatrix();
    glTranslatef(0.0f, 15.0f, 0.0f);


    glColor3f(1.0f, 1.0f, 1.0f);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    tongueModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Caine::drawTurso() const
{
    if (!tursoLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tursoTextureID);

    glPushMatrix();
    glTranslatef(0.0f, 15.0f, 0.0f);


    glColor3f(1.0f, 1.0f, 1.0f);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    tursoModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Caine::drawUpperJaw() const
{
    if (!upperJawLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, upperJawTextureID);

    glPushMatrix();
    glTranslatef(0.0f, 15.0f, 0.0f);


    glColor3f(1.0f, 1.0f, 1.0f);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    upperJawModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Caine::drawRightEye() const
{
    if (!rightEyeLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, rightEyeTextureID);

    glPushMatrix();
    glTranslatef(0.0f, 15.0f, 0.0f);


    glColor3f(1.0f, 1.0f, 1.0f);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    rightEyeModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Caine::drawLeftEye() const
{
    if (!leftEyeLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, leftEyeTextureID);

    glPushMatrix();
    glTranslatef(0.0f, 15.0f, 0.0f);


    glColor3f(1.0f, 1.0f, 1.0f);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    leftEyeModel.draw();
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Caine::draw() const
{

    drawHat();
    drawUpperJaw();
    drawLowerJaw();
    drawTongue();
    drawTurso();
    drawLeftHand();
    drawRightHand();
    drawLeftPalm();
    drawRightPalm();
    drawLeftLeg();
    drawRightLeg();
    drawStaff();
    drawRightEye();
    drawLeftEye();
}
