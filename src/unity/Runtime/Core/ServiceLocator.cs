using System;
using System.Collections.Generic;

namespace EE {
    internal class ServiceLocatorImpl {
        private readonly Dictionary<string, IService> _services = new Dictionary<string, IService>();
        private readonly ServiceNameCache _nameCache = new ServiceNameCache();

        public void Provide(IService service) {
            var type = service.GetType();
            var name = _nameCache.GetServiceName(type);
            if (_services.TryGetValue(name, out var currentService)) {
                currentService.Destroy();
            }
            _services.Remove(name);
            _services.Add(name, service);
        }

        public T Resolve<T>() where T : IService {
            var name = _nameCache.GetServiceName<T>();
            if (_services.TryGetValue(name, out var item)) {
                if (item is T service) {
                    return service;
                }
            }
            throw new Exception($"Cannot find the requested service: {name}");
        }
    }

    public class ServiceLocator {
        private static readonly ServiceLocatorImpl _impl = new ServiceLocatorImpl();

        /// <summary>
        /// Registers a service.
        /// </summary>
        /// <param name="service"></param>
        public static void Provide(IService service) {
            _impl.Provide(service);
        }

        /// <summary>
        /// Resolves the specified service.
        /// </summary>
        public static T Resolve<T>() where T : IService {
            return _impl.Resolve<T>();
        }
    }
}