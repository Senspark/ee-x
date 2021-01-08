using System.Runtime.InteropServices;

namespace EE.Internal {
    internal class PluginManagerImplIos : IPluginManagerImpl {
        [DllImport("__Internal")]
        private static extern void ee_staticInitializePlugins(string version);

        [DllImport("__Internal")]
        private static extern void ee_staticSetLogLevel(int level);

        [DllImport("__Internal")]
        private static extern object ee_staticGetActivity();

        [DllImport("__Internal")]
        private static extern void ee_staticSetActivity(object activity);

        [DllImport("__Internal")]
        private static extern void ee_staticAddPlugin(string name);

        [DllImport("__Internal")]
        private static extern void ee_staticRemovePlugin(string name);

        public void InitializePlugins(string version) {
            ee_staticInitializePlugins(version);
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

        public void AddPlugin(string name) {
            ee_staticAddPlugin(name);
        }

        public void RemovePlugin(string name) {
            ee_staticRemovePlugin(name);
        }
    }
}