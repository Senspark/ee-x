using System.Threading.Tasks;

using UnityEngine;
using UnityEngine.Scripting;

namespace EE {
    [Preserve]
    [Service(nameof(ISceneLoader))]
    public interface ISceneLoader : IService {
        Task<T> LoadScene<T>(string sceneName) where T : MonoBehaviour;
    }
}