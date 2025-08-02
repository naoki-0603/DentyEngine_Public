using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public static class SceneManager
    {
        public static void ChangeSceneFromIndex(int sceneIndex, bool reloadScene)
        {
            InternalCalls.RuntimeSceneManager_ChangeSceneFromIndex(sceneIndex, reloadScene);
        }

        public static void ChangeSceneFromName(string name, bool reloadScene)
        {
            InternalCalls.RuntimeSceneManager_ChangeSceneFromName(name, reloadScene);
        }

        public static GameObject[] FindGameObjectsOfSceneByName(string name)
        {
            uint[] ids = InternalCalls.EditorSceneManager_FindGameObjectsOfSceneByName(name);

            List<GameObject> gameObjects = new List<GameObject>();
            foreach (uint id in ids) 
            {
                GameObject gameObject = new GameObject(id);

                gameObjects.Add(gameObject);
            }

            return gameObjects.ToArray();
        }

        public static GameObject[] FindGameObjectsByTagOfSceneByName(string name, string tag)
        {
            uint[] ids = InternalCalls.EditorSceneManager_FindGameObjectsByTagOfSceneByName(name, tag);

            List<GameObject> gameObjects = new List<GameObject>();
            foreach (uint id in ids)
            {
                GameObject gameObject = new GameObject(id);

                gameObjects.Add(gameObject);
            }

            return gameObjects.ToArray();
        }

        public static string GetActiveSceneName()
        {
            return InternalCalls.EditorSceneManager_GetActiveSceneName();
        }
    }
}
