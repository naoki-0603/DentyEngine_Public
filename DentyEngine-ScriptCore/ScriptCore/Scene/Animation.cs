using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public class Animator : Behaviour
    {
        public bool Enabled
        {
            get
            {
                return InternalCalls.Component_GetEnabled(Parent.entityID, "Animator");
            }

            set
            {
                InternalCalls.Component_SetEnabled(Parent.entityID, value, "Animator");
            }
        }
    }
}