using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public sealed class Vector4I
    {
        public Vector4I() { x = 0; y = 0; z = 0; w = 0; }

        public Vector4I(int x, int y, int z, int w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }

        public static Vector4I Zero => new Vector4I();

        // Member values.
        public int x, y, z, w;
    }
}
