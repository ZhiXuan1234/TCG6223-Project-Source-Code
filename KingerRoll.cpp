#include <GL/glut.h>
#include "KingerRoll.hpp"

using namespace ProjectKingerRoll;

KingerRoll::KingerRoll()
{
    rollLoaded = false;
    rollTextureID = 0;
}

bool KingerRoll::loadRoll(const std::string& filePath)
{
    rollLoaded = rollModel.loadFromObjText(filePath);
    return rollLoaded;
}

void KingerRoll::drawRoll() const
{
    if (!rollLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, rollTextureID);

    glPushMatrix();

    // Change these values if Kinger_Roll appears too big / small / far

    glTranslatef(-30.0f, -19.3f, 0.0f);
    glRotatef(180, 0, 1, 0);
    glScalef(5.0f, 5.0f, 5.0f);

    glColor3f(1.0f, 1.0f, 1.0f);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    rollModel.draw();

    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void KingerRoll::draw() const
{
    drawRoll();
}
