using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
	public class CameraController : MonoBehaviour
	{
		private GameObject m_camera;
		private GameCamera m_cameraComponent;
		private GameObject m_cameraTarget;
		private RigidBody m_cameraTargetRigidBody;
		private Vector3 m_cameraFront = new Vector3(0, 0, -1);

		public CameraController()
		{
		}

		public override void OnCreated(uint entityID)
		{
			base.OnCreated(entityID);
			AddMonoBehaviour(this, "CameraController"); 
		
			m_camera = Scene.FindGameObjectByID(entityID);
			m_cameraComponent = m_camera.GetComponent<GameCamera>();
            m_cameraTarget = Scene.FindGameObjectByName("Player");
			m_cameraTargetRigidBody = m_cameraTarget.GetComponent<RigidBody>();
        }

		public override void OnUpdate()
		{
			UpdateTransform();
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

		public Vector3 GetFront()
		{
			return m_cameraFront;
		}

		private void UpdateTransform()
		{
            Vector3 targetPosition = m_cameraTargetRigidBody.RigidBodyWorld.Offset;
            Vector3 up = Vector3.Up;
            Vector3 cameraPosition = targetPosition + Vector3.Front * -1.75f + up;

            m_cameraComponent.SetFocusPosition(targetPosition);
            m_camera.transform.LocalPosition = cameraPosition;
        }
	}
}