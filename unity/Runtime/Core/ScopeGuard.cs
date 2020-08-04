using System;

namespace EE {
    public sealed class ScopeGuard : IDisposable {
        private bool _disposed;
        private readonly Action _undoFunction;

        public ScopeGuard(Action undoFunction) {
            _undoFunction = undoFunction;
        }

        public void Dispose() {
            if (!_disposed) {
                _undoFunction();
            }
            _disposed = true;
        }
    }
}