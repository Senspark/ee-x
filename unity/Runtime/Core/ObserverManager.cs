using System;
using System.Collections.Generic;

namespace EE {
    public class ObserverManager<Observer> : IObserverManager<Observer> {
        private int _counter;
        private readonly Dictionary<int, Observer> _observers;

        public ObserverManager() {
            _counter = 0;
            _observers = new Dictionary<int, Observer>();
        }

        public int AddObserver(Observer observer) {
            var id = _counter++;
            _observers.Add(id, observer);
            return id;
        }

        public bool RemoveObserver(int id) {
            return _observers.Remove(id);
        }

        public void DispatchEvent(Action<Observer> dispatcher) {
            foreach (var entry in _observers) {
                dispatcher(entry.Value);
            }
        }
    }
}