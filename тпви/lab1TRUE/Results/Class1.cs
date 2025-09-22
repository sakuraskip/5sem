using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Text.Json;

namespace BSTU.Results.Collection
{
    public class Results
    {
        private string _jsonFilePath;
        private Dictionary<int, string> _results;
        private bool _isBlocked = false;
        private int _maxKey = 0;

        public Results(string jsonFilePath)
        {
            _jsonFilePath = jsonFilePath;
            if(File.Exists(jsonFilePath))
            {
                var jsontext = File.ReadAllText(_jsonFilePath);
                _results = JsonSerializer.Deserialize<Dictionary<int, string>>(jsontext);
                if(_results==null)
                {
                    _results = new Dictionary<int, string>();
                }
                if (_results.Count > 0)
                {
                    _maxKey = _results.Keys.Max();
                }
                else _maxKey = 0;
            }
            else
            {
                _results = new Dictionary<int, string>();
            }
        }
        public Dictionary<int, string> GetAllResults()
        {
            _isBlocked = true;
            try
            { return new Dictionary<int, string>(_results); }
            finally { _isBlocked = false; }
        }
        public string GetOneResults(int key)
        {
            _isBlocked = true;
            try
            {
                if(_results.ContainsKey(key))
                {
                    return _results[key];
                }
                return string.Empty;
            }
            finally
            {
                _isBlocked = false;
            }
        }
        public int AddOneResult(string value)
        {
            _isBlocked = true;
            try
            {
                int key = _maxKey++;
                _results[key] = value;
                //СОХРАНЕНИЕ СДЕЛАТЬ
                return key;
            }
            finally
            {
                _isBlocked = false;
            }
        }
        public int UpdateOneResult(int key,string newvalue)
        {
            _isBlocked = true;
            try
            {
                if( _results.ContainsKey(key))
                {
                    _results[key] = newvalue;
                    //СОХРАНЕНИЕ СДЕЛАТЬ
                    return key;
                }
                return -1;
            }
            finally
            {
                _isBlocked = false;
            }
        }
        public int DeleteOneResult(int key)
        {
            _isBlocked = true;
            try
            {
                if(!_results.ContainsKey(key))
                {
                    return -1;
                }
                _results.Remove(key);
                //СОХРАНЕНИЕ СДЕЛАТЬ
                return key;
            }
            finally
            {
                _isBlocked = false;
            }
        }
        private bool SaveToJson()
        {
            return true;
        }
    }
}
