using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public enum VehicleWheel : int
    {
        None = -1,
        FrontLeft = 0,
        FrontRight,
        RearLeft,
        RearRight,

        Max
    }

    // Copy from btRaycastVehicle.h
    public class VehicleWheelInfo
    {

        public Vector3 chassisConnectionPointCS;  //const
        public Vector3 wheelDirectionCS;          //const
        public Vector3 wheelAxleCS;               // const or modified by steering
        public float suspensionRestLength1;      //const
        public float maxSuspensionTravelCm;
        
        public float wheelsRadius;              //const
        public float suspensionStiffness;       //const
        public float wheelsDampingCompression;  //const
        public float wheelsDampingRelaxation;   //const
        public float frictionSlip;
        public float steering;
        public float rotation;
        public float deltaRotation;
        public float rollInfluence;
        public float maxSuspensionForce;

        public float engineForce;

        public float brake;

        public float clippedInvContactDotSuspension;
        public float suspensionRelativeVelocity;

        //calculated by suspension
        public float wheelsSuspensionForce;
        public float skidInfo;

        public bool IsFrontWheel;
    }
}
