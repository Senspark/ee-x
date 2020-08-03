using System;
using System.Collections.Generic;
using System.Linq;

namespace EE {
    public class ServiceNameCache {
        private readonly Dictionary<Type, string> _serviceNames = new Dictionary<Type, string>();

        public string GetServiceName<T>() where T : IService {
            return GetServiceName(typeof(T));
        }

        public string GetServiceName(Type type) {
            if (_serviceNames.TryGetValue(type, out var result)) {
                return result;
            }
            var interfaces = type.GetInterfaces().ToList();
            if (type.IsInterface) {
                interfaces.Add(type);
            }
            foreach (var item in interfaces) {
                var attribute = Attribute.GetCustomAttribute(item, typeof(ServiceAttribute));
                if (attribute is ServiceAttribute serviceAttribute) {
                    var name = serviceAttribute.Name;
                    _serviceNames.Add(type, name);
                    return name;
                }
            }
            throw new Exception($"The requested service is not registered: {type.Name}");
        }
    }
}