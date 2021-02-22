using System.Threading.Tasks;

namespace EE.Internal {
    public class Capper : ICapper {
        private float _interval;

        public bool IsCapped { get; private set; }

        public Capper(float interval) {
            _interval = interval;
            IsCapped = false;
        }

        public void Cap() {
            IsCapped = true;
            Utils.NoAwait(async () => {
                await Task.Delay((int) (1000 * _interval));
                IsCapped = false;
            });
        }
    }
}