using DentyEngine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public class PhysicalMovement
    {
        public void Initialize(float mass = 1.0f)
        {
            _velocity = new Vector3();
            _accelration = new Vector3();

            _resultant = new Vector3();

            _mass = mass;
        }

        public void Intergrate(ref Vector3 localPosition, float deltaTime)
        {
            _accelration = _resultant / _mass;
            _velocity += _accelration * deltaTime;
            localPosition += _velocity * deltaTime;

            _resultant = Vector3.Zero;
        }

        public void AddGravity(Vector3 gravity)
        {
            AddForce(gravity);
        }

        public void AddForce(Vector3 force)
        {
            _resultant += force;
        }

        //
        // Setter
        //
        public void SetVelocity(Vector3 newVelocity)
        {
            _velocity = newVelocity;
        }

        //
        // Getter
        //

        public float GetMass() 
        {
            return _mass;
        }

        public Vector3 GetVelocity()
        {
            return _velocity;
        }

        // Member values.
        private float _mass;
        private Vector3 _velocity;
        private Vector3 _accelration;
        private Vector3 _resultant;

        // Constants value
        private static Vector3 GRAVITY = new Vector3(0.0f, -9.8f, 0.0f);
    }
}
