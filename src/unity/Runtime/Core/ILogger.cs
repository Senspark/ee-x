namespace EE {
    public interface ILogger {
        void Error(string message);
        void Warn(string message);
        void Debug(string message);
    }
}