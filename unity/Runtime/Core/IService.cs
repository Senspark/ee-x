using System.Threading.Tasks;

namespace EE {
    /// <summary>
    /// Base interface for all service classes.
    /// </summary>
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