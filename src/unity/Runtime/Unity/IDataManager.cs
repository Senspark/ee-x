namespace EE {
    [Service("EEx_IDataManager")]
    public interface IDataManager : IService {
        /// <summary>
        /// Gets a value by key.
        /// </summary>
        /// <param name="key">The desired key</param>
        /// <param name="defaultValue">The default value returned if the key doesn't exist</param>
        /// <typeparam name="T">The type of value</typeparam>
        /// <returns>Value</returns>
        T Get<T>(string key, T defaultValue);

        /// <summary>
        /// Sets value by key.
        /// </summary>
        /// <param name="key">The desired key</param>
        /// <param name="value">The desired value</param>
        /// <typeparam name="T">The type of value</typeparam>
        void Set<T>(string key, T value);
    }
}