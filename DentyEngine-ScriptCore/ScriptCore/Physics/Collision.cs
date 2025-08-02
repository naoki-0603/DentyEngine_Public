using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public enum CollisionStatus : int
    {
        None = -1,
        Enter = 0,
        Stay,
        Exit,

        Max
    }
}