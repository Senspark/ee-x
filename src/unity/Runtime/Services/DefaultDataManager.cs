using System.Threading.Tasks;

using EE.Internal;

using UnityEngine;

namespace EE {
    public class DefaultDataManager : IDataManager {
        private IJsonConverter _impl;

        public Task<bool> Initialize() {
            var jsonDotNet = new JsonDotNetConverter();
            if (jsonDotNet.Initialize()) {
                _impl = jsonDotNet;
                return Task.FromResult(true);
            }
            var jsonUnity = new JsonUnityConverter();
            if (jsonUnity.Initialize()) {
                _impl = jsonUnity;
                return Task.FromResult(true);
            }
            return Task.FromResult(false);
        }

        public void Destroy() {
        }

        public T Get<T>(string key, T defaultValue) {
            var str = PlayerPrefs.GetString(key, "");
            return str == "" ? defaultValue : _impl.Deserialize<T>(str);
        }

        public void Set<T>(string key, T value) {
            var str = _impl.Serialize(value);
            PlayerPrefs.SetString(key, str);
        }
    }
}