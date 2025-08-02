using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public sealed class Vector3I
    {
        public Vector3I() { x = 0; y = 0; z = 0; }

        public Vector3I(int x, int y, int z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public static Vector3I Zero => new Vector3I();

        // Member values.
        public int x, y, z;
    }
}
