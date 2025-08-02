using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public static class Timer
    {
        public static float GetDeltaTime()
        {
            float deltaTime = 0.0f;
            InternalCalls.Timer_GetDeltaTime(out deltaTime);

            return deltaTime;
        }
    }
}