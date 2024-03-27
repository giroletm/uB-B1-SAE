using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SAE201205_editor
{
    public class PanelMagic : Panel
    {
        public event MouseEventHandler CTRLMouseWheel;


        protected override Point ScrollToControl(Control activeControl)
        {
            return DisplayRectangle.Location;
        }

        protected override void OnMouseWheel(MouseEventArgs e)
        {
            if ((Program.GetAsyncKeyState(0x10) & 0x8000) > 0 && HScroll && VScroll)
            {
                VScroll = false;
                base.OnMouseWheel(e);
                VScroll = true;
            }
            else if ((Program.GetAsyncKeyState(0x11) & 0x8000) > 0)
            {
                if (CTRLMouseWheel != null)
                    CTRLMouseWheel.Invoke(this, e);
            }
            else
            {
                base.OnMouseWheel(e);
            }
        }
    }
}
