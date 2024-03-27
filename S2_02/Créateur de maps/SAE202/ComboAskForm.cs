using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SAE202
{
    public partial class ComboAskForm : Form
    {
        public string Title
        {
            set => this.Text = value;
        }
        public int ComboIndex
        {
            get => questionComboBox.SelectedIndex;
            set => questionComboBox.SelectedIndex = value;
        }

        public ComboAskForm(ComboBox.ObjectCollection coll)
        {
            InitializeComponent();

            questionComboBox.Items.Clear();
            for(int i = 0; i < coll.Count; i++)
                questionComboBox.Items.Add(coll[i]);

            if (questionComboBox.Items.Count > 0)
                questionComboBox.SelectedIndex = 0;
        }
    }
}
