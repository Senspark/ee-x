using System.Threading.Tasks;

using UnityEngine.Scripting;

namespace EE {
    /// <summary>
    /// Base interface for all service classes.
    /// </summary>
    [Preserve]
    public interface IService {
        /// <summary>
        /// Asynchronously initializes this service.
        /// </summary>
        Task<bool> Initialize();

        /// <summary>
        /// Destroys this service.
        /// </summary>
        void Destroy();
    }
}