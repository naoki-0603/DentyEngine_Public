﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public static class Debug
    {
        public static void Log(string log)
        {
            InternalCalls.Debug_Log(log);
        }
    }
}
