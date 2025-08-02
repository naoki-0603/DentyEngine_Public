using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public static class Physics
    {

        public static Vector3 Gravity
        {
            get
            {
                Vector3 gravity = new Vector3();
                InternalCalls.Physics_GetGravity(out gravity.x, out gravity.y, out gravity.z);

                return gravity;
            }
        }
    }
}
