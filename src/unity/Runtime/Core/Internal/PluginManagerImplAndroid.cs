using UnityEngine;

namespace EE.Internal {
    internal class PluginManagerImplAndroid : IPluginManagerImpl {
        private readonly AndroidJavaClass _clazz = new AndroidJavaClass("com.ee.internal.UnityPluginManager");

        public bool InitializePlugins() {
            using (var unityPlayer = new AndroidJavaClass("com.unity3d.player.UnityPlayer")) {
                using (var activity = unityPlayer.GetStatic<AndroidJavaObject>("currentActivity")) {
                    SetActivity(activity);
                }
            }
            return _clazz.CallStatic<bool>("initializePlugins");
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

        public bool AddPlugin(string name) {
            return _clazz.CallStatic<bool>("addPlugin", name);
        }

        public bool RemovePlugin(string name) {
            return _clazz.CallStatic<bool>("removePlugin", name);
        }
    }
}