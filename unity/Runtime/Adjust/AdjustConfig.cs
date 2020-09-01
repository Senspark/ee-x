namespace EE {
    public class AdjustConfig {
        internal string _token;
        internal AdjustEnvironment _environment;
        internal AdjustLogLevel _logLevel;
        internal bool _eventBufferingEnabled;
        internal bool _useAppSecret;
        internal long _secretId;
        internal long _info1;
        internal long _info2;
        internal long _info3;
        internal long _info4;

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

        public AdjustConfig SetAppSecret(long secretId,
            long info1, long info2, long info3, long info4) {
            _useAppSecret = true;
            _secretId = secretId;
            _info1 = info1;
            _info2 = info2;
            _info3 = info3;
            _info4 = info4;
            return this;
        }
    }
}