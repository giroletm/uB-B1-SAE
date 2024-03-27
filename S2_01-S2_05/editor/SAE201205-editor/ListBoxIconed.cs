using System;
using System.Collections;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SAE201205_editor
{
    class ListBoxIconed : ListBox
    {
        public SmallImageCollection Images = null;

        public ListBoxIconed()
        {
            Images = new SmallImageCollection(this, 16, 16);
            this.DrawMode = DrawMode.OwnerDrawFixed;
            SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.UserPaint | ControlStyles.OptimizedDoubleBuffer | ControlStyles.ResizeRedraw, true);
            this.ItemHeight += 4;
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            Region iRegion = new Region(e.ClipRectangle);
            e.Graphics.FillRegion(new SolidBrush((!this.Enabled) ? SystemColors.Control : this.BackColor), iRegion);
            if (this.Items.Count > 0)
            {
                for (int i = 0; i < this.Items.Count; ++i)
                {
                    System.Drawing.Rectangle irect = this.GetItemRectangle(i);
                    if (e.ClipRectangle.IntersectsWith(irect))
                    {
                        if ((this.SelectionMode == SelectionMode.One && this.SelectedIndex == i)
                        || (this.SelectionMode == SelectionMode.MultiSimple && this.SelectedIndices.Contains(i))
                        || (this.SelectionMode == SelectionMode.MultiExtended && this.SelectedIndices.Contains(i)))
                        {
                            OnDrawItem(new DrawItemEventArgs(e.Graphics, this.Font,
                                irect, i,
                                DrawItemState.Selected, this.ForeColor,
                                this.BackColor));
                        }
                        else
                        {
                            OnDrawItem(new DrawItemEventArgs(e.Graphics, this.Font,
                                irect, i,
                                DrawItemState.Default, this.ForeColor,
                                this.BackColor));
                        }
                        iRegion.Complement(irect);
                    }
                }
            }
            base.OnPaint(e);
        }
        protected override void OnDrawItem(DrawItemEventArgs e)
        {
            if (e.Index >= 0)
            {
                bool doPad = Images.Count > 0;
                bool drawPic = (e.Index < Images.Count) && (Images[e.Index] != null);
                if (this.Items.Count > 0)
                {
                    e.DrawBackground();

                    Color col = this.ForeColor;
                    if ((e.State & DrawItemState.Selected) > 0)
                        col = Color.White;

                    int x = e.Bounds.X;
                    if (doPad)
                        x += 20;

                    e.Graphics.DrawString(this.Items[e.Index].ToString(), e.Font, new SolidBrush(col), new PointF(x, e.Bounds.Y + 2));

                    if (drawPic)
                    {
                        e.Graphics.DrawImage(Images[e.Index], e.Bounds.Location);
                    }
                }
            }

            base.OnDrawItem(e);
        }
        public class SmallImageCollection : IList<Image>
        {
            private ListBoxIconed owner = null;
            private List<Image> items = new List<Image>();

            private int w, h;

            public SmallImageCollection(ListBoxIconed owner, int w, int h)
            {
                this.owner = owner;
                this.w = w;
                this.h = h;
            }

            public Image this[int index]
            {
                get => items[index];
                set => items[index] = ConvertImage(value);
            }

            public int Count
            {
                get
                {
                    return items.Count;
                }
            }

            public bool IsReadOnly => throw new NotImplementedException();

            private Image ConvertImage(Image image)
            {
                if (image == null)
                    return null;

                Bitmap bmp = new Bitmap(w, h);
                using (Graphics g = Graphics.FromImage(bmp))
                {
                    g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.Bicubic;
                    g.DrawImage(image, new Rectangle(0, 0, 16, 16));
                }

                return bmp;
            }

            public void Add(Image image) => items.Add(ConvertImage(image));

            public void Clear() => items.Clear();

            public bool Contains(Image item) => items.Contains(item);

            public void CopyTo(Image[] array, int arrayIndex) => items.CopyTo(array, arrayIndex);

            public IEnumerator<Image> GetEnumerator() => items.GetEnumerator();

            public int IndexOf(Image item) => items.IndexOf(item);

            public void Insert(int index, Image item) => items.Insert(index, item);

            public bool Remove(Image item) => items.Remove(item);

            public void RemoveAt(int index) => items.RemoveAt(index);

            void ICollection<Image>.Add(Image item) => this.Add(item);

            IEnumerator IEnumerable.GetEnumerator() => this.GetEnumerator();
        }
    }

}
