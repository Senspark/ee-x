using System.Collections.Generic;
using System.Linq;

using Jsonite;

using UnityEngine.Assertions;

namespace EE.Internal {
    internal static class FirebaseUtils {
        public static Dictionary<string, object> ParseDefaults(string text) {
            var node = (JsonObject) Json.Deserialize(text);
            Assert.IsNotNull(node);
            return node.ToDictionary(
                entry => entry.Key,
                entry => {
                    var value = entry.Value;
                    switch (value) {
                        case JsonObject item: {
                            return item.ToString();
                        }
                        case JsonArray item: {
                            return item.ToString();
                        }
                    }
                    return value;
                });
        }
    }
}