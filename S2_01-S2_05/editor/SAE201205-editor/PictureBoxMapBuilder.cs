using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace SAE201205_editor
{
    public class PictureBoxMapBuilder : PictureBox
    {
        public event EventHandler<MiddleClickEventArgs> MiddleClick;
        public event EventHandler<StartTilePlacementEventArgs> StartTilePlacement;
        public event EventHandler<StartTilePlacementEventArgs> TileDoubleClick;
        public event PaintEventHandler PostPaint;

        public List<TileInfo> Tiles = new List<TileInfo>();
        public Point? SelectedTile = null;
        public int Layer = 0;
        public TileInfo[,] Tilemap
        {
            get => tilemaps[Layer];
            set => tilemaps[Layer] = value;
        }

        public bool[] LayersVisibility { get; }

        private int pixelsPerTile = 32;
        public int PixelsPerTile {
            get => pixelsPerTile;
            set
            {
                pixelsPerTile = value;
                RefreshSize();
            }
        }

        public Rectangle ViewRectangle => new Rectangle((-Bounds.X) / PixelsPerTile, (-Bounds.Y) / PixelsPerTile, Parent.ClientRectangle.Width / PixelsPerTile, Parent.ClientRectangle.Height / PixelsPerTile);

        public static readonly int MAXLAYERS = 3;
        public List<TileInfo[,]> tilemaps = new List<TileInfo[,]>();

        private int wTiles = 256;
        private int hTiles = 256;

        public int WTiles
        {
            get => wTiles;
            set
            {
                wTiles = value;
                RefreshTilemap();
            }
        }

        public int HTiles
        {
            get => hTiles;
            set
            {
                hTiles = value;
                RefreshTilemap();
            }
        }

        public PictureBoxMapBuilder() : base()
        {
            ClearAllTiles();

            LayersVisibility = new bool[MAXLAYERS];
            for(int i = 0; i < LayersVisibility.Length; i++)
                LayersVisibility[i] = true;
        }

        public void ClearAllTiles()
        {
            tilemaps = new List<TileInfo[,]>();
            for (int i = 0; i < MAXLAYERS; i++)
                tilemaps.Add(new TileInfo[0, 0]);

            RefreshTilemap();
        }

        private TileInfo GetTileInfoForObject(object linkedObject)
        {
            TileInfo tileInfo = null;

            if (linkedObject != null)
                tileInfo = Tiles.Find(ti => ti.LinkedObject == linkedObject);

            return tileInfo;
        }

        private TileInfo CreateTileInfoForObject(object linkedObject, Bitmap displayBitmap)
        {
            TileInfo tileInfo = GetTileInfoForObject(linkedObject);

            if (tileInfo == null && linkedObject != null)
            {
                tileInfo = new TileInfo(linkedObject, displayBitmap);
                Tiles.Add(tileInfo);
            }

            return tileInfo;
        }

        public Point GetPointForPos(Point pos)
        {
            return new Point(pos.X / PixelsPerTile, pos.Y / PixelsPerTile);
        }

        public void PlaceTile(int x, int y, object linkedObject, Bitmap displayBitmap)
        {
            if (x < 0 || y < 0 || x >= Tilemap.GetLength(0)  || y >= Tilemap.GetLength(1))
                return;

            TileInfo tileInfo = CreateTileInfoForObject(linkedObject, displayBitmap);

            if((SelectedTile != null) && (SelectedTile?.X == x  && SelectedTile?.Y == y))
                SelectedTile = null;

            Tilemap[x, y] = tileInfo;
            this.Invalidate();
        }

        private void RefreshTilemap()
        {
            for (int i = 0; i < MAXLAYERS; i++)
            {
                TileInfo[,] nTilemap = new TileInfo[WTiles, HTiles];

                for (int x = 0; x < WTiles; x++)
                {
                    for (int y = 0; y < HTiles; y++)
                    {
                        bool isInBounds = (x < tilemaps[i].GetLength(0)) && (y < tilemaps[i].GetLength(1));
                        nTilemap[x, y] = isInBounds ? (tilemaps[i])[x, y] : null;
                    }
                }

                tilemaps[i] = nTilemap;
            }

            RefreshSize();
        }

        private void RefreshSize()
        {

            this.Size = new Size(WTiles * PixelsPerTile, HTiles * PixelsPerTile);
        }

        protected override void OnPaint(PaintEventArgs pe)
        {
            base.OnPaint(pe);

            DoPaint(pe, pixelsPerTile);

            if(PostPaint != null)
                PostPaint.Invoke(this, pe);
        }

        protected virtual void DoPaint(PaintEventArgs pe, int zoom, bool doAll=false)
        {
            InterpolationMode ipm = pe.Graphics.InterpolationMode;
            pe.Graphics.InterpolationMode = InterpolationMode.NearestNeighbor;

            Rectangle maxBds = doAll ? (new Rectangle(0, 0, WTiles, HTiles)) : ViewRectangle;
            if(!doAll)
            {
                maxBds.Width = Math.Min(maxBds.Width + 2, WTiles - maxBds.X);
                maxBds.Height = Math.Min(maxBds.Height + 2, HTiles - maxBds.Y);
            }

            for(int lID = 0; lID < tilemaps.Count; lID++)
            {
                if (!LayersVisibility[lID])
                    continue;

                TileInfo[,] tilemap = tilemaps[lID];

                for (int x = maxBds.X; x < (maxBds.X+maxBds.Width); x++)
                {
                    for (int y = maxBds.Y; y < (maxBds.Y+maxBds.Height); y++)
                    {
                        if (tilemap[x, y] != null)
                        {
                            if (zoom != 1)
                            {
                                Rectangle tileRect = new Rectangle(x * zoom, y * zoom, zoom, zoom);
                                pe.Graphics.DrawImage(tilemap[x, y].DisplayBitmap, tileRect);
                                if ((SelectedTile != null) && (SelectedTile?.X == x && SelectedTile?.Y == y))
                                {
                                    pe.Graphics.FillRectangle(new SolidBrush(Color.FromArgb(0x60, 0xFF, 0xFF, 0xFF)), tileRect);
                                }
                            }
                            else
                            {
                                pe.Graphics.FillRectangle(new SolidBrush(tilemap[x, y].MiddleColor), new Rectangle(x, y, 1, 1));
                            }
                        }
                    }
                }

                if (Tilemap == tilemap && zoom != 1)
                {
                    CompositingMode c = pe.Graphics.CompositingMode;
                    pe.Graphics.CompositingMode = CompositingMode.SourceCopy;
                    for (int x = 0; x < overRect.Width; x++)
                    {
                        for (int y = 0; y < overRect.Height; y++)
                        {
                            Rectangle tileRect = new Rectangle((overRect.X + x) * zoom, (overRect.Y + y) * zoom, zoom, zoom);
                            if (overTile != null)
                                pe.Graphics.DrawImage(overTile.DisplayBitmap, tileRect);
                            else
                                pe.Graphics.FillRectangle(new SolidBrush(Color.Transparent), tileRect);
                        }
                    }
                    pe.Graphics.CompositingMode = c;
                }
            }

            pe.Graphics.InterpolationMode = ipm;
        }

        public Bitmap AsBitmap(int ppt=32, bool drawView=false)
        {
            Bitmap bmp = new Bitmap(WTiles * ppt, HTiles * ppt);
            using(Graphics g = Graphics.FromImage(bmp))
            {
                PaintEventArgs pe = new PaintEventArgs(g, new Rectangle(new Point(0, 0), this.DefaultSize));
                if (ppt != 1)
                {
                    DoPaint(pe, ppt, true);
                }
                else
                {
                    BitmapData bmpD = bmp.LockBits(new Rectangle(new Point(0, 0), bmp.Size), ImageLockMode.WriteOnly, bmp.PixelFormat);
                    var length = bmpD.Stride * bmpD.Height;
                    byte[] bytes = new byte[length];
                    Marshal.Copy(bmpD.Scan0, bytes, 0, length);

                    foreach (TileInfo[,] tilemap in tilemaps)
                    {
                        for (int x = 0; x < WTiles; x++)
                        {
                            for (int y = 0; y < HTiles; y++)
                            {
                                if (tilemap[x, y] != null)
                                {
                                    Color col = tilemap[x, y].MiddleColor;

                                    int idx = (y * bmpD.Stride) + (x * 4);
                                    bytes[idx] = col.B;
                                    bytes[idx+1] = col.G;
                                    bytes[idx+2] = col.R;
                                    bytes[idx+3] = col.A;
                                }
                            }
                        }
                    }

                    Marshal.Copy(bytes, 0, bmpD.Scan0, length);

                    bmp.UnlockBits(bmpD);
                }
                

                if (drawView)
                {
                    Rectangle dRect = ViewRectangle;
                    g.DrawRectangle(new Pen(Color.Blue), new Rectangle(dRect.X, dRect.Y, dRect.Width, dRect.Height));
                }
            }

            return bmp;
        }

        private Point selPt = new Point();

        private Rectangle overRect = new Rectangle(0, 0, 0, 0);
        private TileInfo overTile = null;

        protected override void OnMouseDown(MouseEventArgs e)
        {
            base.OnMouseDown(e);

            selPt = GetPointForPos(e.Location);

            if (e.Button == MouseButtons.Right)
            {
                StartTilePlacementEventArgs stpe = new StartTilePlacementEventArgs(selPt.X, selPt.Y);
                if (StartTilePlacement != null)
                {
                    StartTilePlacement.Invoke(this, stpe);
                }

                overTile = CreateTileInfoForObject(stpe.LinkedObject, stpe.DisplayBitmap);

                OnMouseMove(e);
            }
            else if (e.Button == MouseButtons.Left)
            {
                if ((Program.GetAsyncKeyState(0x10) & 0x8000) > 0)
                {
                    PlaceTile(selPt.X, selPt.Y, null, null);
                }
                else
                {
                    if (Tilemap[selPt.X, selPt.Y] != null)
                        SelectedTile = new Point(selPt.X, selPt.Y);
                    else
                        SelectedTile = null;
                }
            }
            else if (e.Button == MouseButtons.Middle)
            {
                if (MiddleClick != null) {
                    MiddleClickEventArgs mce = new MiddleClickEventArgs(Tilemap[selPt.X, selPt.Y]);
                    MiddleClick.Invoke(this, mce);
                }
            }
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);

            Point unselPt = GetPointForPos(e.Location);

            if (e.Button == MouseButtons.Right)
            {
                overRect = new Rectangle(Math.Min(selPt.X, unselPt.X),
                   Math.Min(selPt.Y, unselPt.Y),
                   Math.Abs(selPt.X - unselPt.X)+1,
                   Math.Abs(selPt.Y - unselPt.Y)+1);

            }
            else if (e.Button == MouseButtons.Left)
            {
                if((Program.GetAsyncKeyState(0x10) & 0x8000) > 0)
                    PlaceTile(unselPt.X, unselPt.Y, null, null);
            }

            this.Invalidate();
        }

        protected override void OnMouseUp(MouseEventArgs e)
        {
            base.OnMouseUp(e);

            if (e.Button == MouseButtons.Right)
            {
                for (int x = 0; x < overRect.Width; x++)
                {
                    for (int y = 0; y < overRect.Height; y++)
                    {
                        PlaceTile(overRect.X + x, overRect.Y + y, (overTile == null) ? null : overTile.LinkedObject, (overTile == null) ? null : overTile.DisplayBitmap);
                    }
                }

                overTile = null;
                overRect = new Rectangle(0, 0, 0, 0);

                this.Invalidate();
            }
        }

        protected override void OnMouseDoubleClick(MouseEventArgs e)
        {
            base.OnMouseDoubleClick(e);

            if(TileDoubleClick != null)
            {
                Point selPt = GetPointForPos(e.Location);
                StartTilePlacementEventArgs stpea = new StartTilePlacementEventArgs(selPt.X, selPt.Y);
                TileDoubleClick.Invoke(this, stpea);
            }
        }

        public class TileInfo
        {
            public object LinkedObject = null;

            private Bitmap displayBitmap = null;
            private Color col = Color.FromArgb(0, 0, 0, 0);

            public Bitmap DisplayBitmap
            {
                get => displayBitmap;
                set
                {
                    displayBitmap = value;
                    col = displayBitmap.GetPixel(displayBitmap.Width / 2, displayBitmap.Height / 2);
                }
            }

            public Color MiddleColor => col;

            public TileInfo(object linkedObject, Bitmap displayBitmap)
            {
                this.LinkedObject = linkedObject;
                this.DisplayBitmap = displayBitmap;
            }
        }

        public class MiddleClickEventArgs : EventArgs
        {
            public TileInfo TileInfo = null;

            public MiddleClickEventArgs(TileInfo tileInfo)
            {
                this.TileInfo = tileInfo;
            }
        }

        public class StartTilePlacementEventArgs : EventArgs
        {
            public readonly int TileX, TileY;
            public object LinkedObject = null;
            public Bitmap DisplayBitmap = null;

            public StartTilePlacementEventArgs(int TileX, int TileY)
            {
                this.TileX = TileX;
                this.TileY = TileY;
            }
        }
    }
}
