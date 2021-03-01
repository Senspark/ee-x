using System;

namespace EE {
    [AttributeUsage(AttributeTargets.Interface)]
    public class ServiceAttribute : Attribute {
        /// <summary>
        /// Gets the registered name of this service.
        /// </summary>
        public string Name { get; }

        public ServiceAttribute(string name) {
            Name = name;
        }
    }
}