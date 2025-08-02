using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public class UserInterfaceItem
    {
        public Vector2 Position
        {
            get
            {
                Vector2 position = new Vector2();

                InternalCalls.UIComponent_GetPosition(out position.x, out position.y, _ownerID);

                return position;
            }

            set
            {
                InternalCalls.UIComponent_SetPosition(value.x, value.y, _ownerID);
            }
        }

        public float Scale
        {
            get
            {
                return InternalCalls.UIComponent_GetScale(_ownerID);
            }

            set
            {
                InternalCalls.UIComponent_SetScale(value, _ownerID);
            }
        }

        public float Rotation
        {
            get
            {
                return InternalCalls.UIComponent_GetRotation(_ownerID);
            }

            set
            {
                InternalCalls.UIComponent_SetRotation(value, _ownerID);
            }
        }

        public bool Render
        {
            get
            {
                return InternalCalls.UIComponent_GetRender(_ownerID);
            }

            set
            {
                InternalCalls.UIComponent_SetRender(value, _ownerID);
            }
        }

        protected UserInterfaceItem(uint ownerID)
        {
            _ownerID = ownerID;
        }

        protected readonly uint _ownerID;
    }

    public class UIText : UserInterfaceItem
    {
        public UIText(uint ownerID) : base(ownerID)
        {
        }

        public string Text
        {
            get
            {
                return InternalCalls.UIComponent_Text_GetText(_ownerID);
            }

            set
            {
                InternalCalls.UIComponent_Text_SetText(value, _ownerID);
            }
        }

        public float Spacing
        {
            get
            {
                return InternalCalls.UIComponent_Text_GetSpacing(_ownerID);
            }

            set
            {
                InternalCalls.UIComponent_Text_SetSpacing(value, _ownerID);
            }
        }

        // TODO Replace return type to Color.
        public Vector4 Color
        {
            get
            {
                Vector4 color = new Vector4();

                InternalCalls.UIComponent_Text_GetTextColor(out color.x, out color.y, out color.z, out color.w, _ownerID);

                return color;
            }

            set
            {
                InternalCalls.UIComponent_Text_SetTextColor(value.x, value.y, value.z, value.w, _ownerID);
            }
        }
    }

    public class UIImage : UserInterfaceItem
    {
        public UIImage(uint ownerID) : base(ownerID)
        {
        }

        public Vector4I ClipRect
        {
            get
            {
                Vector4I clipRect = new Vector4I();

                InternalCalls.UIComponent_Image_GetClipRect(out clipRect.x, out clipRect.y, out clipRect.z, out clipRect.w, _ownerID);

                return clipRect;
            }

            set
            {
                InternalCalls.UIComponent_Image_SetClipRect(value.x, value.y, value.z, value.w, _ownerID);
            }
        }

        public Vector2I TextureSize
        {
            get
            {
                Vector2I textureSize = new Vector2I();

                InternalCalls.UIComponent_Image_GetTextureSize(out textureSize.x, out textureSize.y, _ownerID);

                return textureSize;
            }
        }
    }
}
