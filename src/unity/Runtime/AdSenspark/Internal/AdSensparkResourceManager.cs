using System;
using System.Threading.Tasks;

using UnityEngine;

namespace EE.Internal {
    internal class AdSensparkResourceManager {
        private bool _initialized = false;
        private AdSensparkJsonData _adSensparkJsonData = new AdSensparkJsonData();
        /// <summary>
        /// Khởi tạo dữ liệu.
        /// </summary>
        /// <param name="url">Đường dẫn để tải file json</param>
        /// <returns></returns>
        public async Task<bool> Initialize(string url) {
            if(_initialized) {
                Debug.LogWarning("Ad senspark: Gọi một lần thôi.");
                return false;
            }
            var jsonData = await Downloader.Load("ad_senspark_file_config", url);
            if (jsonData.Length <= 0) {
                Debug.LogWarning("Ad senspark: không load được file json ");
                return false;
            }
            var jsonBase64String = System.Text.Encoding.UTF8.GetString(jsonData);
            _adSensparkJsonData = JsonUtility.FromJson<AdSensparkJsonData>(jsonBase64String);
            _initialized = true;
            return true;
        }
        /// <summary>
        /// Lấy tài nguyên dạng byte array
        /// </summary>
        /// <param name="adFormat"></param>
        /// <returns></returns>
        /// <exception cref="ArgumentOutOfRangeException"></exception>
        public async Task<Byte[]> GetResource(AdFormat adFormat) {
            string link = "";
            string fileName = "";
            switch (adFormat) {
                case AdFormat.Banner:
                    break;

                case AdFormat.Rectangle:
                    break;

                case AdFormat.AppOpen:
                    break;

                case AdFormat.Interstitial:
                    break;

                case AdFormat.RewardedInterstitial:
                    break;

                case AdFormat.Rewarded:
                    break;

                default:
                    throw new ArgumentOutOfRangeException(nameof(adFormat), adFormat, null);
            }
            return await Downloader.Load(fileName, link);
        }
    }
}