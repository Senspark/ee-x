using System;
using System.Drawing;
using System.Threading.Tasks;

namespace EE {
    public class IAdViewObserver {
        /// <summary>
        /// Occurs when this ad is loaded.
        /// </summary>
        public Action OnLoaded { get; set; }

        /// <summary>
        /// Occurs when user clicks this ad.
        /// </summary>
        public Action OnClicked { get; set; }
    }

    public interface IAdView : IObserverManager<IAdViewObserver> {
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
        /// Gets or sets the anchor of this ad.
        /// </summary>
        PointF Anchor { get; set; }

        /// <summary>
        /// Gets or set the position of this ad.
        /// </summary>
        PointF Position { get; set; }

        /// <summary>
        /// Gets or set the size of this ad.
        /// </summary>
        SizeF Size { get; set; }

        /// <summary>
        /// Gets or set the visibility of this ad.
        /// </summary>
        bool IsVisible { get; set; }
    }
}