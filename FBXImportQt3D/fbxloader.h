#ifndef FBXLOADER_H
#define FBXLOADER_H

#include <fbxsdk.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QVector>
#include <QColor>

using namespace fbxsdk;

struct FBXLambertMat
{
    QColor ambient;
    QColor diffuse;
    QColor emissive;
    QColor bump;
    float transparency;
};

struct FBXPhongMat : public FBXLambertMat
{
    QColor specular;
    float shininess;
    float reflectivity;
};

class FBXLoader
{
public:
    FBXLoader();

    void Load(QString fbxFilePath);

    void DisplayContent(FbxScene* pScene);
    void DisplayContent(FbxNode* pNode);
    void DisplayMesh(FbxNode* pNode);
    void DisplayPosition(FbxMesh* mesh);

    QVector<float> vertices;
    QVector<int> indexes;

    FBXLambertMat lambert;

private:
    void GetMaterial(FbxNode* node, int materialIndex);
    void GetLambertMaterial(FbxSurfaceLambert* lambertMat);
    void GetPhongMaterial(FbxSurfacePhong* phongMat);

};

#endif // FBXLOADER_H
