using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public sealed class Vector2I
    {
        public Vector2I() { x = 0; y = 0; }
        
        public Vector2I(int x, int y)
        {
            this.x = x;
            this.y = y;
        }

        public static Vector2I Zero => new Vector2I();

        // Member values.
        public int x, y;
    }
}
