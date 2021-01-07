namespace EE {
    public interface IAdView : IAd {
        /// <summary>
        /// Gets or sets the anchor of this ad.
        /// </summary>
        (float, float) Anchor { get; set; }

        /// <summary>
        /// Gets or set the position of this ad.
        /// </summary>
        (float, float) Position { get; set; }

        /// <summary>
        /// Gets or set the size of this ad.
        /// </summary>
        (float, float) Size { get; set; }

        /// <summary>
        /// Gets or set the visibility of this ad.
        /// </summary>
        bool IsVisible { get; set; }
    }
}