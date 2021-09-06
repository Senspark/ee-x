using UnityEngine;

using System.Threading.Tasks;

using UnityEngine.Networking;

namespace EE.Internal {
    public class AdSensparkLoader {
        private string ParseToLocalPath(string fileName) {
            return Application.persistentDataPath + "/" + fileName;
        }

        /// <summary>
        ///  Tải file .
        /// </summary>
        /// <returns></returns>
        public async Task<byte[]> Load(string fileName, string link) {
            var localPath = ParseToLocalPath(fileName);
            if (System.IO.File.Exists(localPath)) {
                var texture = await LoadFileOffline(localPath);
                if (texture != null)
                    return texture;
            }

            var res = await LoadFileOnline(localPath, link);
            return res;
        }

        private async Task<byte[]> LoadFileOffline(string localPath) {
            Debug.Log("Load file da luu tren may. " + localPath);
            using var unityWebRequest = UnityWebRequest.Get("file://" + localPath);
            unityWebRequest.timeout = 10;
            await unityWebRequest.SendWebRequest();

            if (unityWebRequest.result == UnityWebRequest.Result.ConnectionError ||
                unityWebRequest.result == UnityWebRequest.Result.ProtocolError ||
                unityWebRequest.result == UnityWebRequest.Result.DataProcessingError) {
                Debug.Log(unityWebRequest.error);
                return null;
            }

            var dlHandler = unityWebRequest.downloadHandler;

            if (dlHandler.isDone) {
                if (dlHandler.data.Length > 0) {
                    Debug.Log("Load thanh cong file luu tren may.");
                    return dlHandler.data;
                }

                Debug.LogError("Load file loi");
            } else {
                Debug.LogError("Load file loi");
            }

            return null;
        }

        private async Task<byte[]> LoadFileOnline(string localPath, string link) {
            Debug.Log($"Khong tim thay file tren may \"{localPath}\". Load texture tu link : " + link);

            using var unityWebRequest = UnityWebRequest.Get(link);
            unityWebRequest.timeout = 10;
            await unityWebRequest.SendWebRequest();

            if (unityWebRequest.result == UnityWebRequest.Result.ConnectionError ||
                unityWebRequest.result == UnityWebRequest.Result.ProtocolError ||
                unityWebRequest.result == UnityWebRequest.Result.DataProcessingError) {
                Debug.LogError(unityWebRequest.error);
                return null;
            }

            var dlHandler = unityWebRequest.downloadHandler;

            if (dlHandler.isDone) {
                if (dlHandler.data.Length > 0) {
                    System.IO.File.WriteAllBytes(localPath, dlHandler.data);
                    Debug.Log($"Tai thanh cong, luu file tai: {localPath}");
                    return dlHandler.data;
                }
                Debug.LogError("File da tai khong phu hop :(");
            } else {
                Debug.LogError("Tai file khong thanh cong.");
            }

            return null;
        }
    }
}