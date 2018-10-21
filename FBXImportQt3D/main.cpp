#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "fbxloader.h"
#include <QFileInfo>
#include <QDir>

#include <Qt3DQuickExtras/qt3dquickwindow.h>
#include <QGuiApplication>
#include <QResource>

#include <Qt3DCore/QEntity>
#include <Qt3DQuick/QQmlAspectEngine>
#include <QtQml>

#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QGeometryRenderer>


#include <Qt3DRender/QCamera>
#include <Qt3DRender/QCameraLens>
#include <Qt3DCore/QAspectEngine>

#include <Qt3DInput/QInputAspect>
#include <Qt3DRender/QRenderAspect>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QTorusMesh>

#include <qt3dextras/Qt3DWindow>
#include <qt3dextras/QOrbitCameraController>

Qt3DCore::QEntity* drawLine(QVector<float>& vertices, QVector<int>& indexes, const FBXLambertMat& lambert)
{
    Qt3DCore::QEntity *_rootEntity = new Qt3DCore::QEntity();

    auto *geometry = new Qt3DRender::QGeometry(_rootEntity);

    // position vertices (start and end)
    QByteArray bufferBytes
            = QByteArray::fromRawData(reinterpret_cast<char*>(vertices.data()), sizeof(float) * vertices.count());

    auto *buf = new Qt3DRender::QBuffer(geometry);
    buf->setData(bufferBytes);

    auto *positionAttribute = new Qt3DRender::QAttribute(geometry);
    positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
    positionAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    positionAttribute->setVertexSize(3);
    positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(buf);
    positionAttribute->setByteStride(3 * sizeof(float));
    positionAttribute->setCount(vertices.count());
    geometry->addAttribute(positionAttribute); // We add the vertices in the geometry

    // connectivity between vertices
    QByteArray indexBytes= QByteArray::fromRawData(reinterpret_cast<char*>(indexes.data()), sizeof(int) * indexes.count());

    auto *indexBuffer = new Qt3DRender::QBuffer(geometry);
    indexBuffer->setData(indexBytes);

    auto *indexAttribute = new Qt3DRender::QAttribute(geometry);
    indexAttribute->setVertexBaseType(Qt3DRender::QAttribute::UnsignedInt);
    indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexBuffer);
    indexAttribute->setCount(indexes.count());
    geometry->addAttribute(indexAttribute); // We add the indices linking the points in the geometry

    // mesh
    auto *line = new Qt3DRender::QGeometryRenderer(_rootEntity);
    line->setGeometry(geometry);
    line->setPrimitiveType(Qt3DRender::QGeometryRenderer::Triangles);

    auto *material = new Qt3DExtras::QPhongMaterial(_rootEntity);
    material->setAmbient(lambert.diffuse);

    // entity
    auto *lineEntity = new Qt3DCore::QEntity(_rootEntity);
    lineEntity->addComponent(line);
    lineEntity->addComponent(material);

    return _rootEntity;
}


int main(int argc, char* argv[])
{
    auto myDir = QFileInfo(argv[0]).dir();
    myDir.cdUp();
    myDir.cdUp();
    QString fbxFilePath = myDir.filePath("FBXImportQt3D\\sample2.fbx").replace("/", "\\");

    FBXLoader loader;
    loader.Load(fbxFilePath);

    QGuiApplication app(argc, argv);
    Qt3DExtras::Qt3DWindow view;


    Qt3DCore::QEntity *scene = drawLine(loader.vertices, loader.indexes, loader.lambert);

    // Camera
    Qt3DRender::QCamera *camera = view.camera();
    camera->lens()->setPerspectiveProjection(30.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(-5, 2, 0));
    camera->setViewCenter(QVector3D(0, 0, 0));

    // For camera controls
    Qt3DExtras::QOrbitCameraController *camController = new Qt3DExtras::QOrbitCameraController(scene);
    camController->setLinearSpeed( 50.0f );
    camController->setLookSpeed( 180.0f );
    camController->setCamera(camera);

    view.setRootEntity(scene);

    view.show();

    return app.exec();
}
