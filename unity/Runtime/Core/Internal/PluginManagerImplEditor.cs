namespace EE.Internal {
    internal class PluginManagerImplEditor : IPluginManagerImpl {
        public bool InitializePlugins() {
            return true;
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