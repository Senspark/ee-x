using System;
using System.Threading.Tasks;

namespace EE {
    public class AdLoadResult {
        public string Network;
        public bool Result;
        public int ErrorCode;
        public string ErrorMessage;
    }

    public class AdObserver {
        /// <summary>
        /// Occurs when this ad is loaded.
        /// </summary>
        public Action OnLoaded { get; set; }

        /// <summary>
        /// Occurs when this ad finishes loading.
        /// </summary>
        public Action<AdLoadResult> OnLoadResult { get; set; }

        /// <summary>
        /// Occurs when user clicks this ad.
        /// </summary>
        public Action OnClicked { get; set; }
    }

    public interface IAd : IObserverManager<AdObserver> {
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
    }
}