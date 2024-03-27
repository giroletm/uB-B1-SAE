using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SAE202
{
    public class Biome
    {
        public string name = "Sans nom";
        public Color color = Color.LightGray;
        public float incomingWeight = 0.0f;
        public float insideWeight = 0.0f;
    }

    public class Polygon
    {
        public List<Point2D> points = new List<Point2D>();
        public Biome biome = null;

        public bool collisionPoint(Point2D pt) // http://www.jeffreythompson.org/collision-detection/poly-point.php
        {
            bool collision = false;

            // go through each of the vertices, plus
            // the next vertex in the list
            int next = 0;
            for (int current = 0; current < points.Count; current++)
            {

                // get next vertex in list
                // if we've hit the end, wrap around to 0
                next = current + 1;
                if (next == points.Count) next = 0;

                // get the PVectors at our current position
                // this makes our if statement a little cleaner
                Point2D vc = points[current];    // c for "current"
                Point2D vn = points[next];       // n for "next"

                // compare position, flip 'collision' variable
                // back and forth
                if (((vc.Y >= pt.Y && vn.Y < pt.Y) || (vc.Y < pt.Y && vn.Y >= pt.Y)) &&
                    (pt.X < (vn.X - vc.X) * (pt.Y - vc.Y) / (vn.Y - vc.Y) + vc.X))
                {
                    collision = !collision;
                }
            }
            return collision;
        }

    }

    public class Point2D
    {
        public float X = 0.0f;
        public float Y = 0.0f;

        public Point2D() { }
        public Point2D(float x, float y)
        {
            this.X = x;
            this.Y = y;
        }
    }

    public static class Utils
    {
        public static bool LoadWorldmap(byte[] data, out List<Polygon> polygons, out List<Biome> biomes, out int defaultBiomeIdx)
        {
            polygons = new List<Polygon>();
            biomes = new List<Biome>();
            defaultBiomeIdx = -1;

            int currIdx = 0;

            uint magic = data.GetUInt32(currIdx);
            currIdx += 4;

            if (magic != 0x4250414Du)
                return false;

            defaultBiomeIdx = data.GetInt32(currIdx);
            currIdx += 4;


            int idxPolyTable = (int)data.GetUInt64(currIdx);
            currIdx += 8;

            int idxBiomeTable = (int)data.GetUInt64(currIdx);
            currIdx += 8;


            currIdx = idxBiomeTable;

            int biomeCount = data.GetInt32(currIdx);
            currIdx += 4;

            for (int i = 0; i < biomeCount; i++)
            {
                Biome b = new Biome();

                int biomeIdx = (int)data.GetUInt64(currIdx);
                currIdx += 8;

                b.name = data.GetString((int)data.GetUInt64(biomeIdx));
                biomeIdx += 8;

                b.color = Color.FromArgb(data[biomeIdx + 3], data[biomeIdx + 0], data[biomeIdx + 1], data[biomeIdx + 2]);
                biomeIdx += 4;

                b.incomingWeight = data.GetFloat(biomeIdx);
                biomeIdx += 4;

                b.insideWeight = data.GetFloat(biomeIdx);
                biomeIdx += 4;

                biomes.Add(b);
            }


            currIdx = idxPolyTable;

            int polyCount = data.GetInt32(currIdx);
            currIdx += 4;

            for (int i = 0; i < polyCount; i++)
            {
                Polygon p = new Polygon();

                int polyIdx = (int)data.GetUInt64(currIdx);
                currIdx += 8;

                int biomeNum = data.GetInt32(polyIdx);
                p.biome = (biomeNum < 0) ? null : biomes[biomeNum];
                polyIdx += 4;

                p.points = new List<Point2D>();

                int ptsCount = data.GetInt32(polyIdx);
                polyIdx += 4;
                for (int j = 0; j < ptsCount; j++)
                {
                    Point2D point = new Point2D();

                    int ptIdx = (int)data.GetUInt64(polyIdx);
                    polyIdx += 8;

                    point.X = data.GetFloat(ptIdx);
                    ptIdx += 4;

                    point.Y = data.GetFloat(ptIdx);
                    ptIdx += 4;

                    p.points.Add(point);
                }

                polygons.Add(p);
            }

            return true;
        }

        public static byte[] SaveWorldmap(List<Polygon> polygons, List<Biome> biomes, int defaultBiomeIdx)
        {
            List<byte> data = new List<byte>();

            data.AddUInt32(0x4250414Du); // Magic

            data.AddInt32(defaultBiomeIdx); // Default biome

            int idxPolyTable = data.Count;
            data.AddUInt64(0); // Index de la liste de polygones

            int idxBiomeTable = data.Count;
            data.AddUInt64(0); // Index de la liste de biomes

            data.InsertInt64(idxPolyTable, data.Count);
            data.AddInt32(polygons.Count);
            int polyIdx = data.Count;
            foreach (Polygon polygon in polygons)
            {
                data.AddUInt64(0);
            }
            foreach (Polygon polygon in polygons)
            {
                data.InsertInt64(polyIdx, data.Count);
                polyIdx += 8;

                data.AddInt32(biomes.IndexOf(polygon.biome));

                data.AddInt32(polygon.points.Count);
                int pointIdx = data.Count;
                foreach (Point2D point in polygon.points)
                {
                    data.AddUInt64(0);
                }
                foreach (Point2D point in polygon.points)
                {
                    data.InsertInt64(pointIdx, data.Count);
                    pointIdx += 8;

                    data.AddFloat(point.X);
                    data.AddFloat(point.Y);
                }
            }

            data.InsertInt64(idxBiomeTable, data.Count);
            data.AddInt32(biomes.Count);
            int biomeIdx = data.Count;
            foreach (Biome biome in biomes)
            {
                data.AddUInt64(0);
            }
            int stringIdx = data.Count;
            foreach (Biome biome in biomes)
            {
                data.InsertInt64(biomeIdx, data.Count);
                biomeIdx += 8;

                data.AddUInt64(0);
                data.AddUInt32(((uint)biome.color.A << 24) | ((uint)biome.color.B << 16) | ((uint)biome.color.G << 8) | ((uint)biome.color.R));
                data.AddFloat(biome.incomingWeight);
                data.AddFloat(biome.insideWeight);
            }

            foreach (Biome biome in biomes)
            {
                data.InsertInt64(stringIdx, data.Count);
                stringIdx += 20;

                data.AddString(biome.name);
            }


            return data.ToArray();
        }
    }

    public static class ByteArrayExtensiosn
    {
        public static string GetString(this byte[] bytes, int idx)
        {
            string output = "";

            int currIdx = idx;
            while (bytes[currIdx] != 0)
            {
                output += (char)bytes[currIdx];
                currIdx++;
            }

            return output;
        }

        public static float GetFloat(this byte[] bytes, int idx)
        {
            return System.BitConverter.ToSingle(bytes, idx);
        }

        public static long GetInt64(this byte[] bytes, int idx)
        {
            return (long)bytes.GetUInt64(idx);
        }

        public static ulong GetUInt64(this byte[] bytes, int idx)
        {
            return ((ulong)bytes[idx]) | ((ulong)bytes[idx + 1] << 8) | ((ulong)bytes[idx + 2] << 16) | ((ulong)bytes[idx + 3] << 24) | ((ulong)bytes[idx + 4] << 32) | ((ulong)bytes[idx + 5] << 40) | ((ulong)bytes[idx + 6] << 48) | ((ulong)bytes[idx + 7] << 56);
        }

        public static int GetInt32(this byte[] bytes, int idx)
        {
            return (int)bytes.GetUInt32(idx);
        }

        public static uint GetUInt32(this byte[] bytes, int idx)
        {
            return ((uint)bytes[idx]) | ((uint)bytes[idx + 1] << 8) | ((uint)bytes[idx + 2] << 16) | ((uint)bytes[idx + 3] << 24);
        }

        public static short GetInt16(this byte[] bytes, int idx)
        {
            return (short)bytes.GetUInt16(idx);
        }

        public static ushort GetUInt16(this byte[] bytes, int idx)
        {
            return (ushort)(((uint)bytes[idx]) | ((uint)bytes[idx + 1] << 8));
        }
    }

    public static class ByteListExtensions {
        public static void AddString(this List<byte> bytes, string item)
        {
            foreach (char c in item)
            {
                bytes.Add((byte)(c & 0xFF));
            }
            bytes.Add(0);
        }

        public static void AddFloat(this List<byte> bytes, float item)
        {
            foreach (byte b in BitConverter.GetBytes(item))
            {
                bytes.Add(b);
            }
        }

        public static void InsertFloat(this List<byte> bytes, int index, float item)
        {
            int i = index;
            foreach (byte b in BitConverter.GetBytes(item))
            {
                bytes[i] = b;
                i++;
            }
        }

        public static void AddInt64(this List<byte> bytes, long item)
        {
            bytes.AddUInt64((uint)item);
        }

        public static void InsertInt64(this List<byte> bytes, int index, long item)
        {
            bytes.InsertUInt64(index, (uint)item);
        }

        public static void AddUInt64(this List<byte> bytes, ulong item)
        {
            bytes.Add((byte)(item & 0xFF));
            bytes.Add((byte)((item >> 8) & 0xFF));
            bytes.Add((byte)((item >> 16) & 0xFF));
            bytes.Add((byte)((item >> 24) & 0xFF));
            bytes.Add((byte)((item >> 32) & 0xFF));
            bytes.Add((byte)((item >> 40) & 0xFF));
            bytes.Add((byte)((item >> 48) & 0xFF));
            bytes.Add((byte)((item >> 56) & 0xFF));
        }

        public static void InsertUInt64(this List<byte> bytes, int index, ulong item)
        {
            bytes[index] = (byte)(item & 0xFF);
            bytes[index + 1] = (byte)((item >> 8) & 0xFF);
            bytes[index + 2] = (byte)((item >> 16) & 0xFF);
            bytes[index + 3] = (byte)((item >> 24) & 0xFF);
            bytes[index + 4] = (byte)((item >> 32) & 0xFF);
            bytes[index + 5] = (byte)((item >> 40) & 0xFF);
            bytes[index + 6] = (byte)((item >> 48) & 0xFF);
            bytes[index + 7] = (byte)((item >> 56) & 0xFF);
        }

        public static void AddInt32(this List<byte> bytes, int item)
        {
            bytes.AddUInt32((uint)item);
        }

        public static void InsertInt32(this List<byte> bytes, int index, int item)
        {
            bytes.InsertUInt32(index, (uint)item);
        }

        public static void AddUInt32(this List<byte> bytes, uint item)
        {
            bytes.Add((byte)(item & 0xFF));
            bytes.Add((byte)((item >> 8) & 0xFF));
            bytes.Add((byte)((item >> 16) & 0xFF));
            bytes.Add((byte)((item >> 24) & 0xFF));
        }

        public static void InsertUInt32(this List<byte> bytes, int index, uint item)
        {
            bytes[index] = (byte)(item & 0xFF);
            bytes[index + 1] = (byte)((item >> 8) & 0xFF);
            bytes[index + 2] = (byte)((item >> 16) & 0xFF);
            bytes[index + 3] = (byte)((item >> 24) & 0xFF);
        }

        public static void AddInt16(this List<byte> bytes, short item)
        {
            bytes.AddUInt16((ushort)item);
        }

        public static void InsertInt16(this List<byte> bytes, int index, short item)
        {
            bytes.InsertUInt16(index, (ushort)item);
        }

        public static void AddUInt16(this List<byte> bytes, uint item)
        {
            bytes.Add((byte)(item & 0xFF));
            bytes.Add((byte)((item >> 8) & 0xFF));
        }

        public static void InsertUInt16(this List<byte> bytes, int index, ushort item)
        {
            bytes[index] = (byte)(item & 0xFF);
            bytes[index + 1] = (byte)((item >> 8) & 0xFF);
        }
    }
}
