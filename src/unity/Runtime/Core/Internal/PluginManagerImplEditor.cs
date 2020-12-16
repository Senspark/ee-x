namespace EE.Internal {
    internal class PluginManagerImplEditor : IPluginManagerImpl {
        public bool InitializePlugins(string version) {
            return true;
        }

        public void SetLogLevel(LogLevel level) {
        }

        public object GetActivity() {
            return null;
        }

        public void SetActivity(object activity) {
        }

        public bool AddPlugin(string name) {
            return true;
        }

        public bool RemovePlugin(string name) {
            return true;
        }
    }
}