using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SAE201205_editor
{
    public partial class DoubleNumAskForm : Form
    {
        public int Sel1 = -1;
        public int Sel2 = -1;

        private int borderW = -1, borderH = -1;
        private string warn = "";

        public DoubleNumAskForm(int val1, int val2, int mins, int maxs, string question, string label1, string label2, string warn)
        {
            InitializeComponent();

            this.Text = question;
            this.warn = warn;
            elem1Label.Text = label1;
            elem2Label.Text = label2;
            elem1NumUpDown.Minimum = mins;
            elem1NumUpDown.Maximum = maxs;
            elem2NumUpDown.Minimum = mins;
            elem2NumUpDown.Maximum = maxs;

            int d = -1;
            if(elem1Label.Width > elem2Label.Width)
            {
                d = elem1Label.Width;
                elem2Label.Left += d - elem2Label.Width;
            }
            else
            {
                d = elem2Label.Width;
                elem1Label.Left += d - elem1Label.Width;
            }

            elem1NumUpDown.Left = 18 + d;
            elem2NumUpDown.Left = 18 + d;

            this.Width = 46 + d + elem1NumUpDown.Width;
            okButton.Left = (this.Width / 2) - (okButton.Width / 2);



            this.borderW = val1;
            this.borderH = val2;
            elem1NumUpDown.Value = val1;
            elem2NumUpDown.Value = val2;
        }

        private void okButton_Click(object sender, EventArgs e)
        {
            int potenW = (int)elem1NumUpDown.Value;
            int potenH = (int)elem2NumUpDown.Value;

            if ((warn != "") && (potenW < borderW || potenH < borderH))
            {
                DialogResult r = MessageBox.Show("Vous avez entré une taille plus petite que celle actuelle, des données risquent d'être perdues." + Environment.NewLine + "Êtes vous sûr de vouloir continuer ?", "Attention", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                if (r != DialogResult.Yes)
                    return;
            }

            Sel1 = potenW;
            Sel2 = potenH;

            this.DialogResult = DialogResult.OK;
        }
    }
}
