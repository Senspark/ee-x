namespace EE.Internal {
    internal class LockCapper : ICapper {
        private bool _locked;
        public bool IsCapped => _locked;

        public void Cap() {
            _locked = false;
        }

        public void Lock() {
            _locked = true;
        }

        public void Unlock() {
            _locked = false;
        }
    }
}