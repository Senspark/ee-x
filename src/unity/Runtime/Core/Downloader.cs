using UnityEngine;

using System.Threading.Tasks;

using UnityEngine.Networking;

namespace EE.Internal {
    internal class Downloader {
        private static string ParseToLocalPath(string fileName) {
            return Application.persistentDataPath + "/" + fileName;
        }

        /// <summary>
        /// Tải dữ liệu.
        /// </summary>
        /// <param name="fileName">Tên file.</param>
        /// <param name="url">Link tải file</param>
        /// <returns>Bytes</returns>
        public static async Task<byte[]> Load(string fileName, string url) {
            var localPath = ParseToLocalPath(fileName);
            if (System.IO.File.Exists(localPath)) {
                var texture = await LoadFileOffline(localPath);
                if (texture != null)
                    return texture;
            }

            var res = await LoadFileOnline(localPath, url);
            return res;
        }

        /// <summary>
        /// Lấy dữ liệu offline.
        /// </summary>
        /// <param name="localPath"></param>
        /// <returns></returns>
        private static async Task<byte[]> LoadFileOffline(string localPath) {
            Debug.Log("Downloader: Load file da luu tren may. " + localPath);
            using var unityWebRequest = UnityWebRequest.Get("file://" + localPath);
            unityWebRequest.timeout = 10;
            await unityWebRequest.SendWebRequest();

            if (unityWebRequest.result == UnityWebRequest.Result.ConnectionError ||
                unityWebRequest.result == UnityWebRequest.Result.ProtocolError ||
                unityWebRequest.result == UnityWebRequest.Result.DataProcessingError) {
                Debug.LogWarning(unityWebRequest.error);
                return null;
            }

            var dlHandler = unityWebRequest.downloadHandler;

            if (dlHandler.isDone) {
                if (dlHandler.data.Length > 0) {
                    Debug.Log("Downloader: Load thanh cong file luu tren may.");
                    return dlHandler.data;
                }

                Debug.LogWarning("Downloader: Load file loi");
            } else {
                Debug.LogWarning("Downloader: Load file loi");
            }

            return null;
        }

        /// <summary>
        /// Tải file và lưu trữ file.
        /// </summary>
        /// <param name="localPath"></param>
        /// <param name="url"></param>
        /// <returns></returns>
        private static async Task<byte[]> LoadFileOnline(string localPath, string url) {
            Debug.Log($"Downloader: Khong tim thay file tren may \"{localPath}\". Load texture tu link : " + url);

            using var unityWebRequest = UnityWebRequest.Get(url);
            unityWebRequest.timeout = 10;
            await unityWebRequest.SendWebRequest();

            if (unityWebRequest.result == UnityWebRequest.Result.ConnectionError ||
                unityWebRequest.result == UnityWebRequest.Result.ProtocolError ||
                unityWebRequest.result == UnityWebRequest.Result.DataProcessingError) {
                Debug.LogWarning(unityWebRequest.error);
                return null;
            }

            var dlHandler = unityWebRequest.downloadHandler;

            if (dlHandler.isDone) {
                if (dlHandler.data.Length > 0) {
                    System.IO.File.WriteAllBytes(localPath, dlHandler.data);
                    Debug.Log($"Downloader: Tai thanh cong, luu file tai: {localPath}");
                    return dlHandler.data;
                }
                Debug.LogWarning("Downloader: File da tai khong phu hop :(");
            } else {
                Debug.LogWarning("Downloader: Tai file khong thanh cong.");
            }

            return null;
        }
    }
}