namespace EE {
    public class AdjustConfig {
        internal string _token;
        internal AdjustEnvironment _environment;
        internal AdjustLogLevel _logLevel;

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
    }
}