using System;
using System.Threading.Tasks;

namespace EE {
    public class IInterstitialAdObserver {
        /// <summary>
        /// Occurs when this ad is loaded.
        /// </summary>
        public Action OnLoaded { get; set; }

        /// <summary>
        /// Occurs when user clicks this ad.
        /// </summary>
        public Action OnClicked { get; set; }
    }

    public interface IInterstitialAd : IObserverManager<IInterstitialAdObserver> {
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
        Task<bool> Show();
    }
}