using System;
using System.CodeDom;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public abstract class Object
    {
    }

    public class GameObject : Object
    {
        //
        // Member functions.
        //
        internal GameObject(uint id)
        {
            entityID = id;
            transform = new Transform();
            transform.Parent = this;

            _monoComponents = new List<Component>();
        }


        public T GetComponent<T>() where T : Component, new()
        {
            string componentType = InternalHasComponent(typeof(T).Name, typeof(T));
            
            if (componentType == "BuildInComponent")
            {
                string name = typeof(T).Name;

                T component = new T()
                {
                    Parent = this,
                    Name = name
                };

                return component;
            }
            else if (componentType == "MonoBehaviour")
            {
                foreach (MonoBehaviour monoBehaviour in _monoComponents)
                {
                    if (typeof(T).Name == monoBehaviour.Name)
                    {
                        return monoBehaviour as T;
                    }
                }
            }

            return null;
        }

        public Component[] GetComponents()
        {
            List<Component> components = new List<Component>();

            string[] componentNames = InternalCalls.GameObject_GetComponents(entityID);
            
            foreach (string name in componentNames) 
            {
                Component component = ComponentCreator.Create(name);
                if (entityID == 0)
                {
                    Debug.Log("Entity id is zero!!");
                }
                component.Parent = this;
                component.Name = name;

                components.Add(component);
            }

            return components.ToArray();
        }

        public void AddMonoBehaviour(MonoBehaviour monoBehaviour)
        {
            _monoComponents.Add(monoBehaviour);
        }

        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);

            bool hasComponent = InternalCalls.GameObject_HasComponent(entityID, componentType);

            if (hasComponent)
            {
                return true;
            }

            foreach (MonoBehaviour monoBehaviour in _monoComponents)
            {
                if (monoBehaviour.Name == typeof(T).Name)
                    return true;
            }

            return false;
        }

        public string Name
        {
            get
            {
                return InternalCalls.GameObject_GetName(entityID);
            }

            set 
            {
                InternalCalls.GameObject_SetName(value, entityID);
            }
        }

        public string Tag
        {
            get
            {
                return InternalCalls.GameObject_GetTag(entityID);
            }
        }

        public bool Enabled
        {
            get
            {
                return InternalCalls.GameObject_GetEnabled(entityID);
            }

            set
            {
                InternalCalls.GameObject_SetEnabled(value, entityID);
            }
        }

        private string InternalHasComponent(string componentName, Type type)
        {
            bool hasComponent = InternalCalls.GameObject_HasComponent(entityID, type);
            if (hasComponent)
            {
                return "BuildInComponent";
            }

            foreach (MonoBehaviour monoBehaviour in _monoComponents)
            {
                if (monoBehaviour.Name == componentName)
                    return "MonoBehaviour";
            }

            return "None";
        }

        //
        // Member variables.
        //
        public Transform transform;
        public readonly uint entityID;

        private List<Component> _monoComponents;
    }
}
