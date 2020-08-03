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
            }
            return _source.Task;
        }

        public void Resolve(Result result) {
            _finalizer(result);
            _finalizer = null;
            _source.SetResult(result);
            _source = null;
            IsProcessing = false;
        }
    }
}