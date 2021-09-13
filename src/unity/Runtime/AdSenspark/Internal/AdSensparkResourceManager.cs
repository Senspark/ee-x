using System;
using System.Collections.Generic;
using System.Threading.Tasks;

using UnityEngine;

namespace EE.Internal {
    internal class AdSensparkResourcePack {
        /// <summary>
        /// Tài nguyên load từ server.
        /// </summary>
        public Byte[] data = new byte[0];
        /// <summary>
        /// Tài nguyên để trong thư mục resource của Unity.
        /// </summary>
        public string dataLocalPath = "";
        /// <summary>
        /// Đường dẫn khi user click ad.
        /// </summary>
        public string promotionUrl = "";
        /// <summary>
        /// Tài nguyên được lưu vào máy sau khi load từ server.
        /// </summary>
        public string fileName = "";

        public bool IsNull() {
            return data.Length == 0;
        }
    }

    internal class AdSensparkResourceManager {
        private bool _initialized = false;
        private AdSensparkJsonData _adSensparkJsonData = new AdSensparkJsonData();

        /// <summary>
        /// Danh sách các file đã tạo.
        /// </summary>
        private List<string> paths = new List<string>();

        /// <summary>
        /// Khởi tạo dữ liệu.
        /// </summary>
        /// <param name="url">Đường dẫn để tải file json</param>
        /// <returns></returns>
        public async Task<bool> Initialize(string url) {
            if (_initialized) {
                Debug.LogWarning("Ad senspark: Gọi khởi tạo resource manager một lần thôi.");
                return false;
            }
            var jsonData = await Downloader.Load("ad_senspark_file_config", url, false);
            if (jsonData.Length <= 0) {
                Debug.LogWarning("Ad senspark: resource manager không load được file json ");
                return false;
            }
            var jsonBase64String = System.Text.Encoding.UTF8.GetString(jsonData);
            _adSensparkJsonData = JsonUtility.FromJson<AdSensparkJsonData>(jsonBase64String);
            _initialized = true;
            return true;
        }

        public void OnDestroy() {
            foreach (var path in paths) {
                System.IO.File.Delete(path);
            }
            paths.Clear();
        }

        /// <summary>
        /// Lấy tài nguyên dạng byte array
        /// </summary>
        /// <param name="adFormat"></param>
        /// <param name="index">index hỗ trợ xoay vòng ad. qua ver online thì không cần phần này.</param>
        /// <returns></returns>
        /// <exception cref="ArgumentOutOfRangeException"></exception>
        public async Task<AdSensparkResourcePack> GetResource(AdFormat adFormat, int index = 0) {
            var adSensparkResourcePack = new AdSensparkResourcePack();
            if (!_initialized) {
                Debug.LogWarning("Ad senspark: chưa khởi tạo resource manager.");
                return adSensparkResourcePack;
            }
            string dataUrl = _adSensparkJsonData.GetDataUrl(adFormat, index);
            var adAmount = _adSensparkJsonData.GetAdAmount(adFormat);
            var rollIndex = index % adAmount;
            string fileName = adFormat.ToString() + rollIndex;
            switch (adFormat) {
                case AdFormat.Banner:
                case AdFormat.Interstitial:
                case AdFormat.Rewarded: {
                    var data = await Downloader.Load(fileName, dataUrl);
                    string promotionUrl = _adSensparkJsonData.GetPromotionUrl(adFormat, index);
                    adSensparkResourcePack.promotionUrl = promotionUrl;
                    adSensparkResourcePack.data = data;
                    adSensparkResourcePack.fileName = fileName;
                    string path = Downloader.ParseToLocalPath(fileName);

                    if (!paths.Contains(path))
                        paths.Add(path);
                }
                    break;

                default: {
                    Debug.LogWarning("Ad senspark resource manager: chưa hỗ trợ ad loại này.");
                }
                    break;
            }

            return adSensparkResourcePack;
        }
    }
}