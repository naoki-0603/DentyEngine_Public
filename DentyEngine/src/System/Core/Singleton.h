#pragma once

namespace DentyEngine
{
    namespace DesignPattern
    {
        template <class Type>
        class Singleton
        {
        public:
            static Type& GetInstance()
            {
                static Type instance;
                return instance;
            }
        };
    }
}