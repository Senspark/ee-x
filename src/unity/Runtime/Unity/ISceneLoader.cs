using System.Threading.Tasks;

using UnityEngine;

namespace EE {
    [Service(nameof(ISceneLoader))]
    public interface ISceneLoader : IService {
        Task<T> LoadScene<T>(string sceneName) where T : MonoBehaviour;
    }
}