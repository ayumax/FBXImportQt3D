import QtQuick 2.11
import QtQuick 2.2 as QQ2
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

import QtQml.Models 2.3


Entity {
    id: sceneRoot

    Camera {
        id: camera
        projectionType: CameraLens.PerspectiveProjection
        fieldOfView: 45
        aspectRatio: 16/9
        nearPlane : 0.1
        farPlane : 10000.0
        position: Qt.vector3d( -30.0, 10.0, 10 )
        upVector: Qt.vector3d( 0.0, 1.0, 0.0 )
        viewCenter: Qt.vector3d( 0.0, 0.0, 0.0 )
    }

    OrbitCameraController {
        camera: camera
    }

    components: [
        RenderSettings {
            activeFrameGraph: ForwardRenderer {
                clearColor: Qt.rgba(0, 0, 0, 1)
                camera: camera
            }
        },
        // Event Source will be set by the Qt3DQuickWindow
        InputSettings { },
        ObjectPicker{
          id: spherePicker
          onPressed:{
             myCar.notifyClick()
          }
        }

    ]


    Entity {
        objectName: targetObject
        components: [
            CuboidMesh {
                xExtent: 2000
                yExtent: 0.1
                zExtent: 500
            },
            PhongMaterial {
                ambient:"dimgray"
            },
            Transform {
                translation: Qt.vector3d(0, -1, 0)
            }
        ]
    }

}
