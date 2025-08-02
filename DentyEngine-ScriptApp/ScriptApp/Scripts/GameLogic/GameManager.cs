using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    namespace Global
    {
        public class GameManagerResources
        {
            public float m_timer = 0.0f;
            public uint m_stageMaxCoin = 0u;
            public bool m_gameStart = false;
            public bool m_gameFinished = false;
            public bool m_onReset = false;
            public bool m_forceReset = false;

            public void Reset()
            {
                m_timer = 0.0f;
                m_stageMaxCoin = 0u;
                m_gameStart = false;
                m_gameFinished = false;
                m_onReset = false;
                m_forceReset = false;
            }
        }

        public static class GameManager
        {
            public static void Start()
            {
                m_resources.m_stageMaxCoin = (uint)Scene.GetGameObjectsCountByTag("Coin");
                m_resources.m_timer = 0.0f;
                m_resources.m_gameStart = true;
                m_resources.m_gameFinished = false;
                m_resources.m_onReset = false;

                UIManager.Init(m_resources.m_stageMaxCoin);
            }

            public static void Finish()
            {
                m_resources.m_gameFinished = true;
            }

            public static void UpdateTimer()
            {
                m_resources.m_timer += Timer.GetDeltaTime();

                UIManager.Update(m_resources.m_timer);
            }

            public static void CheckCompleteTask(uint currentCoin)
            {
                UIManager.UpdateCoin(currentCoin);

                if (currentCoin >= m_resources.m_stageMaxCoin)
                {
                    Finish();
                }
            }

            public static void Reset()
            {
                m_resources.m_onReset = true;
            }

            public static void ForceReset()
            {
                m_resources.m_forceReset = true;
            }

            public static bool HasInitialized()
            {
                return m_resources.m_gameStart;
            }

            public static bool HasFinished()
            {
                return m_resources.m_gameFinished;
            }

            public static bool IsReset()
            {
                return m_resources.m_onReset;
            }

            public static bool IsForceReset()
            {
                return m_resources.m_forceReset;
            }

            public static void ResetGame()
            {
                // ゲームをリセットする
                {
                    m_resources.Reset();
                }

                // コインをもとに戻す
                {
                    GameObject[] objects = Scene.GetGameObjects();
                    foreach (var item in objects)
                    {
                        if (item.Tag != "Coin")
                            continue;

                        item.Enabled = true;
                    }
                }

                UIManager.Reset();
            }

            private static GameManagerResources m_resources = new GameManagerResources();
        }
    }
}
