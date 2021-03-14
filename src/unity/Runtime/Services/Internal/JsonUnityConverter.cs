using UnityEngine;

namespace EE.Internal {
    internal class JsonUnityConverter : IJsonConverter {
        public bool Initialize() {
            return true;
        }

        public T Deserialize<T>(string value) {
            return JsonUtility.FromJson<T>(value);
        }

        public string Serialize<T>(T value) {
            return JsonUtility.ToJson(value);
        }
    }
}