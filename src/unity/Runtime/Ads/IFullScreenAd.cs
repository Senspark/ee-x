using System.Threading.Tasks;

namespace EE {
    public enum AdResult {
        /// Used by DefaultFullScreenAd.
        Failed,
        Canceled,
        Completed,

        /// Used by LazyFullScreenAd.
        NotInitialized,
        NotConfigured,

        /// Used by GenericAd.
        Capped,
        NoInternet,
        NotLoaded,
    }

    public interface IFullScreenAd : IAd {
        /// <summary>
        /// Asynchronously shows this ad.
        /// </summary>
        Task<AdResult> Show();
    }
}