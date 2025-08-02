using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    // Add checkpoint state.
    public class CheckPoint : Component
    {
        public int DesireEntityPassingCheckPointCount
        {
            get
            {
                return InternalCalls.CheckPointComponent_GetDesireEntityPassingCheckPointCount(Parent.entityID);
            }
        }

        public int Number
        {
            get
            {
                return InternalCalls.CheckPointComponent_GetNumber(Parent.entityID);
            }
        }

        public int NextCheckPointNumber
        {
            get
            {
                return InternalCalls.CheckPointComponent_GetNextCheckPointNumber(Parent.entityID);
            }
        }
    }
}
