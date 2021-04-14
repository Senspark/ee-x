using System.Threading.Tasks;

namespace EE.Internal {
    public class Capper : ICapper {
        private readonly float _interval;
        private bool _capped;
        private bool _locked;

        public bool IsCapped => _capped || _locked;

        public Capper(float interval) {
            _interval = interval;
            _locked = false;
            _capped = false;
        }

        public void Cap() {
            _capped = true;
            Utils.NoAwait(async () => {
                await Task.Delay((int) (1000 * _interval));
                _capped = false;
            });
        }

        public void Lock() {
            _locked = true;
        }

        public void Unlock() {
            _locked = false;
        }
    }
}