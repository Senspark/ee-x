using System.Threading.Tasks;

namespace EE {
    public enum FullScreenAdResult {
        /// Failed to display the ad.
        Failed,

        /// Succeeded to display the ad and the user has canceled the ad.
        Canceled,

        /// Succeeded to display the ad and the user has completed the ad.
        Completed,
    }

    public interface IFullScreenAd : IAd {
        /// <summary>
        /// Asynchronously shows this ad.
        /// </summary>
        Task<FullScreenAdResult> Show();
    }
}