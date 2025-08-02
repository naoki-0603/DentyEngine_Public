using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public class UIManagerResources
    {
        public UIText m_timeText;
        public UIText m_coinText;
        public UIText m_maxCoinText;
        public UIText m_gameClearText;
        public UIText m_gameRestartText;
    }

    static public class UIManager
    {
        public static void Init(uint coinMax)
        {
            GameObject[] uiItems = Scene.FindGameObjectsByTag("UI");
            foreach (GameObject uiItem in uiItems)
            {
                if (uiItem.Name == "TimeNumber")
                    s_resources.m_timeText = uiItem.GetComponent<UIComponent>().GetTextItem();
                else if (uiItem.Name == "CoinNum")
                    s_resources.m_coinText = uiItem.GetComponent<UIComponent>().GetTextItem();
                else if (uiItem.Name == "CoinMax")
                    s_resources.m_maxCoinText = uiItem.GetComponent<UIComponent>().GetTextItem();
                else if (uiItem.Name == "GameClear_Text")
                    s_resources.m_gameClearText = uiItem.GetComponent<UIComponent>().GetTextItem();
                else if (uiItem.Name == "GameClear_RestartText")
                    s_resources.m_gameRestartText = uiItem.GetComponent<UIComponent>().GetTextItem();
            }

            s_resources.m_maxCoinText.Text = "/" + coinMax.ToString();
        }

        public static void Update(float time)
        {
            uint t = (uint)time;
            s_resources.m_timeText.Text = t.ToString();
        }

        public static void UpdateCoin(uint coin)
        {
            s_resources.m_coinText.Text = coin.ToString();
        }

        public static void Reset()
        {
            s_resources.m_coinText.Text = "0";
            s_resources.m_maxCoinText.Text = "0";
            s_resources.m_timeText.Text = "0";

            s_resources.m_gameRestartText.Render = false;
            s_resources.m_gameClearText.Render = false;
        }

        public static void ShowClearText()
        {
            s_resources.m_gameClearText.Render = true;
            s_resources.m_gameRestartText.Render = true;
        }

        private static UIManagerResources s_resources = new UIManagerResources();
    } 
}
