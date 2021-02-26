using System.IO;
using System.Threading.Tasks;

using UnityEngine;

namespace EE {
    public class NullLogManager : ILogManager {
        private static readonly UnityEngine.ILogger Logger = Debug.unityLogger;

        public Task<bool> Initialize() => Task.FromResult(true);

        public void Destroy() {
        }

        public void Log(
            string message = "",
            string memberName = "",
            string sourceFilePath = "",
            int sourceLineNumber = 0) {
            var fileName = Path.GetFileName(sourceFilePath);
            var messageWithColon = message.Length == 0 ? "" : $": {message}";
            var fullMessage = $"{fileName}:{sourceLineNumber}: {memberName + messageWithColon}";
            Logger.Log(LogType.Log, fullMessage);
        }
    }
}