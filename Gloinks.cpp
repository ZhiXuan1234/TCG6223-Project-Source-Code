#include <GL/glut.h>
#include <iostream>
#include "Gloinks.hpp"

using namespace ProjectGloinks;



Gloinks::Gloinks()
{
    gloinksBowlingPinLoaded = false;
    gloinksCircleLoaded = false;
    gloinksCubeLoaded = false;
    gloinksMoonLoaded = false;
    gloinksStarLoaded = false;
    gloinksTriangularLoaded = false;

}

bool Gloinks::loadGloinksBowlingPin(const std::string& filePath)
{
    gloinksBowlingPinLoaded = gloinksBowlingPinModel.loadFromObjText(filePath);
    return gloinksBowlingPinLoaded;
}

bool Gloinks::loadGloinksCircle(const std::string& filePath)
{
    gloinksCircleLoaded = gloinksCircleModel.loadFromObjText(filePath);
    return gloinksCircleLoaded;
}

bool Gloinks::loadGloinksCube(const std::string& filePath)
{
    gloinksCubeLoaded = gloinksCubeModel.loadFromObjText(filePath);
    return gloinksCubeLoaded;
}

bool Gloinks::loadGloinksMoon(const std::string& filePath)
{
    gloinksMoonLoaded = gloinksMoonModel.loadFromObjText(filePath);
    return gloinksMoonLoaded;
}

bool Gloinks::loadGloinksStar(const std::string& filePath)
{
    gloinksStarLoaded = gloinksStarModel.loadFromObjText(filePath);
    return gloinksStarLoaded;
}

bool Gloinks::loadGloinksTriangular(const std::string& filePath)
{
    gloinksTriangularLoaded = gloinksTriangularModel.loadFromObjText(filePath);
    return gloinksTriangularLoaded;
}

void Gloinks::drawGloinksBowlingPin() const
{
    if (!gloinksBowlingPinLoaded)
        return;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, BowlingPinTextureID);

    glPushMatrix();
        glDisable(GL_CULL_FACE);
            glEnable(GL_NORMALIZE);
                glTranslatef(0.0f, -19.0f, 20.0f);
                glScalef(4.0f, 4.0f, 4.0f);
                gloinksBowlingPinModel.draw();
            glDisable(GL_NORMALIZE);
        glEnable(GL_CULL_FACE);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);          // Disable texturing for other parts
}

void Gloinks::drawGloinksCirle() const
{
    if (!gloinksCircleLoaded)
        return;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, CircleTextureID);

    glPushMatrix();
        glDisable(GL_CULL_FACE);
            glEnable(GL_NORMALIZE);
            glTranslatef(0.0f, -19.0f, 20.0f);
                glScalef(4.0f, 4.0f, 4.0f);
                gloinksCircleModel.draw();
            glDisable(GL_NORMALIZE);
        glEnable(GL_CULL_FACE);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);          // Disable texturing for other parts
}

void Gloinks::drawGloinksCube() const
{
    if (!gloinksCubeLoaded)
        return;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, CubeTextureID);

    glPushMatrix();
        glDisable(GL_CULL_FACE);
            glEnable(GL_NORMALIZE);
            glTranslatef(0.0f, -19.0f, 20.0f);
                glScalef(4.0f, 4.0f, 4.0f);
                gloinksCubeModel.draw();
            glDisable(GL_NORMALIZE);
        glEnable(GL_CULL_FACE);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);          // Disable texturing for other parts
}

void Gloinks::drawGloinksMoon() const
{
    if (!gloinksMoonLoaded)
        return;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, MoonTextureID);

    glPushMatrix();
        glDisable(GL_CULL_FACE);
            glEnable(GL_NORMALIZE);
            glTranslatef(0.0f, -19.0f, 20.0f);
                glScalef(4.0f, 4.0f, 4.0f);
                gloinksMoonModel.draw();
            glDisable(GL_NORMALIZE);
        glEnable(GL_CULL_FACE);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);          // Disable texturing for other parts
}

void Gloinks::drawGloinksStar() const
{
    if (!gloinksStarLoaded)
        return;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, StarTextureID);

    glPushMatrix();
        glDisable(GL_CULL_FACE);
            glEnable(GL_NORMALIZE);
            glTranslatef(0.0f, -19.0f, 20.0f);
                glScalef(4.0f, 4.0f, 4.0f);
                gloinksStarModel.draw();
            glDisable(GL_NORMALIZE);
        glEnable(GL_CULL_FACE);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);          // Disable texturing for other parts
}

void Gloinks::drawGloinksTriangular() const
{
    if (!gloinksTriangularLoaded)
        return;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, TriangularTextureID);

    glPushMatrix();
        glDisable(GL_CULL_FACE);
            glEnable(GL_NORMALIZE);
            glTranslatef(0.0f, -19.0f, 20.0f);
                glScalef(4.0f, 4.0f, 4.0f);
                gloinksTriangularModel.draw();
            glDisable(GL_NORMALIZE);
        glEnable(GL_CULL_FACE);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);          // Disable texturing for other parts
}

void Gloinks::draw() const
{

    drawGloinksBowlingPin();
    drawGloinksCirle();
    drawGloinksCube();
    drawGloinksMoon();
    drawGloinksStar();
    drawGloinksTriangular();

}
