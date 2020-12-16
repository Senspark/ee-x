using System.Runtime.InteropServices;

namespace EE.Internal {
    internal class PluginManagerImplIos : IPluginManagerImpl {
        [DllImport("__Internal")]
        private static extern bool ee_staticInitializePlugins(string version);

        [DllImport("__Internal")]
        private static extern void ee_staticSetLogLevel(int level);

        [DllImport("__Internal")]
        private static extern object ee_staticGetActivity();

        [DllImport("__Internal")]
        private static extern void ee_staticSetActivity(object activity);

        [DllImport("__Internal")]
        private static extern bool ee_staticAddPlugin(string name);

        [DllImport("__Internal")]
        private static extern bool ee_staticRemovePlugin(string name);

        public bool InitializePlugins(string version) {
            return ee_staticInitializePlugins(version);
        }

        public void SetLogLevel(LogLevel level) {
            ee_staticSetLogLevel((int) level);
        }

        public object GetActivity() {
            return ee_staticGetActivity();
        }

        public void SetActivity(object activity) {
            ee_staticSetActivity(activity);
        }

        public bool AddPlugin(string name) {
            return ee_staticAddPlugin(name);
        }

        public bool RemovePlugin(string name) {
            return ee_staticRemovePlugin(name);
        }
    }
}