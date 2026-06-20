#ifndef CAINE_HPP
#define CAINE_HPP

#include <GL/glut.h>
#include <string>
#include "ObjModel.hpp"
#include "CaineAnimation.hpp"

namespace ProjectCaine
{

/**
 * Caine
 * Represents the Caine boss character, managing its models, textures, position, and drawing routines.
 * 
 * Handles loading of individual OBJ model parts, binding associated texture IDs, scaling, 
 * updating animation timelines, and executing hierarchical rendering of limbs, body, and head.
 */
class Caine
{
private:
    // OBJ Model Parts
    ObjModel hatModel;
    ObjModel leftHandModel;
    ObjModel leftLegModel;
    ObjModel leftPalmModel;
    ObjModel lowerJawModel;
    ObjModel rightHandModel;
    ObjModel rightLegModel;
    ObjModel rightPalmModel;
    ObjModel staffModel;
    ObjModel tongueModel;
    ObjModel tursoModel;
    ObjModel upperJawModel;
    ObjModel leftEyeModel;
    ObjModel rightEyeModel;

    // Loading Flags
    bool hatLoaded;
    bool leftHandLoaded;
    bool leftLegLoaded;
    bool leftPalmLoaded;
    bool lowerJawLoaded;
    bool rightHandLoaded;
    bool rightLegLoaded;
    bool rightPalmLoaded;
    bool staffLoaded;
    bool tongueLoaded;
    bool tursoLoaded;
    bool upperJawLoaded;
    bool leftEyeLoaded;
    bool rightEyeLoaded;

public:
    /**
     * Constructor that initializes position, spawn-tracking flags, and loading states to default.
     */
    Caine();

    /**
     * Updates the character's animation timelines, timers, and state transitions.
     * deltaTime The elapsed frame time in seconds.
     */
    void update(float deltaTime);

    CaineAnimation animation; // Animation state machine instance for this character

    float uniformScale; // Uniform scale multiplier for the entire 3D model representation
    float posX;         // Current X coordinate in 3D world space
    float posY;         // Current Y coordinate in 3D world space
    float posZ;         // Current Z coordinate in 3D world space

    float spawnX;              // Spawn point X coordinate (captured on first frame)
    float spawnY;              // Spawn point Y coordinate (captured on first frame)
    float spawnZ;              // Spawn point Z coordinate (captured on first frame)
    bool spawnPositionSaved;   // Flag tracking whether the spawn point has been successfully cached

    /**
     * Triggers the death sequence for Caine.
     * 
     * Resets active animation postures, snaps Caine to his initial spawn position, and sets the death timer.
     */
    void triggerDeath();

    /**
     * Triggers the hurt sequence for Caine, initiating the red flash effect.
     */
    void triggerHurt();

    // AI state variables
    float aiFlightTimer;         // time elapsed in current flight path
    float aiFlightDuration;      // duration to fly in current direction before picking new target
    float aiTargetX;             // flight target X
    float aiTargetZ;             // flight target Z
    float aiTeleportTimer;       // timer tracking time until next teleport
    float aiTeleportInterval;    // random interval between teleports
    bool isTeleporting;          // shrinking phase
    bool isAppearing;            // growing phase
    float teleportTransitionTimer; // timer for scale shrinking/growing
    float visualScaleFactor;     // dynamic scale factor multiplied during teleport
    float facingYaw;             // horizontal angle facing the player

    void resetAI();

    /**
     * Sets the uniform scale of the character model.
     * scale The scaling factor to apply uniformly.
     */
    void setScale(float scale);

    int currentHealth;
    int maxHealth;
    void takeDamage(int amount);
    Vec3 getCaineWorldCenter() const;

    // Texture Bindings
    GLuint hatTextureID;
    GLuint leftHandTextureID;
    GLuint leftLegTextureID;
    GLuint leftPalmTextureID;
    GLuint lowerJawTextureID;
    GLuint rightHandTextureID;
    GLuint rightLegTextureID;
    GLuint rightPalmTextureID;
    GLuint staffTextureID;
    GLuint tongueTextureID;
    GLuint tursoTextureID;
    GLuint upperJawTextureID;
    GLuint leftEyeTextureID;
    GLuint rightEyeTextureID;

    // OBJ File Loading Interface
    bool loadHat(const std::string& filePath);
    bool loadLeftHand(const std::string& filePath);
    bool loadLeftLeg(const std::string& filePath);
    bool loadLeftPalm(const std::string& filePath);
    bool loadLowerJaw(const std::string& filePath);
    bool loadRightHand(const std::string& filePath);
    bool loadRightLeg(const std::string& filePath);
    bool loadRightPalm(const std::string& filePath);
    bool loadStaff(const std::string& filePath);
    bool loadTongue(const std::string& filePath);
    bool loadTurso(const std::string& filePath);
    bool loadUpperJaw(const std::string& filePath);
    bool loadRightEye(const std::string& filePath);
    bool loadLeftEye(const std::string& filePath);

    // Nested Rendering Functions for Model Parts
    void drawHat() const;
    void drawLeftHand() const;
    void drawLeftLeg() const;
    void drawLeftPalm() const;
    void drawLowerJaw() const;
    void drawRightHand() const;
    void drawRightLeg() const;
    void drawRightPalm() const;
    void drawStaff() const;
    void drawTongue() const;
    void drawTurso() const;
    void drawUpperJaw() const;
    void drawRightEye() const;
    void drawLeftEye() const;

    /**
     * Renders the entire character model hierarchically.
     * 
     * Applies positioning translation, idle hovering offset, laying down transition,
     * leaning forward transition, death head tilting, and delegates drawing to the body/limb sub-functions.
     */
    void draw() const;
};

} // namespace ProjectCaine

#endif // CAINE_HPP
