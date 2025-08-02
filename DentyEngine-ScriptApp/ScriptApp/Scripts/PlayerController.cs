using DentyEngine.Global;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
	public class PlayerController : MonoBehaviour
	{
		private float m_speed = 3.0f;
		private float m_jumpSpeed = 4.0f;

		private const string k_shouldCollideObjectTag = "Stage";
        private const float k_maxSpeed = 3.5f;

		private bool m_onGround = false;

		private GameObject m_player;
		private GameObject m_respawnPoint;
		private RigidBody m_body;
		private Vector3 m_velocity;

		private uint m_numCoins = 0u;

		// TODO(naoki): OnInitializedみたいなイベント関数を用意する。
		private bool m_initialized = false;

		public PlayerController()
		{
		}

		public override void OnCreated(uint entityID)
		{
			base.OnCreated(entityID);
			AddMonoBehaviour(this, "PlayerController");

			m_player = Scene.FindGameObjectByID(entityID);
			m_respawnPoint = Scene.FindGameObjectByName("RespawnPoint");
            m_body = m_player.GetComponent<RigidBody>();
			m_body.Gravity = new Vector3();
			m_velocity = new Vector3();

			m_onGround = false;

			m_numCoins = 0u;
        }

		public override void OnUpdate()
		{
			if (Global.GameManager.IsForceReset())
			{
				Reset();

				return;
			}

			if (!Global.GameManager.HasFinished() && Global.GameManager.HasInitialized())
			{
                if (!m_initialized)
                {
                    Init();

                    m_initialized = true;
                }

                m_velocity = m_body.LinearVelocity;

                InputMove();

                if (m_onGround)
                {
                    InputJump();
                }
                CheckSpeed();

                m_body.LinearVelocity = m_velocity;


                // TODO(naoki): カプセル化されていないので、後ほど改善できればする
                Global.GameManager.CheckCompleteTask(m_numCoins);
            }
			else if (Global.GameManager.HasFinished())
			{
				if (Global.GameManager.IsReset())
				{
					Reset();
				}
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
			GameObject collideObject = Scene.FindGameObjectByID(collisionObjectID);
			if (collideObject.Tag == k_shouldCollideObjectTag)
			{
				m_onGround = true;
			}
			
        }

		public override void OnCollisionStay(uint collisionObjectID)
		{
		}

		public override void OnCollisionExit(uint collisionObjectID)
		{
            GameObject collideObject = Scene.FindGameObjectByID(collisionObjectID);
            if (collideObject.Tag == k_shouldCollideObjectTag)
            {
                m_onGround = false;
            }
        }

		public override void OnTriggerEnter(uint collisionObjectID)
		{
			GameObject collideObject = Scene.FindGameObjectByID(collisionObjectID);
			if (collideObject.Tag == "Coin")
			{
				collideObject.Enabled = false;

                ++m_numCoins;
			}
            else if (collideObject.Name == "Respawn_trigger")
            {
                Respawn();
            }
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
			m_onGround = false;
		}

        public uint GetNumCoins()
		{
			return m_numCoins;
		}

        private void InputMove()
		{
			float dt = Timer.GetDeltaTime();
			
			float lx = Input.GetNormalizedThumbLX();
			float ly = Input.GetNormalizedThumbLY();

            m_velocity.x += m_speed * lx * dt;
			m_velocity.z += m_speed * ly * dt;
		}

		private void InputJump()
		{
			if (Input.IsPadTriggered(PadKeyCode.A))
			{
				m_velocity.y = m_jumpSpeed;
            }
        }

		private void CheckSpeed()
		{
			float speedXZSq = (m_velocity.x * m_velocity.x + m_velocity.z * m_velocity.z);

			if (speedXZSq >= (k_maxSpeed * k_maxSpeed))
			{
				Vector2 vel = new Vector2(m_velocity.x, m_velocity.z);
				float velLen = vel.Length();
				vel.x /= velLen;
				vel.y /= velLen;

				vel *= k_maxSpeed;

				m_velocity.x = vel.x;
				m_velocity.z = vel.y;
            }
		}

		private void Respawn()
		{
            m_velocity = Vector3.Zero;
            m_onGround = false;

            m_body.LinearVelocity = m_velocity;

            m_body.SetPosition(m_respawnPoint.transform.LocalPosition);
        }

		private void Reset()
		{
			m_velocity = Vector3.Zero;
			m_onGround = false;
			m_initialized = false;
			m_numCoins = 0;

            m_body.LinearVelocity = m_velocity;

			m_body.SetPosition(m_respawnPoint.transform.LocalPosition);

		}

		private void Init()
		{
            m_player = Scene.FindGameObjectByID(Parent.entityID);
            m_respawnPoint = Scene.FindGameObjectByName("RespawnPoint");
            m_body = m_player.GetComponent<RigidBody>();
            m_velocity = new Vector3();

            m_onGround = false;

            m_numCoins = 0u;
        }

	}
}