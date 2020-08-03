namespace EE {
    public class ObserverBinder<Observer> {
        private readonly ObserverHandle _handle;
        private readonly IObserverManager<Observer> _manager;

        internal ObserverBinder(
            ObserverHandle handle,
            IObserverManager<Observer> manager) {
            _handle = handle;
            _manager = manager;
        }

        public ObserverBinder<Observer> AddObserver(Observer observer) {
            var id = _manager.AddObserver(observer);
            var manager = _manager;
            _handle.Add(() => { //
                manager.RemoveObserver(id);
            });
            return this;
        }
    }
}