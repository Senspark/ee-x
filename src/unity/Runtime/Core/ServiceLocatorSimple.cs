using System;
using System.Collections.Generic;

using UnityEngine;

namespace EE {
    public class ServiceLocatorSimple {
        private static readonly Dictionary<object, object> _services = new Dictionary<object, object>();
        
        /// <summary>
        ///     Thêm một dịch vụ.
        /// </summary>
        /// <param name="service"></param>
        /// <typeparam name="T"></typeparam>
        public static T AddService<T>(T service) {
            if (_services.ContainsKey(typeof(T)))
                Debug.LogWarning($"Service {typeof(T)} already added.");
            else
                _services.Add(typeof(T), service);

            return service;
        }

        /// <summary>
        ///     Lấy một dịch vụ.
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <returns></returns>
        /// <exception cref="NullReferenceException"></exception>
        public static T GetService<T>() {
            try {
                return (T) _services[typeof(T)];
            }
            catch (Exception ex) {
                throw new NullReferenceException($"Service {typeof(T)} not available {ex}");
            }
        }

        /// <summary>
        ///     Xoá dịch vụ.
        /// </summary>
        /// <param name="service"></param>
        /// <typeparam name="T"></typeparam>
        public static void RemoveService<T>(T service) {
            if (_services.ContainsKey(typeof(T)))
                if (_services[typeof(T)].Equals(service))
                    _services.Remove(typeof(T));
        }

        /// <summary>
        ///     Kiểm tra dịch vụ đã có chưa.
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <returns></returns>
        public static bool ServiceAvailable<T>() {
            return _services.ContainsKey(typeof(T));
        }
    }
}