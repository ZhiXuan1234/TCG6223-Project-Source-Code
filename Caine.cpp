#include <GL/glut.h>
#include <iostream>
#include "Caine.hpp"

using namespace ProjectCaine;

GLuint headTextureID;

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

bool Caine::loadHead(const std::string& filePath)
{
    headLoaded = headModel.loadFromObjText(filePath);
    return headLoaded;
}



void Caine::drawHead() const
{
    if (!headLoaded)
        return;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, headTextureID);

    glPushMatrix();
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


void Caine::draw() const
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
}
