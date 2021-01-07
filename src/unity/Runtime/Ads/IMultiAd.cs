namespace EE {
    public interface IMultiAd<in Ad> {
        /// <summary>
        /// Adds an ad item to this multi-ad instance.
        /// </summary>
        void AddItem(Ad item);
    }
}