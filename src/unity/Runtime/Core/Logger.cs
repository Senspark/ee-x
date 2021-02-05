namespace EE {
    public class Logger : ILogger {
        private readonly string _tag;

        public LogLevel LogLevel { get; set; } = LogLevel.Verbose;

        public Logger(string tag) {
            _tag = tag;
        }

        public void Error(string message) {
            if (LogLevel <= LogLevel.Error) {
                UnityEngine.Debug.LogError($"{_tag}: {message}");
            }
        }

        public void Warn(string message) {
            if (LogLevel <= LogLevel.Warning) {
                UnityEngine.Debug.LogWarning($"{_tag}: {message}");
            }
        }

        public void Debug(string message) {
            if (LogLevel <= LogLevel.Warning) {
                UnityEngine.Debug.Log($"{_tag}: {message}");
            }
        }
    }
}