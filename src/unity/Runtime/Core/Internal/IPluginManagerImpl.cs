namespace EE.Internal {
    internal interface IPluginManagerImpl {
        bool InitializePlugins(string version);
        void SetLogLevel(LogLevel level);
        object GetActivity();
        void SetActivity(object activity);
        bool AddPlugin(string name);
        bool RemovePlugin(string name);
    }
}