using System;
using System.Collections.Generic;
using System.Data.Odbc;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    //
    // Lights
    //
    public class Light : Component
    {
        
    }

    public class DirectionalLight : Light
    {
        public bool Enabled
        {
            get
            {
                return InternalCalls.Component_GetEnabled(Parent.entityID, "DirectionalLight");
            }

            set
            {
                InternalCalls.Component_SetEnabled(Parent.entityID, value, "DirectionalLight");
            }
        }
    }

    public class PointLight : Light
    {
        public bool Enabled
        {
            get
            {
                return InternalCalls.Component_GetEnabled(Parent.entityID, "PointLight");
            }

            set
            {
                InternalCalls.Component_SetEnabled(Parent.entityID, value, "PointLight");
            }
        }
    }

    public class SpotLight : Light
    {
        public bool Enabled
        {
            get
            {
                return InternalCalls.Component_GetEnabled(Parent.entityID, "SpotLight");
            }

            set
            {
                InternalCalls.Component_SetEnabled(Parent.entityID, value, "SpotLight");
            }
        }
    }

    //
    // GameCamera
    //
    public class GameCamera : Behaviour
    {
        public void SetFocusPosition(Vector3 position)
        {
            InternalCalls.GameCameraComponent_SetFocusPosition(Parent.entityID, position.x, position.y, position.z);
        }

        public void SetViewProjection(Matrix viewProjection)
        {
            InternalCalls.GameCameraComponent_SetViewProjection(
                Parent.entityID,
                viewProjection._11, viewProjection._12, viewProjection._13, viewProjection._14,
                viewProjection._21, viewProjection._22, viewProjection._23, viewProjection._24,
                viewProjection._31, viewProjection._32, viewProjection._33, viewProjection._34,
                viewProjection._41, viewProjection._42, viewProjection._43, viewProjection._44
            );
        }

        public void SetView(Matrix view)
        {
            InternalCalls.GameCameraComponent_SetView(
                Parent.entityID,
                view._11, view._12, view._13, view._14,
                view._21, view._22, view._23, view._24,
                view._31, view._32, view._33, view._34,
                view._41, view._42, view._43, view._44
            );
        }

        public void SetProjection(Matrix projection)
        {
            InternalCalls.GameCameraComponent_SetView(
                Parent.entityID,
                projection._11, projection._12, projection._13, projection._14,
                projection._21, projection._22, projection._23, projection._24,
                projection._31, projection._32, projection._33, projection._34,
                projection._41, projection._42, projection._43, projection._44
            );
        }

        public bool Enabled
        {
            get
            {
                return InternalCalls.Component_GetEnabled(Parent.entityID, "GameCamera");
            }

            set
            {
                InternalCalls.Component_SetEnabled(Parent.entityID, value, "GameCamera");
            }
        }

        //
        // Static functions.
        //
        public static Matrix ComputeView(Vector3 front, Vector3 localPosition)
        {
            Matrix view = Matrix.Identity;
            InternalCalls.GameCameraComponent_ComputeView(
                front.x, front.y, front.z,
                localPosition.x, localPosition.y, localPosition.z,
                out view._11, out view._12, out view._13, out view._14,
                out view._21, out view._22, out view._23, out view._24,
                out view._31, out view._32, out view._33, out view._34,
                out view._41, out view._42, out view._43, out view._44
            );

            return view;
        }

        public static Matrix ComputePerspectiveProjection(Perspective perspective)
        {
            Matrix projection = Matrix.Identity;

            InternalCalls.GameCameraComponent_ComputePerspectiveProjection(
                perspective.fovAngleY, perspective.aspectRatio, perspective.nearZ, perspective.farZ,
                out projection._11, out projection._12, out projection._13, out projection._14,
                out projection._21, out projection._22, out projection._23, out projection._24,
                out projection._31, out projection._32, out projection._33, out projection._34,
                out projection._41, out projection._42, out projection._43, out projection._44
            );

            return projection;
        }

        public static Matrix ComputeOrthographicProjection(Orthographic orthographic) 
        {
            Matrix projection = Matrix.Identity;

            InternalCalls.GameCameraComponent_ComputeOrthographicProjection(
                orthographic.viewSize.x, orthographic.viewSize.y, orthographic.nearZ, orthographic.farZ,
                out projection._11, out projection._12, out projection._13, out projection._14,
                out projection._21, out projection._22, out projection._23, out projection._24,
                out projection._31, out projection._32, out projection._33, out projection._34,
                out projection._41, out projection._42, out projection._43, out projection._44
            );

            return projection;
        }

        public static void UpdateMainCameraConstans(CameraConstants constants)
        {
            InternalCalls.GameCameraComponent_UpdateMainCameraConstants(
                constants.view._11, constants.view._12, constants.view._13, constants.view._14,
                constants.view._21, constants.view._22, constants.view._23, constants.view._24,
                constants.view._31, constants.view._32, constants.view._33, constants.view._34,
                constants.view._41, constants.view._42, constants.view._43, constants.view._44,

                constants.projection._11, constants.projection._12, constants.projection._13, constants.projection._14,
                constants.projection._21, constants.projection._22, constants.projection._23, constants.projection._24,
                constants.projection._31, constants.projection._32, constants.projection._33, constants.projection._34,
                constants.projection._41, constants.projection._42, constants.projection._43, constants.projection._44,
                
                constants.position.x, constants.position.y, constants.position.z, constants.position.w
            );
        }

        public static Matrix GetMainCameraOrientation()
        {
            Matrix orientation = new Matrix();

            InternalCalls.GameCamera_MainCamera_GetOrientation(
                out orientation._11, out orientation._12, out orientation._13, out orientation._14,
                out orientation._21, out orientation._22, out orientation._23, out orientation._24,
                out orientation._31, out orientation._32, out orientation._33, out orientation._34,
                out orientation._41, out orientation._42, out orientation._43, out orientation._44
                );

            return orientation;
        }
    }

    //
    // Renderer
    //

    public class Renderer : Component
    {

    }

    public class MeshRenderer : Renderer
    {
        public bool Enabled
        {
            get
            {
                return InternalCalls.Component_GetEnabled(Parent.entityID, "MeshRenderer");
            }

            set
            {
                InternalCalls.Component_SetEnabled(Parent.entityID, value, "MeshRenderer");
            }
        }
    }

    public class SkinnedMeshRenderer : Renderer
    {
        public bool Enabled
        {
            get
            {
                return InternalCalls.Component_GetEnabled(Parent.entityID, "SkinnedMeshRenderer");
            }

            set
            {
                InternalCalls.Component_SetEnabled(Parent.entityID, value, "SkinnedMeshRenderer");
            }
        }
    }

    //
    // UI
    //
    public class UIComponent : Behaviour
    {
        public UIText GetTextItem()
        {
            if (!IsText())
            {
                // TODO Output error log.
                return null;
            }

            return new UIText(Parent.entityID);
        }

        public UIImage GetImageItem()
        {
            if (!IsImage())
            {
                // TODO Output error log.
                return null;
            }

            return new UIImage(Parent.entityID);
        }

        public bool IsText()
        {
            return (GetItemType() == "Text");
        }

        public bool IsImage()
        {
            return (GetItemType() == "Image");
        }

        public bool Enabled
        {
            get
            {
                return InternalCalls.Component_GetEnabled(Parent.entityID, "UIComponent");
            }

            set
            {
                InternalCalls.Component_SetEnabled(Parent.entityID, value, "UIComponent");
            }
        }

        private string GetItemType()
        {
            return InternalCalls.UIComponent_GetItemType(Parent.entityID);
        }
    }
}
