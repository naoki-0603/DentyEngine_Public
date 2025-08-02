using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public class CPUParticleProps
    {
        public CPUParticleProps()
        {
            beginSize = 0.0f;
            endSize = 0.0f;
            sizeVariation = 0.0f;

            lifeTime = 1.0f;

            rotation = 0.0f;

            position = new Vector3();

            beginPosition = new Vector3();
            endPosition = new Vector3();

            initialVelocity = new Vector3();
            velocityVariation = new Vector3();

            beginColor = Color.WHITE;
            endColor = Color.WHITE;

            applyGravityForce = true;
            useBeginAndEndPosition = false;
        }

        public float beginSize;
        public float endSize;
        public float sizeVariation;

        public float lifeTime;

        public float rotation;

        public Vector3 position;

        public Vector3 beginPosition;
        public Vector3 endPosition;

        public Vector3 initialVelocity;
        public Vector3 velocityVariation;

        public Color beginColor;
        public Color endColor;

        public bool applyGravityForce;
        public bool useBeginAndEndPosition;
    }

    public static class CPUParticleSystem
    {
        public static void Emit(CPUParticleProps props)
        {
            InternalCalls.CPUParticleSystem_Emit(
                props.beginSize, props.endSize, props.sizeVariation,
                props.lifeTime,
                props.rotation,
                props.position.x, props.position.y, props.position.z,
                props.beginPosition.x, props.beginPosition.y, props.beginPosition.z,
                props.endPosition.x, props.endPosition.y, props.endPosition.z,
                props.initialVelocity.x, props.initialVelocity.y, props.initialVelocity.z,
                props.velocityVariation.x, props.velocityVariation.y, props.velocityVariation.z,
                props.beginColor.color.x, props.beginColor.color.y, props.beginColor.color.z, props.beginColor.color.w,
                props.endColor.color.x, props.endColor.color.y, props.endColor.color.z, props.endColor.color.w,
                props.applyGravityForce, props.useBeginAndEndPosition
            );
        }
    }

    public static class GPUParticleSystem
    {

    }
}
