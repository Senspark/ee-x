using System.Threading.Tasks;

using UnityEngine;
using UnityEngine.Assertions;
using UnityEngine.SceneManagement;

namespace EE {
    public class DefaultSceneLoader : ISceneLoader {
        public Task<bool> Initialize() => Task.FromResult(true);

        public void Destroy() {
        }

        public async Task<T> LoadScene<T>(string sceneName) where T : MonoBehaviour {
            await SceneManager.LoadSceneAsync(sceneName, LoadSceneMode.Single);
            var item = Object.FindObjectOfType<T>();
            Assert.IsNotNull(item);
            return item;
        }
    }
}