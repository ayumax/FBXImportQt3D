#include "fbxloader.h"

#include <QDebug>





FBXLoader::FBXLoader()
{

}


void FBXLoader::Load(QString fbxFilePath)
{
    FbxManager* manager = FbxManager::Create();
    FbxIOSettings* ios = FbxIOSettings::Create(manager, IOSROOT);
    manager->SetIOSettings(ios);
    FbxScene* scene = FbxScene::Create(manager, "");


    auto filePathStdString = fbxFilePath.toStdString();
    const char* filename = filePathStdString.c_str();

    FbxImporter* importer = FbxImporter::Create(manager, "");
    if (!importer->Initialize(filename, -1, manager->GetIOSettings()))
    {
        return;
    }

    importer->Import(scene);
    importer->Destroy();


    FbxGeometryConverter geometryConverter(manager);
    geometryConverter.Triangulate(scene, true);

    DisplayContent(scene);

    manager->Destroy();

}


void FBXLoader::DisplayContent(FbxScene* scene)
{
    FbxNode* node = scene->GetRootNode();

    if (node)
    {
        for (int i = 0; i < node->GetChildCount(); i++)
        {
            DisplayContent(node->GetChild(i));
        }
    }
}

void FBXLoader::DisplayContent(FbxNode* node)
{
    FbxNodeAttribute::EType lAttributeType;

    if (node->GetNodeAttribute() == NULL)
    {
        qDebug() << "NULL Node Attribute\n\n";
    }
    else
    {
        lAttributeType = (node->GetNodeAttribute()->GetAttributeType());

        switch (lAttributeType)
        {
        default:
            break;

        case FbxNodeAttribute::eMesh:
            DisplayMesh(node);
            break;
        }
    }


    for (int i = 0; i < node->GetChildCount(); i++)
    {
        DisplayContent(node->GetChild(i));
    }
}


void FBXLoader::DisplayMesh(FbxNode* node)
{
    auto materialCount = node->GetMaterialCount();
    for (int i = 0; i < materialCount; ++i)
    {
        GetMaterial(node, i);
    }

    FbxMesh* mesh = (FbxMesh*)node->GetNodeAttribute();

   // qDebug() << "\n\nMesh Name: " << (char *)node->GetName() ;

    DisplayPosition(mesh);

}

void FBXLoader::GetMaterial(FbxNode* node, int materialIndex)
{
    auto material = node->GetMaterial(materialIndex);

    // materialはKFbxSurfaceMaterialオブジェクト

    // LambertかPhongか
    if ( material->GetClassId().Is( FbxSurfaceLambert::ClassId ) )
    {

       // Lambertにダウンキャスト
       FbxSurfaceLambert* lambert = (FbxSurfaceLambert*)material;
       GetLambertMaterial(lambert);
    }
    else if ( material->GetClassId().Is( FbxSurfacePhong::ClassId ) )
    {

       // Phongにダウンキャスト
       FbxSurfacePhong* phong = (FbxSurfacePhong*)material;
    }
}



void FBXLoader::GetLambertMaterial(FbxSurfaceLambert* lambertMat)
{
    lambert.ambient = QColor(255 * (float)lambertMat->Ambient.Get()[0],
                    255 * (float)lambertMat->Ambient.Get()[1],
                    255 * (float)lambertMat->Ambient.Get()[2]);

    lambert.diffuse = QColor(255 * (float)lambertMat->Diffuse.Get()[0],
                    255 * (float)lambertMat->Diffuse.Get()[1],
                    255 * (float)lambertMat->Diffuse.Get()[2]);

    lambert.emissive = QColor(255 * (float)lambertMat->Emissive.Get()[0],
                    255 * (float)lambertMat->Emissive.Get()[1],
                    255 * (float)lambertMat->Emissive.Get()[2]);


    lambert.bump = QColor(255 * (float)lambertMat->Bump.Get()[0],
                    255 * (float)lambertMat->Bump.Get()[1],
                    255 * (float)lambertMat->Bump.Get()[2]);

    lambert.transparency = (float)lambertMat->TransparencyFactor.Get();
}

void FBXLoader::GetPhongMaterial(FbxSurfacePhong* phongMat)
{
    GetLambertMaterial(phongMat);

//    specular_.r = (float)phong->GetSpecularColor().Get()[ 0 ];
//    specular_.g = (float)phong->GetSpecularColor().Get()[ 1 ];
//    specular_.b = (float)phong->GetSpecularColor().Get()[ 2 ];

//    // 光沢
//    shininess_ = (float)phong->GetShininess().Get();

//    // 反射
//    reflectivity_ = (float)phong->GetReflectionFactor().Get();
}

void FBXLoader::DisplayPosition(FbxMesh* mesh)
{
    //総ポリゴン数
    int polygonNum = mesh->GetPolygonCount();

    //p個目のポリゴンへの処理
    for (int p = 0; p < polygonNum; ++p)
    {
        //p個目のポリゴンのn個目の頂点への処理
        for (int n = 0; n < 3; ++n)
        {
            int index = mesh->GetPolygonVertex(p, n);
            indexes.push_back(index);
          //  std::cout << "index[" << p+n <<"] : " << index << std::endl;
        }
    }

    int positionNum = mesh->GetControlPointsCount();	// 頂点数
    FbxVector4* position = mesh->GetControlPoints();	// 頂点座標配列

    for (int i = 0; i < positionNum; ++i)
    {
        vertices.push_back(position[i][0]);
        vertices.push_back(position[i][2]);
        vertices.push_back(position[i][1]);

//        qDebug() << "position[" << i << "] : ("
//            << position[i][0] << ","
//            << position[i][1] << ","
//            << position[i][2] << ","
//            << position[i][3] << ")";
    }
}
