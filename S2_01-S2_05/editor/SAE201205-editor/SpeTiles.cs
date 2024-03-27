using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Json;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace SAE201205_editor
{
    public class SpeTiles
    {
        public List<TileType> tileTypes = new List<TileType>();

        public class TileType
        {
            public string name = "";
            public int id = 0;
            public List<Setting> settings = new List<Setting>();
        }

        public class Setting
        {
            public string iName = "";
            public string dName = "";
            public string type = "";
        }

        private SpeTiles()
        {
            tileTypes = new List<TileType>();
        }

        private SpeTiles(string json)
        {
            JObject jo = JObject.Parse(json);
            JToken root = jo.Root;

            foreach(JToken jspetile in root.SelectToken("spetiles"))
            {
                TileType tileType = new TileType();

                tileType.name = jspetile.SelectToken("Name").ToObject<string>();
                tileType.id = jspetile.SelectToken("ID").ToObject<int>();
                tileType.settings = new List<Setting>();

                foreach(JToken jsetting in jspetile.SelectToken("Settings"))
                {
                    Setting setting = new Setting();
                    setting.iName = jsetting.SelectToken("IName").ToObject<string>();
                    setting.dName = jsetting.SelectToken("DName").ToObject<string>();
                    setting.type = jsetting.SelectToken("Type").ToObject<string>();

                    tileType.settings.Add(setting);
                }

                tileTypes.Add(tileType);
            }
        }

        public static SpeTiles FromFile(string path)
        {
            try
            {
                return new SpeTiles(File.ReadAllText(path));
            }
            catch (Exception e)
            {
                return null;
            }
        }

        public static SpeTiles FromJSON(string json)
        {
            try
            {
                return new SpeTiles(json);
            }
            catch (Exception e)
            {
                return null;
            }
        }

        public static SpeTiles FromEmpty()
        {
            return new SpeTiles();
        }

        public bool ToFile(string path)
        {
            try
            {
                JObject root = new JObject();

                JArray jspetiles = new JArray();
                foreach(TileType tileType in tileTypes)
                {
                    JObject jspetile = new JObject();
                    jspetile.Add("Name", tileType.name);
                    jspetile.Add("ID", tileType.id);

                    JArray jsettings = new JArray();
                    foreach(Setting setting in tileType.settings)
                    {
                        JObject jsetting = new JObject();
                        jsetting.Add("IName", setting.iName);
                        jsetting.Add("DName", setting.dName);
                        jsetting.Add("Type", setting.type);

                        jsettings.Add(jsetting);
                    }

                    jspetile.Add("Settings", jsettings);

                    jspetiles.Add(jspetile);
                }

                root.Add("spetiles", jspetiles);

                File.WriteAllText(path, root.ToString());

                return true;
            }
            catch (Exception e)
            {
                return false;
            }
        }
    }

}
