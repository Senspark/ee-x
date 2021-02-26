using System.Threading.Tasks;

using UnityEngine;

namespace EE {
    public class DefaultDataManager : IDataManager {
        public Task<bool> Initialize() => Task.FromResult(true);

        public void Destroy() {
        }

        public T Get<T>(string key, T defaultValue) {
            var str = PlayerPrefs.GetString(key, "");
            return str == "" ? defaultValue : JsonUtility.FromJson<T>(str);
        }

        public void Set<T>(string key, T value) {
            var str = JsonUtility.ToJson(value);
            PlayerPrefs.SetString(key, str);
        }
    }
}