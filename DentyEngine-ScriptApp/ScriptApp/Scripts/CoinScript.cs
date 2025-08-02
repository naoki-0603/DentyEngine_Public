using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
	public class CoinScript : MonoBehaviour
	{
		public CoinScript()
		{
		}

		public override void OnCreated(uint entityID)
		{
			base.OnCreated(entityID);
			AddMonoBehaviour(this, "CoinScript"); 
		}

		public override void OnUpdate()
		{
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