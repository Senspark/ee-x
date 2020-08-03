using UnityEngine;

namespace EE.Internal {
    internal class PluginManagerImplAndroid : IPluginManagerImpl {
        private readonly AndroidJavaClass _clazz = new AndroidJavaClass("com.ee.PluginManagerKt");

        public bool InitializePlugins() {
            return _clazz.CallStatic<bool>("ee_staticInitializePlugins");
        }

        public object GetActivity() {
            return _clazz.CallStatic<object>("ee_staticGetActivity");
        }

        public void SetActivity(object activity) {
            _clazz.CallStatic("ee_staticSetActivity", activity);
        }

        public bool AddPlugin(string name) {
            return _clazz.CallStatic<bool>("ee_staticAddPlugin", name);
        }

        public bool RemovePlugin(string name) {
            return _clazz.CallStatic<bool>("ee_staticRemovePlugin", name);
        }
    }
}