using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public static class Scene
    {
        public static GameObject FindGameObjectByID(uint id)
        {
            bool exists = InternalCalls.Scene_FindGameObjectByID(id);
            if (exists) 
            {
                return new GameObject(id);
            }

            return null;
        }

        public static GameObject FindGameObjectByTag(string tag)
        {
            uint id = InternalCalls.Scene_FindGameObjectByTag(tag);
            if (id != 0)
            {
                return new GameObject(id);
            }

            return null;
        }

        public static GameObject FindGameObjectByName(string name)
        {
            GameObject[] gameObjects = GetGameObjects();
            foreach (GameObject gameObject in gameObjects)
            {
                if (gameObject.Name != name)
                    continue;

                return gameObject;
            }

            return null;
        }

        public static GameObject[] FindGameObjectsByTag(string tag)
        {
            uint[] ids = InternalCalls.Scene_FindGameObjectsByTag(tag);

            List<GameObject> gameObjects = new List<GameObject>();
            foreach (uint id in ids)
            {
                gameObjects.Add(new GameObject(id));
            }

            return gameObjects.ToArray();
        }

        public static GameObject[] GetGameObjects()
        {
            uint[] ids = InternalCalls.Scene_GetGameObjects();

            List<GameObject> gameObjects = new List<GameObject>();
            foreach (uint id in ids) 
            {
                gameObjects.Add(new GameObject(id));
            }

            return gameObjects.ToArray();
        }

        public static int GetGameObjectsCountByTag(string tag)
        {
            GameObject[] gameObjects = FindGameObjectsByTag(tag);

            return gameObjects.Length;
        }
    }
}
