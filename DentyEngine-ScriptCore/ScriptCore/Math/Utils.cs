using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public static class Utils
    {
        public static bool ApproxEqual(float a, float b)
        {
            bool equal;
            InternalCalls.Math_ApproxEqual(a, b, out equal);

            return equal;
        }

        public static float ToRadian(float degree)
        {
            float radian;
            InternalCalls.Math_ToRadian(degree, out radian);

            return radian;
        }

        public static float LimitAngle(float radian)
        {
            if (radian <= (float)-Math.PI)
            {
                radian += (float)(Math.PI + Math.PI);
            }
            else if (radian >= (float)Math.PI)
            {
                radian += -(float)(Math.PI + Math.PI);
            }

            return radian;
        }
    }
}
