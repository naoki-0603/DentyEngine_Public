using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public class Collider : PhysicsBehaviour
    {

    }

    public class BoxCollider : Collider
    {
        public bool Enabled
        {
            get
            {
                return InternalCalls.Component_GetEnabled(Parent.entityID, "BoxCollider");
            }

            set
            {
                InternalCalls.Component_SetEnabled(Parent.entityID, value, "BoxCollider");
            }
        }
    }

    public class SphereCollider : Collider
    {
        public bool Enabled
        {
            get
            {
                return InternalCalls.Component_GetEnabled(Parent.entityID, "SphereCollider");
            }

            set
            {
                InternalCalls.Component_SetEnabled(Parent.entityID, value, "SphereCollider");
            }
        }
    }

    public class CapsuleCollider : Collider
    {
        public bool Enabled
        {
            get
            {
                return InternalCalls.Component_GetEnabled(Parent.entityID, "CapsuleCollider");
            }

            set
            {
                InternalCalls.Component_SetEnabled(Parent.entityID, value, "CapsuleCollider");
            }
        }
    }

    public class MeshCollider : Collider
    {
        public bool Enabled
        {
            get
            {
                return InternalCalls.Component_GetEnabled(Parent.entityID, "MeshCollider");
            }

            set
            {
                InternalCalls.Component_SetEnabled(Parent.entityID, value, "MeshCollider");
            }
        }
    }

    public class ConvexMeshCollider : Collider
    {
        public bool Enabled
        {
            get
            {
                return InternalCalls.Component_GetEnabled(Parent.entityID, "ConvexMeshCollider");
            }

            set
            {
                InternalCalls.Component_SetEnabled(Parent.entityID, value, "ConvexMeshCollider");
            }
        }
    }

    public class ApproximationMeshCollider : Collider
    {
        public bool Enabled
        {
            get
            {
                return InternalCalls.Component_GetEnabled(Parent.entityID, "ApproximationMeshCollider");
            }

            set
            {
                InternalCalls.Component_SetEnabled(Parent.entityID, value, "ApproximationMeshCollider");
            }
        }
    }

    public class RigidBody : PhysicsBehaviour
    {
        public void SetPosition(Vector3 position)
        {
            InternalCalls.RigidBodyComponent_SetPosition(Parent.entityID, position.x, position.y, position.z);
        }

        public void EnableAngularVelocity()
        {
            InternalCalls.RigidBodyComponent_EnableAngularVelocity(Parent.entityID);
        }

        public void DisableAngularVelocity()
        {
            InternalCalls.RigidBodyComponent_DisableAngularVelocity(Parent.entityID);
        }

        public void AddForce(Vector3 velocity)
        {
            InternalCalls.RigidBodyComponent_AddForce(Parent.entityID, velocity.x, velocity.y, velocity.z);
        }

        public void AddImpulse(Vector3 impulse)
        {
            InternalCalls.RigidBodyComponent_AddImpulse(Parent.entityID, impulse.x, impulse.y, impulse.z);
        }

        public Vector3 LinearVelocity
        {
            get
            {
                Vector3 velocity = new Vector3();

                InternalCalls.RigidBodyComponent_GetLinearVelocity(
                    Parent.entityID, out velocity.x, out velocity.y, out velocity.z
                );

                return velocity;
            }

            set
            {
                InternalCalls.RigidBodyComponent_SetLinearVelocity(
                    Parent.entityID, value.x, value.y, value.z
                ); 
            }
        }

        public Vector3 Gravity
        {
            set
            {
                InternalCalls.RigidBodyComponent_SetGravity(
                    Parent.entityID, value.x, value.y, value.z
                );
            }
        }

        public bool Enabled
        {
            get
            {
                return InternalCalls.Component_GetEnabled(Parent.entityID, "RigidBody");
            }

            set
            {
                InternalCalls.Component_SetEnabled(Parent.entityID, value, "RigidBody");
            }
        }

        public Matrix RigidBodyWorld
        {
            get
            {
                Matrix world = new Matrix();

                InternalCalls.RigidBodyComponent_GetRigidBodyWorld(
                    Parent.entityID,
                    out world._11, out world._12, out world._13, out world._14,
                    out world._21, out world._22, out world._23, out world._24,
                    out world._31, out world._32, out world._33, out world._34,
                    out world._41, out world._42, out world._43, out world._44
                    );

                return world;
            }
        }
    }

    public class DynamicCharacterController : PhysicsBehaviour
    {
    }

    public class VehicleComponent : PhysicsBehaviour
    {
        //
        // New
        //
        public void SetActivate(bool forceActivate)
        {
            InternalCalls.VehicleComponent_SetActivate(forceActivate, Parent.entityID);
        }

        public void SetVelocity(Vector3 velocity)
        {
            InternalCalls.VehicleComponent_SetVelocity(
                velocity.x, velocity.y, velocity.z, Parent.entityID
                );
        }

        public Vector3 GetVelocity()
        {
            Vector3 velocity = new Vector3();

            InternalCalls.VehicleComponent_GetVelocity(
                out velocity.x, out velocity.y, out velocity.z, Parent.entityID
                );

            return velocity;
        }

        public void SetLinearVelocity(Vector3 linearVelocity)
        {
            InternalCalls.VehicleComponent_SetLinearVelocity(linearVelocity.x, linearVelocity.y, linearVelocity.z, Parent.entityID);
        }

        public Vector3 GetLinearVelocity()
        {
            Vector3 linearVelocity = new Vector3();

            InternalCalls.VehicleComponent_GetLinearVelocity(out linearVelocity.x, out linearVelocity.y, out linearVelocity.z, Parent.entityID);

            return linearVelocity;
        }

        public void ApplyCentralImpulse(Vector3 impulse)
        {
            InternalCalls.VehicleComponent_ApplyCentralImpulse(impulse.x, impulse.y, impulse.z, Parent.entityID);
        }

        public void ApplyForce(Vector3 force)
        {
            InternalCalls.VehicleComponent_ApplyForce(force.x, force.y, force.z, Parent.entityID);
        }

        public void SetGravity(Vector3 gravity)
        {
            InternalCalls.VehicleComponent_SetGravity(gravity.x, gravity.y, gravity.z, Parent.entityID);
        }

        public void SetFront(Vector3 front)
        {
            InternalCalls.VehicleComponent_SetFront(front.x, front.y, front.z, Parent.entityID);
        }

        public void SetOrientation(Matrix orientation)
        {
            InternalCalls.VehicleComponent_SetOrientation(
                orientation._11, orientation._12, orientation._13, orientation._14,
                orientation._21, orientation._22, orientation._23, orientation._24,
                orientation._31, orientation._32, orientation._33, orientation._34,
                Parent.entityID
                );
        }

        public Matrix GetOrientation()
        {
            Matrix orientation = new Matrix();

            InternalCalls.VehicleComponent_GetOrientation(
                out orientation._11, out orientation._12, out orientation._13, out orientation._14,
                out orientation._21, out orientation._22, out orientation._23, out orientation._24,
                out orientation._31, out orientation._32, out orientation._33, out orientation._34,
                Parent.entityID
                );

            return orientation;
        }

        public void SetPosition(Vector3 position)
        {
            InternalCalls.VehicleComponent_SetPosition(
                position.x, position.y, position.z, Parent.entityID
                );
        }

        public Vector3 GetPosition()
        {
            Vector3 position = new Vector3();

            InternalCalls.VehicleComponent_GetPosition(
                out position.x, out position.y, out position.z, Parent.entityID
                );

            return position;
        }

        public void SetMotionWorldTransform(Matrix transform)
        {
            InternalCalls.VehicleComponent_SetMotionWorldTransform(
                transform._11, transform._12, transform._13, transform._14,
                transform._21, transform._22, transform._23, transform._24,
                transform._31, transform._32, transform._33, transform._34,
                transform._41, transform._42, transform._43, transform._44,
                Parent.entityID
            );
        }

        public Matrix GetMotionWorldTransform()
        {
            Matrix transform = new Matrix();

            InternalCalls.VehicleComponent_GetMotionWorldTransform(
                out transform._11, out transform._12, out transform._13, out transform._14,
                out transform._21, out transform._22, out transform._23, out transform._24,
                out transform._31, out transform._32, out transform._33, out transform._34,
                out transform._41, out transform._42, out transform._43, out transform._44,
                Parent.entityID
            );

            return transform;
        }

        public void SetWorldTransform(Matrix transform)
        {
            InternalCalls.VehicleComponent_SetWorldTransform(
                transform._11, transform._12, transform._13, transform._14,
                transform._21, transform._22, transform._23, transform._24,
                transform._31, transform._32, transform._33, transform._34,
                transform._41, transform._42, transform._43, transform._44,
                Parent.entityID
            );
        }

        public Matrix GetWorldTransform()
        {
            Matrix transform = new Matrix();

            InternalCalls.VehicleComponent_GetWorldTransform(
                out transform._11, out transform._12, out transform._13, out transform._14,
                out transform._21, out transform._22, out transform._23, out transform._24,
                out transform._31, out transform._32, out transform._33, out transform._34,
                out transform._41, out transform._42, out transform._43, out transform._44,
                Parent.entityID
            );

            return transform;
        }


        public float GetCurrentKm()
        {
            return GetLinearVelocity().Length() * 3.6f;
        }

        public Vector3 GetHitWallNormal()
        {
            Vector3 normal = new Vector3();
            InternalCalls.VehicleComponent_GetHitWallNormal(
                out normal.x, out normal.y, out normal.z, Parent.entityID
                );

            return normal;
        }

        public void Jump(Vector3 velocity)
        {
            InternalCalls.VehicleComponent_Jump(velocity.x, velocity.y, velocity.z, Parent.entityID);
        }

        public void SetJumpFlag(bool flag)
        {
            InternalCalls.VehicleComponent_SetJumpFlag(flag, Parent.entityID);
        }

        public bool IsJump()
        {
            return InternalCalls.VehicleComponent_IsJump(Parent.entityID);
        }

        public bool IsDrifting()
        {
            return InternalCalls.VehicleComponent_IsDrifting(Parent.entityID);
        }

        public void SetDrifting(bool drifting)
        {
            InternalCalls.VehicleComponent_SetDrifting(drifting, Parent.entityID);
        }

        public void ResetDrift()
        {
            InternalCalls.VehicleComponent_ResetDrift(Parent.entityID);
        }

        public int GetDriftDirection()
        {
            return InternalCalls.VehicleComponent_GetDriftDirection(Parent.entityID);
        }

        public float GetOnDriftInputLX()
        {
            return InternalCalls.VehicleComponent_GetOnDriftInputLX(Parent.entityID);
        }

        public bool OnGround()
        {
            return InternalCalls.VehicleComponent_OnGround(Parent.entityID);
        }

        public bool OnGrass()
        {
            return InternalCalls.VehicleComponent_OnGrass(Parent.entityID);
        }

        public bool HitWall()
        {
            return InternalCalls.VehicleComponent_HitWall(Parent.entityID);
        }

        public bool Enabled
        {
            get
            {
                return InternalCalls.Component_GetEnabled(Parent.entityID, "VehicleComponent");
            }

            set
            {
                InternalCalls.Component_SetEnabled(Parent.entityID, value, "VehicleComponent");
            }
        }
    }
}
