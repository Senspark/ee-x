using System;
using System.Threading.Tasks;

using Random = UnityEngine.Random;

namespace EE.Internal {
    using Processor = Func<Task<bool>>;

    internal class Retrier {
        private readonly float _baseDelay;
        private readonly float _multiplier;
        private readonly float _limit;
        private bool _processing;
        private bool _stopRequested;

        public Retrier(float baseDelay, float multiplier, float limit) {
            _baseDelay = baseDelay;
            _multiplier = multiplier;
            _limit = limit;
        }

        public async Task Process(Processor processor) {
            if (_processing) {
                return;
            }
            _processing = true;
            var delay = _baseDelay;
            while (true) {
                var x = Random.Range(0, 1);
                await Task.Delay((int) ((delay + x) * 1000));
                if (_stopRequested) {
                    _processing = false;
                    _stopRequested = false;
                    return;
                }
                var result = await processor();
                if (result) {
                    _processing = false;
                    return;
                }
                delay *= _multiplier;
                delay = Math.Min(delay, _limit);
            }
        }

        public void Stop() {
            if (_processing) {
                _stopRequested = true;
            }
        }
    }
}