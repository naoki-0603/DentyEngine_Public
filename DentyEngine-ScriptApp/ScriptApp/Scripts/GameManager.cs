using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
	public class GameManager : MonoBehaviour
	{
		public GameManager()
		{
		}

		public override void OnCreated(uint entityID)
		{
			base.OnCreated(entityID);
			AddMonoBehaviour(this, "GameManager");

		}

		public override void OnUpdate()
		{
			if (Global.GameManager.IsForceReset())
			{
				Global.GameManager.ResetGame();
			}

            if (!Global.GameManager.HasInitialized())
            {
                Global.GameManager.Start();
            }

            if (!Global.GameManager.HasFinished() && Global.GameManager.HasInitialized())
			{
				if (Input.IsPadTriggered(PadKeyCode.Y))
				{
					Global.GameManager.ForceReset();
				}
				else
				{
                    Global.GameManager.UpdateTimer();
                }
            }
			else if (!Global.GameManager.IsReset())
			{
				if (Global.GameManager.HasFinished())
				{
					UIManager.ShowClearText();
				}

				if (Input.IsPadTriggered(PadKeyCode.X) && Global.GameManager.HasFinished())
				{
					Global.GameManager.Reset();
				}
			}
			else if (Global.GameManager.IsReset())
			{
				Global.GameManager.ResetGame();
			}
        }

		public override void OnFixedUpdate()
		{
		}

		public override void OnLateUpdate()
		{
		}

		public override void OnEnabled()
		{
		}

		public override void OnDisabled()
		{
		}

		public override void OnGui()
		{
		}

		public override void OnDestroy()
		{
		}

		public override void OnCollisionEnter(uint collisionObjectID)
		{
		}

		public override void OnCollisionStay(uint collisionObjectID)
		{
		}

		public override void OnCollisionExit(uint collisionObjectID)
		{
		}

		public override void OnTriggerEnter(uint collisionObjectID)
		{
		}

		public override void OnTriggerStay(uint collisionObjectID)
		{
		}

		public override void OnTriggerExit(uint collisionObjectID)
		{
		}
		public override void OnRuntimeSceneChange()
		{
		}
		public override void OnRuntimeSceneChanged(string sceneName)
		{
		}
		public override void OnChangeToPlayState()
		{
		}
		public override void OnChangeToEditState()
		{
		}
	}
}