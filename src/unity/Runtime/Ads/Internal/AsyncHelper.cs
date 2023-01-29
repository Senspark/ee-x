using System;
using System.Threading.Tasks;

namespace EE.Internal {
    internal class AsyncHelper<Result> : IAsyncHelper<Result> {
        private TaskCompletionSource<Result> _source;
        private Action<Result> _finalizer;

        public bool IsProcessing { get; private set; }

        public Task<Result> Process(Action processor, Action<Result> finalizer) {
            if (IsProcessing) {
                // Waiting.
            } else {
                _source = new TaskCompletionSource<Result>();
                IsProcessing = true;
                _finalizer = finalizer;
                processor();
            }
            return _source.Task;
        }

        public void Resolve(Result result) {
            // Keep copies.
            var finalizer = _finalizer;
            var source = _source;

            // Update state.
            _finalizer = null;
            _source = null;
            IsProcessing = false;

            // Process.
            finalizer(result);
            source.SetResult(result);
        }
    }
}