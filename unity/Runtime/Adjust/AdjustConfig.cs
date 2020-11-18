namespace EE {
    public class AdjustConfig {
        internal string _token;
        internal AdjustEnvironment _environment;
        internal AdjustLogLevel _logLevel;
        internal bool _eventBufferingEnabled;

        public AdjustConfig SetToken(string token) {
            _token = token;
            return this;
        }

        public AdjustConfig SetEnvironment(AdjustEnvironment environment) {
            _environment = environment;
            return this;
        }

        public AdjustConfig SetLogLevel(AdjustLogLevel logLevel) {
            _logLevel = logLevel;
            return this;
        }

        public AdjustConfig SetEventBufferingEnabled(bool enabled) {
            _eventBufferingEnabled = enabled;
            return this;
        }
    }
}