#ifndef KINGER_HPP
#define KINGER_HPP

#include "ObjModel.hpp"
#include "KingerAnimation.hpp"
#include <string>

namespace ProjectKinger
{

class Kinger
{
private:
    ObjModel headModel;
    ObjModel headPieceModel;
    ObjModel leftEyeModel;
    ObjModel rightEyeNModel;
    ObjModel bodyModel;
    ObjModel clothModel;
    ObjModel leftHandModel;
    ObjModel rightHandwGunModel;
    ObjModel bucketModel;
    ObjModel bucketHandleModel;

    //ObjModel Model;

    bool headLoaded;
    bool headPieceLoaded;
    bool leftEyeLoaded;
    bool rightEyeNLoaded;
    bool bodyLoaded;
    bool clothLoaded;
    bool leftHandLoaded;
    bool rightHandwGunLoaded;
    bool bucketLoaded;
    bool bucketHandleLoaded;

    //bool Loaded;

public:
    Kinger();
    KingerAnimation animation;
    
    int currentHealth = 100;
    int maxHealth = 100;

    void update(float deltaTime, float cameraYaw, float cameraPitch, const bool* keyStates);
    
    float velocityY;
    bool isGrounded;
    float jumpScaleY; 
    void jump();
    void takeDamage(int amount);
    void rebirth();

    float aimPitch;

    float posX;
    float posY;
    float posZ;
    float facingYaw;

    static constexpr float MAX_LEAN_ANGLE = 15.0f; 
    float targetLeanPitch;
    float currentLeanPitch;
    float targetLeanRoll;
    float currentLeanRoll;

    void moveForward(float speed, float cameraYaw);
    void moveRight(float speed, float cameraYaw);

    GLuint headTextureID;
    GLuint headPieceTextureID;
    GLuint clothTextureID;
    GLuint rightHandwGunTextureID;
    GLuint leftEyeTextureID;
    GLuint rightEyeNTextureID;

    bool loadHead(const std::string& filePath);
    bool loadHeadPiece(const std::string& filePath);
    bool loadLeftEye(const std::string& filePath);
    bool loadRightEyeN(const std::string& filePath);
    bool loadBody(const std::string& filePath);
    bool loadCloth(const std::string& filePath);
    bool loadLeftHand(const std::string& filePath);
    bool loadRightHandwGun(const std::string& filePath);
    bool loadBucket(const std::string& filePath);
    bool loadBucketHandle(const std::string& filePath);
    //bool load(const std::string& filePath);

    void drawHead() const;
    void drawHeadPiece() const;
    void drawLeftEye() const;
    void drawRightEyeN() const;
    void drawBody() const;
    void drawCloth() const;
    void drawLeftHand() const;
    void drawRightHandwGun() const;
    void drawBucket() const;
    void drawBucketHandle() const;
    void drawBullet() const;
    void draw() const;


};

} // namespace ProjectKinger

#endif
