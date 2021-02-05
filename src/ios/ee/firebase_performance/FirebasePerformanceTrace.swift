//
//  FirebasePerformanceTrace.swift
//  ee-x-bb8dc50f
//
//  Created by eps on 2/3/21.
//

private let kPrefix = "FirebasePerformance"

internal class FirebasePerformanceTrace {
    private let _bridge: IMessageBridge
    private let _traceName: String
    private let _trace: Trace

    public init(_ bridge: IMessageBridge, _ traceName: String, _ trace: Trace) {
        _bridge = bridge
        _traceName = traceName
        _trace = trace
        registerHandlers()
    }

    public func destroy() {
        deregisterHandlers()
    }

    private var kStart: String {
        return "\(kPrefix)_start_\(_traceName)"
    }

    private var kStop: String {
        return "\(kPrefix)_stop_\(_traceName)"
    }

    private var kIncrementMetric: String {
        return "\(kPrefix)_incrementMetric_\(_traceName)"
    }

    private var kGetLongMetric: String {
        return "\(kPrefix)_getLongMetric_\(_traceName)"
    }

    private var kPutMetric: String {
        return "\(kPrefix)_putMetric_\(_traceName)"
    }

    func registerHandlers() {
        _bridge.registerHandler(kStart) { _ in
            self.start()
            return ""
        }
        _bridge.registerHandler(kStop) { _ in
            self.stop()
            return ""
        }
        _bridge.registerHandler(kIncrementMetric) { message in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard
                let key = dict["key"] as? String,
                let value = dict["value"] as? Int64
            else {
                assert(false, "Invalid argument")
                return ""
            }
            self.incrementMetric(key, value)
            return ""
        }
        _bridge.registerHandler(kPutMetric) { message in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard
                let key = dict["key"] as? String,
                let value = dict["value"] as? Int64
            else {
                assert(false, "Invalid argument")
                return ""
            }
            self.putMetric(key, value)
            return ""
        }
        _bridge.registerHandler(kGetLongMetric) { message in
            "\(self.getLongMetric(message))"
        }
    }

    func deregisterHandlers() {
        _bridge.deregisterHandler(kStart)
        _bridge.deregisterHandler(kStop)
        _bridge.deregisterHandler(kIncrementMetric)
        _bridge.deregisterHandler(kPutMetric)
        _bridge.deregisterHandler(kGetLongMetric)
    }

    func start() {
        _trace.start()
    }

    func stop() {
        _trace.stop()
    }

    func incrementMetric(_ name: String, _ value: Int64) {
        _trace.incrementMetric(name, by: value)
    }

    func putMetric(_ name: String, _ value: Int64) {
        _trace.setValue(value, forMetric: name)
    }

    func getLongMetric(_ name: String) -> Int64 {
        return _trace.valueForMetric(name)
    }
}
