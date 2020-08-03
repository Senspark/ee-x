using System;
using System.Collections.Generic;

namespace EE {
    public class ObserverHandle {
        private readonly List<ScopeGuard> _guards;

        public ObserverHandle() {
            _guards = new List<ScopeGuard>();
        }

        public ObserverBinder<Observer> Bind<Observer>(IObserverManager<Observer> manager) {
            return new ObserverBinder<Observer>(this, manager);
        }

        internal void Add(Action rollback) {
            _guards.Add(new ScopeGuard(rollback));
        }

        public void Clear() {
            foreach (var entry in _guards) {
                entry.Dispose();
            }
            _guards.Clear();
        }
    }
}