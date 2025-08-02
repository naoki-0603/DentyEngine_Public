using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public class Component : Object
    {
        public GameObject Parent { get; internal set; }
        public string Name { get; internal set; }
    }

    public class Behaviour : Component
    {

    }

    public class PhysicsBehaviour : Behaviour
    {

    }

    public class MonoBehaviour : Behaviour
    {
        public virtual void OnCreated(uint entityID)
        {
            Parent = Scene.FindGameObjectByID(entityID);

        }

        public virtual void OnUpdate()
        {

        }

        public virtual void OnFixedUpdate()
        {

        }

        public virtual void OnLateUpdate()
        {

        }
        
        public virtual void OnEnabled()
        {

        }

        public virtual void OnDisabled()
        {

        }

        public virtual void OnDestroy()
        {

        }

        public virtual void OnGui()
        {

        }

        public virtual void OnCollisionEnter(uint collisionObjectID)
        {

        }

        public virtual void OnCollisionStay(uint collisionObjectID)
        {

        }

        public virtual void OnCollisionExit(uint collisionObjectID)
        {

        }

        public virtual void OnTriggerEnter(uint collisionObjectID)
        {

        }

        public virtual void OnTriggerStay(uint collisionObjectID)
        {

        }

        public virtual void OnTriggerExit(uint collisionObjectID)
        {

        }

        public virtual void OnRuntimeSceneChange()
        {

        }

        public virtual void OnRuntimeSceneChanged(string sceneName)
        {

        }

        public virtual void OnChangeToPlayState()
        {

        }

        public virtual void OnChangeToEditState()
        {

        }

        public void AddMonoBehaviour(MonoBehaviour mono, string name)
        {
            mono.Name = name;
            Parent.AddMonoBehaviour(mono);
        }
    }

    //
    // Transform
    // 
    public class Transform : Component
    {
        public Vector3 Position
        {
            get
            {
                Vector3 position = new Vector3();
                InternalCalls.TransformComponent_GetPosition(Parent.entityID, out position.x, out position.y, out position.z);

                return position;
            }
        }

        public Vector3 LocalPosition
        {
            get
            {
                Vector3 localPosition = new Vector3();

                InternalCalls.TransformComponent_GetLocalPosition(Parent.entityID, out localPosition.x, out localPosition.y, out localPosition.z);
                return localPosition;
            }

            set
            {
                InternalCalls.TransformComponent_SetLocalPosition(Parent.entityID, value.x, value.y, value.z);
            }
        }

        public Vector3 LocalEulerAngles
        {
            get
            {
                Vector3 localEulerAngles = new Vector3();

                InternalCalls.TransformComponent_GetLocalEulerAngles(Parent.entityID, out localEulerAngles.x, out localEulerAngles.y, out localEulerAngles.z);
                return localEulerAngles;
            }

            set
            {
                InternalCalls.TransformComponent_SetLocalEulerAngles(Parent.entityID, value.x, value.y, value.z);
            }
        }

        public Vector3 LocalScale
        {
            get
            {
                Vector3 localScale = new Vector3();

                InternalCalls.TransformComponent_GetLocalScale(Parent.entityID, out localScale.x, out localScale.y, out localScale.z);

                return localScale;
            }

            set
            {
                InternalCalls.TransformComponent_SetLocalScale(Parent.entityID, value.x, value.y, value.z);
            }
        }

        public Vector3 GetRight
        {
            get
            {
                Vector3 right = new Vector3();

                InternalCalls.TransformComponent_GetRight(Parent.entityID, out right.x, out right.y, out right.z);

                return right;
            }
        }

        public Vector3 GetUp
        {
            get
            {
                Vector3 up = new Vector3();

                InternalCalls.TransformComponent_GetUp(Parent.entityID, out up.x, out up.y, out up.z);

                return up;
            }
        }

        public Vector3 GetFront
        {
            get
            {
                Vector3 front = new Vector3();

                InternalCalls.TransformComponent_GetFront(Parent.entityID, out front.x, out front.y, out front.z);

                return front;
            }
        }

        public Matrix Local
        {
            get
            {
                Matrix local = new Matrix();

                InternalCalls.TransformComponent_GetLocal(
                    Parent.entityID,
                    out local._11, out local._12, out local._13, out local._14,
                    out local._21, out local._22, out local._23, out local._24,
                    out local._31, out local._32, out local._33, out local._34,
                    out local._41, out local._42, out local._43, out local._44
                );

                return local;
            }

            set
            {
                InternalCalls.TransformComponent_SetLocal(
                    Parent.entityID,
                    value._11, value._12, value._13, value._14,
                    value._21, value._22, value._23, value._24,
                    value._31, value._32, value._33, value._34,
                    value._41, value._42, value._43, value._44
                );
            }
        }

        public Matrix World
        {
            get
            {
                Matrix world = new Matrix();

                InternalCalls.TransformComponent_GetWorld(
                    Parent.entityID,
                    out world._11, out world._12, out world._13, out world._14,
                    out world._21, out world._22, out world._23, out world._24,
                    out world._31, out world._32, out world._33, out world._34,
                    out world._41, out world._42, out world._43, out world._44
                );

                return world;
            }

            set
            {
                InternalCalls.TransformComponent_SetWorld(
                    Parent.entityID,
                    value._11, value._12, value._13, value._14,
                    value._21, value._22, value._23, value._24,
                    value._31, value._32, value._33, value._34,
                    value._41, value._42, value._43, value._44
                );
            }
        }

        public bool Update
        {
            get
            {
                return InternalCalls.TransformComponent_GetUpdate(Parent.entityID);
            }

            set
            {
                InternalCalls.TransformComponent_SetUpdate(Parent.entityID, value);
            }
        }

        public bool Enabled
        {
            get
            {
                return InternalCalls.Component_GetEnabled(Parent.entityID, "Transform");
            }

            set
            {
                InternalCalls.Component_SetEnabled(Parent.entityID, value, "Transform");
            }
        }
    }
}
