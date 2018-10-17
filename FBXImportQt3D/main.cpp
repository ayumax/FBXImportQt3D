#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <fbxsdk.h>
#include <QFileInfo>
#include <QDir>

using namespace fbxsdk;

void DisplayContent(FbxScene* pScene);
void DisplayContent(FbxNode* pNode);
void DisplayMesh(FbxNode* pNode);
void DisplayIndex(FbxMesh* mesh);
void DisplayPosition(FbxMesh* mesh);


void DisplayContent(FbxScene* scene)
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

void DisplayContent(FbxNode* node)
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


void DisplayMesh(FbxNode* node)
{
    FbxMesh* mesh = (FbxMesh*)node->GetNodeAttribute();

    qDebug() << "\n\nMesh Name: " << (char *)node->GetName() ;

    DisplayPosition(mesh);

}


void DisplayPosition(FbxMesh* mesh)
{
    int positionNum = mesh->GetControlPointsCount();	// 頂点数
    FbxVector4* position = mesh->GetControlPoints();	// 頂点座標配列

    for (int i = 0; i < positionNum; ++i)
    {
        qDebug() << "position[" << i << "] : ("
            << position[i][0] << ","
            << position[i][1] << ","
            << position[i][2] << ","
            << position[i][3] << ")";
    }
}

int main(int argc, char *argv[])
{
    FbxManager* manager = FbxManager::Create();
    FbxIOSettings* ios = FbxIOSettings::Create(manager, IOSROOT);
    manager->SetIOSettings(ios);
    FbxScene* scene = FbxScene::Create(manager, "");

    auto myDir = QFileInfo(argv[0]).dir();
    myDir.cdUp();
    myDir.cdUp();
    QString _filePath = myDir.
            filePath("FBXImportQt3D\\sample.fbx")
            .replace("/", "\\");

    auto filePathStdString = _filePath.toStdString();
    const char* filename = filePathStdString.c_str();

    FbxImporter* importer = FbxImporter::Create(manager, "");
    if (!importer->Initialize(filename, -1, manager->GetIOSettings()))
    {
        return -1;
    }

    importer->Import(scene);
    importer->Destroy();


    FbxGeometryConverter geometryConverter(manager);
    geometryConverter.Triangulate(scene, true);

    DisplayContent(scene);

    manager->Destroy();




    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;



    return app.exec();
}
