using UnityEngine;

namespace EE.Internal {
    internal class PluginManagerImplAndroid : IPluginManagerImpl {
        private readonly AndroidJavaClass _clazz = new AndroidJavaClass("com.ee.internal.UnityPluginManager");

        public void InitializePlugins(string version) {
            using (var unityPlayer = new AndroidJavaClass("com.unity3d.player.UnityPlayer")) {
                using (var activity = unityPlayer.GetStatic<AndroidJavaObject>("currentActivity")) {
                    SetActivity(activity);
                }
            }
            _clazz.CallStatic("initializePlugins", version);
        }

        public void SetLogLevel(LogLevel level) {
            _clazz.CallStatic("setLogLevel", (int) level);
        }

        public object GetActivity() {
            return _clazz.CallStatic<object>("getActivity");
        }

        public void SetActivity(object activity) {
            _clazz.CallStatic("setActivity", activity);
        }

        public void AddPlugin(string name) {
            _clazz.CallStatic("addPlugin", name);
        }

        public void RemovePlugin(string name) {
            _clazz.CallStatic("removePlugin", name);
        }
    }
}