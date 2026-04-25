#ifndef KINGER_HPP
#define KINGER_HPP

#include "ObjModel.hpp"
#include <string>

namespace ProjectKinger
{

class Kinger
{
private:
    ObjModel headModel;
    ObjModel headPieceModel;
    ObjModel leftEyeModel;
    ObjModel rightEyeModel;
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
    bool rightEyeLoaded;
    bool bodyLoaded;
    bool clothLoaded;
    bool leftHandLoaded;
    bool rightHandwGunLoaded;
    bool bucketLoaded;
    bool bucketHandleLoaded;

    //bool Loaded;

public:
    Kinger();

    bool loadHead(const std::string& filePath);
    bool loadHeadPiece(const std::string& filePath);
    bool loadLeftEye(const std::string& filePath);
    bool loadRightEye(const std::string& filePath);
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
    void drawRightEye() const;
    void drawBody() const;
    void drawCloth() const;
    void drawLeftHand() const;
    void drawRightHandwGun() const;
    void drawBucket() const;
    void drawBucketHandle() const;
    void draw() const;
};

} // namespace ProjectKinger

#endif
