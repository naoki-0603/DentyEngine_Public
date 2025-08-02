using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace DentyEngine
{
    public abstract class SceneParameters
    {
    }

    public static class SceneParameterSender
    {
        public static void SetOrOverwrite(SceneParameters parameters, string name)
        {
            if (Exists(name))
            {
                Remove(name);
            }

            _parametes.Add(name, parameters);
        }

        public static void Remove(string name)
        {
            _parametes.Remove(name);
        }

        public static Type Get<Type>(string name) where Type : class
        {
            if (Exists(name))
            {
                return _parametes[name] as Type;
            }

            return null;
        }

        public static bool Exists(string name)
        {
            return _parametes.ContainsKey(name);
        }

        private static Dictionary<string, SceneParameters> _parametes = new Dictionary<string, SceneParameters>();
    }
}
