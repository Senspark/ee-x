namespace EE.Internal {
    internal class PluginManagerImplEditor : IPluginManagerImpl {
        public bool InitializePlugins() {
            return false;
        }

        public object GetActivity() {
            return null;
        }

        public void SetActivity(object activity) {
        }

        public bool AddPlugin(string name) {
            return false;
        }

        public bool RemovePlugin(string name) {
            return false;
        }
    }
}