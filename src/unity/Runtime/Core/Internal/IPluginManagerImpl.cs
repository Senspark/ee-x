namespace EE.Internal {
    internal interface IPluginManagerImpl {
        bool InitializePlugins();
        void SetLogLevel(LogLevel level);
        object GetActivity();
        void SetActivity(object activity);
        bool AddPlugin(string name);
        bool RemovePlugin(string name);
    }
}