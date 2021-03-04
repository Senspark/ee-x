using System;
using System.Linq;
using System.Reflection;

using UnityEngine.Assertions;

namespace EE.Internal {
    internal class JsonDotNetConverter : IJsonConverter {
        private MethodInfo _methodDeserializeObject;
        private MethodInfo _methodSerializeObject;

        public bool Initialize() {
            var type = Type.GetType("Newtonsoft.Json.JsonConvert, Newtonsoft.Json");
            if (type == null) {
                return false;
            }
            _methodDeserializeObject = type.GetMethods()
                .First(item => item.IsGenericMethod &&
                               item.Name == "DeserializeObject" &&
                               item.GetParameters().Length == 1 &&
                               item.GetParameters()[0].ParameterType == typeof(string));
            _methodSerializeObject = type.GetMethod("SerializeObject", new[] {typeof(object)});
            Assert.IsNotNull(_methodDeserializeObject);
            Assert.IsNotNull(_methodSerializeObject);
            return true;
        }

        public T Deserialize<T>(string value) {
            var result = _methodDeserializeObject.MakeGenericMethod(typeof(T)).Invoke(null, new object[] {value});
            return (T) result;
        }

        public string Serialize<T>(T value) {
            var result = _methodSerializeObject.Invoke(null, new object[] {value});
            return (string) result;
        }
    }
}