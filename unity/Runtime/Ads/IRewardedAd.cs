using System;
using System.Threading.Tasks;

namespace EE {
    public enum IRewardedAdResult {
        /// Failed to display the ad.
        Failed,

        /// Succeeded to display the ad and the user has canceled the ad.
        Canceled,

        /// Succeeded to display the ad and the user has completed the ad.
        Completed,
    }

    public class IRewardedAdObserver {
        /// <summary>
        /// Occurs when this ad is loaded.
        /// </summary>
        public Action OnLoaded { get; set; }

        /// <summary>
        /// Occurs when user clicks this ad.
        /// </summary>
        public Action OnClicked { get; set; }
    }

    public interface IRewardedAd : IObserverManager<IRewardedAdObserver> {
        /// <summary>
        /// Destroys this ad.
        /// </summary>
        void Destroy();

        /// <summary>
        /// Checks whether this ad is loaded.
        /// </summary>
        bool IsLoaded { get; }

        /// <summary>
        /// Asynchronously loads this ad.
        /// </summary>
        Task<bool> Load();

        /// <summary>
        /// Asynchronously shows this ad.
        /// </summary>
        Task<IRewardedAdResult> Show();
    }
}