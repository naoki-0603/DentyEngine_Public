using Microsoft.SqlServer.Server;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// Reference https://easings.net/ja

namespace DentyEngine
{
    public static class Easing
    {
        //
        // Float functions.
        //

        public static float Linear(in float begin, in float end, in float t)
        {
            if (t >= 1.0f)
                return end;

            return (begin * (1.0f - t)) + (end * t);
        }

        public static float EaseOutQuart(in float begin, in float end, in float t)
        {
            if (t >= 1.0f)
                return end;

            float currentRate = 1.0f - (float)Math.Pow(1.0f - t, 4.0f);

            return (begin * (1.0f - currentRate)) + (end * currentRate);
        }

        public static float EaseOutElastic(in float begin, in float end, in float t)
        {
            if (t <= 0.0f)
                return begin;

            if (t >= 1.0f)
                return end;

            float c4 = 2.0f * (float)Math.PI / 3.0f;
            float currentRate = (float)Math.Pow(2.0f, -10.0f * t) * (float)Math.Sin((t * 10 - 0.75f) * c4) + 1.0f;

            return (begin * (1.0f - currentRate)) + (end * currentRate);
        }

        //
        // Vector2 functions.
        //
        public static Vector2 Linear(in Vector2 begin, in Vector2 end, in float t)
        {
            if (t >= 1.0f)
                return end;

            float x = Linear(in begin.x, in end.x, in t);
            float y = Linear(in begin.y, in end.y, in t);

            return new Vector2(x, y);
        }

        public static Vector2 EaseOutQuart(in Vector2 begin, in Vector2 end, in float t)
        {
            if (t >= 1.0f)
                return end;

            float x = EaseOutQuart(in begin.x, in end.x, t);
            float y = EaseOutQuart(in begin.y, in end.y, t);

            return new Vector2(x, y);
        }

        public static Vector2 EaseOutElastic(in Vector2 begin, in Vector2 end, in float t)
        {
            if (t <= 0.0f)
                return begin;

            if (t >= 1.0f)
                return end;

            float x = EaseOutElastic(in begin.x, in end.x, in t);
            float y = EaseOutElastic(in begin.y, in end.y, in t);

            return new Vector2(x, y);
        }

        //
        // Vector3 functions.
        //

        public static Vector3 Linear(in Vector3 begin, in Vector3 end, in float t)
        {
            if (t >= 1.0f)
                return end;

            float x = Linear(in begin.x, in end.x, t);
            float y = Linear(in begin.y, in end.y, t);
            float z = Linear(in begin.z, in end.z, t);

            return new Vector3(x, y, z);
        }

        public static Vector3 EaseOutQuart(in Vector3 begin, in Vector3 end, in float t)
        {
            if (t >= 1.0f)
                return end;

            float x = EaseOutQuart(in begin.x, in end.x, t);
            float y = EaseOutQuart(in begin.y, in end.y, t);
            float z = EaseOutQuart(in begin.z, in end.z, t);

            return new Vector3(x, y, z);
        }

        public static Vector3 EaseOutElastic(in Vector3 begin, in Vector3 end, in float t)
        {
            if (t <= 0.0f)
                return begin;

            if (t >= 1.0f)
                return end;

            float x = EaseOutElastic(in begin.x, in end.x, in t);
            float y = EaseOutElastic(in begin.y, in end.y, in t);
            float z = EaseOutElastic(in begin.z, in end.z, in t);

            return new Vector3(x, y, z);
        }
    }
}
