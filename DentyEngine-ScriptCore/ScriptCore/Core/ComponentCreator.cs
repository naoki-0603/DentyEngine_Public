using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public static class ComponentCreator
    {
        public static Component Create(string componentName)
        {
            // Graphics
            if (componentName == "DirectionalLight") return new DirectionalLight();
            if (componentName == "PointLight") return new PointLight();
            if (componentName == "SpotLight") return new SpotLight();

            if (componentName == "GameCamera") return new GameCamera();
            if (componentName == "MeshRenderer") return new MeshRenderer();
            if (componentName == "SkinnedMeshRenderer") return new SkinnedMeshRenderer();

            // Physics
            if (componentName == "BoxCollider") return new BoxCollider();
            if (componentName == "SphereCollider") return new SphereCollider();
            if (componentName == "CapsuleCollider") return new CapsuleCollider();
            if (componentName == "MeshCollider") return new MeshCollider();
            if (componentName == "ConvexMeshCollider") return new ConvexMeshCollider();
            if (componentName == "ApproximationMeshCollider") return new ApproximationMeshCollider();
            if (componentName == "RigidBody") return new RigidBody();
            if (componentName == "DynamicCharacterController") return new DynamicCharacterController();

            // Animation
            if (componentName == "Animator") return new Animator();

            return null;
        }
    }
}
