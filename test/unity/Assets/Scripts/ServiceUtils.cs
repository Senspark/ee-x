using System.Threading.Tasks;

namespace EETest {
    public static class ServiceUtils {
        public static async Task Initialize() {
            EE.PluginManager.InitializePlugins();
            var logManager = new EE.FirebaseLogManager();
            var analyticsManager = new EE.FirebaseAnalyticsManager();
            var sceneLoader = new EE.DefaultSceneLoader();
            await logManager.Initialize();
            await analyticsManager.Initialize();
            await sceneLoader.Initialize();
            EE.ServiceLocator.Provide(logManager);
            EE.ServiceLocator.Provide(analyticsManager);
            EE.ServiceLocator.Provide(sceneLoader);
        }
    }
}